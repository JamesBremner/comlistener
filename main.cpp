#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

#include <nana/gui.hpp>
#include  <nana/gui/timer.hpp>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace boost::asio;

#define EXPECTED_BYTES 128

std::vector< unsigned char > vBuffer;    // transer data from asio thread to nana thread
std::mutex mutex;                       // protect tranfer buffer from data races

class cGUI
{
public:
    cGUI();
    void StartTimer();
    void OnTimer();     /// check for new data received by asio thread
private:
    nana::form  fm;
    nana::timer t;      /// timer to trigger checks for new data received by the asio thread
};

class cAsioThread
{
public:

    /** \brief CTOR
        \param[in] port  COM port to listen at, C-style string

        Call from main thread to start things rolling

        Starts a new thread
        where everything else in this class runs
    */
    cAsioThread( char* port );       ///< Call from main thread to start things rolling

    /// Open connection to COM port
    void Connect();

    /// read simulator used when no COM available for testing
    void OnRead();

    /// start an asynchronous read
    void NextRead();

    /// handle completion of an asynchronous read
    void handle_read(
        const boost::system::error_code& error,
        std::size_t bytes_received );

private:
    char* myPortNumber;
    std::thread * myThread;

    boost::asio::io_service myEventManager;
    boost::asio::serial_port * myPort;
    unsigned char myRcvBuffer[ EXPECTED_BYTES ];
};

cGUI::cGUI()
    : fm(nana::rectangle( 100,100, 300, 300 ) )
{
    fm.show();
    StartTimer();
}

void cGUI::OnTimer()
{
    nana::msgbox msg("Data Received");
    {
        std::lock_guard<std::mutex> lck (mutex);

        if( !vBuffer.size() )
            return;

        msg << vBuffer.size() << " bytes";

        vBuffer.clear();
    }

    msg.show();
}

void cGUI::StartTimer()
{
    t.interval( std::chrono::milliseconds( 100 ) );
    t.elapse( [this]()
    {
        OnTimer();
    });
    t.start();
}

cAsioThread::cAsioThread( char* port )
    : myPortNumber( port )
{
    myThread = new std::thread(
        &cAsioThread::Connect,
        std::ref( *this ) );
    std::this_thread::sleep_for (std::chrono::seconds( 3 ));
    bool ok = false;
    if( myPort )
        if( myPort->is_open() )
        {
            ok = true;
        }
    if( ! ok )
    {
        std::cout << "Failed to open COM port\n";
        exit(1);
    }
}
void cAsioThread::Connect()
{
#ifdef DEBUG_NO_COM
//    while( true )
//    {
//        std::this_thread::sleep_for (std::chrono::seconds(1));
//        OnRead();
//    }
#else

    try
    {
        myPort = new boost::asio::serial_port(
            myEventManager,
            "//./COM" + std::string( myPortNumber ));
    }
    catch( boost::system::system_error& e )
    {
        std::cout << e.what() << " Error opening port\n";
        myPort = 0;
        return;
    }
    // configure port

    myPort->set_option(serial_port_base::baud_rate(      9600                        ));
    myPort->set_option(serial_port_base::parity(         serial_port_base::parity::none  ));
    myPort->set_option(serial_port_base::character_size( serial_port::character_size(8)  ));
    myPort->set_option(serial_port_base::stop_bits(      serial_port::stop_bits::one     ));
    myPort->set_option(serial_port_base::flow_control(   serial_port::flow_control::none ));

    if( ! myPort->is_open() )
        return;

    NextRead();

    return;
#endif // DEBUG_NO_COM
}

void cAsioThread::NextRead()
{
    async_read(
        *myPort,
        buffer(myRcvBuffer, EXPECTED_BYTES),
        boost::bind(&cAsioThread::handle_read, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred ));
}
void cAsioThread::OnRead()
{
    static int count = 5;
    count++;
    std::cout << "read " << count << "\n";
    std::lock_guard<std::mutex> lck (mutex);
    for( int k = 0; k < count; k++ )
        vBuffer.push_back( (unsigned char) k );
}

void cAsioThread::handle_read(
    const boost::system::error_code& error,
    std::size_t bytes_received )
{
    {
        std::lock_guard<std::mutex> lck (mutex);
        for( int k = 0; k < bytes_received; k++ )
            vBuffer.push_back( myRcvBuffer[ k ] );
    }
    NextRead();
}

int main( int argc, char* argv[] )
{
    if( argc != 2 )
    {
        std::cout << "Usage: comlistener <com port number>\n";
        exit(1);
    }
    cAsioThread theAsioThread( argv[1] );

    cGUI GUI;

    nana::exec();

}
