#include "main.hpp"

// Initialize all actor class variables.
Actor::Actor(int x, int y, int ch, const char *name, 
    const TCODColor &col) :
    x(x),y(y),ch(ch),col(col),name(name),
    blocks(true),attacker( nullptr ),destructible( nullptr ),ai( nullptr ),
    pickable( nullptr ), container( nullptr ), turnSinceFight( turnSinceFight ) {
}

Actor::~Actor()
{ }

// Find the distance between two points
float Actor::getDistance( int cx, int cy ) const
{
    int dx = x - cx;
    int dy = y - cy;
    return sqrtf( dx*dx + dy*dy );
}

void Actor::render() const {
    TCODConsole::root->setChar( lastLocationX, lastLocationY, ch);
    TCODConsole::root->setCharForeground( lastLocationX, lastLocationY, col);
}

void Actor::update( std::shared_ptr<Actor> owner ) {
	if ( ai ) ai->update( owner );
}