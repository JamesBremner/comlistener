#include <sstream>
#include "com.h"

class cGUI
{
public:
    cGUI();
    void run();
private:
    wex::gui&               myForm;
    wex::label&             myPortlb;
    wex::editbox&           myPorteb;
    wex::button&            myConnectbn;
    wex::com&               myTalker;
    wex::list&              myRcvList;
};

cGUI::cGUI()
    : myForm( wex::maker::make() )
    , myPortlb( wex::maker::make<wex::label>( myForm ))
    , myPorteb( wex::maker::make<wex::editbox>( myForm ))
    , myConnectbn( wex::maker::make<wex::button>( myForm ))
    , myTalker( wex::maker::make<wex::com>( myForm ))
    , myRcvList( wex::maker::make<wex::list>( myForm ))
{
    myForm.move( 50,50,450,400);
    myForm.text("COM Explorer");

//    myPortlb.move(20,20,100,30);
//    myPortlb.text("COM Port");
//    myPorteb.move(140,20,50,30);
//    myPorteb.text("9");
    myConnectbn.move( 210, 20, 100,30);
    myConnectbn.bgcolor(0x9090FF);
    myConnectbn.text("Connect");

    myRcvList.move( 20,70, 400,300 );

    myConnectbn.events().click([&]
    {
        // user wants to connect to COM ports

        for( int kport = 1; kport < 13; kport++ )
        {
            std::stringstream ss;
            wex::com& myTalker = wex::maker::make<wex::com>( myForm );
            myTalker.port( std::to_string( kport ) );
            myTalker.open();
            if( ! myTalker.isOpen() )
            {
                ss << "Open failed on COM" << kport;
            }
            else
            {
                ss << "Open on COM" << kport;
            }
            myRcvList.add( ss.str() );
            myRcvList.update();
        }

    });

//    myForm.events().asyncReadComplete([&]( int id )
//    {
//        // packet has been received
//
//        auto r = myTalker.readData();
//        myRcvList.add( "read " + std::to_string( r.size() ) + " bytes");
//        std::stringstream ss;
//        ss << std::hex;
//        int k = 0;
//        for( auto c : r )
//        {
//            ss << "0x" << (int)c << " ";
//            k++;
//            if( k > 9 )
//            {
//                myRcvList.add( ss.str() );
//                ss.str("");
//                k = 0;
//            }
//        }
//        myRcvList.add( ss.str() );
//        myRcvList.update();
//
//        // wait for next
//
//        myTalker.read_async( -1 );
//    });

    myForm.show();
}
void cGUI::run()
{
    myForm.run();
}

int main()
{
    cGUI gui;
    gui.run();
}

