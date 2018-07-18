#include "main.hpp"

Engine engine( 160, 100 );

int main() {

    while ( !TCODConsole::isWindowClosed() ) 
    {
        engine.update();
        engine.render();
        TCODConsole::flush();
    }

    return 0;
}
