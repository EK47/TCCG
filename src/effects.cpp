#include "main.hpp"

void renderLine( Actor *owner, Actor *target, float multiplier, const TCODColor &col )
{
    // Current x and y;
    int cX = owner -> x;
    int cY = owner -> y;
    TCODLine::init( cX, cY, target -> x, target -> y );
    do
    {
        TCODRandom *rng = TCODRandom::getInstance();
        float val = rng -> getInt( 1, 3 );
        val = val * ( 0.2f * multiplier );
        TCODConsole::root -> setCharBackground( cX, cY, col * val );
    } while( !TCODLine::step( &cX, &cY ) );
    TCODConsole::flush();
}