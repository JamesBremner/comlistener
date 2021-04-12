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

