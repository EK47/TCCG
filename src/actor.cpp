/*

    The Calvin Chronicle's Game
    Copyright (C) 2018 Ethan Kelly

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "main.hpp"

// Initialize all actor class variables.
Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &col) : x(x), y(y), ch(ch), col(col), name(name), blocks(true), 
    attacker( nullptr ), destructible( nullptr ), ai( nullptr ), pickable( nullptr ), container( nullptr ), 
    turnSinceFight( turnSinceFight )
{

}

Actor::~Actor()
{
    
}

// Find the distance between two points
float Actor::getDistance( int cx, int cy ) const
{
    return hypot( x - cx, y - cy );
}

// Render the actors character, and last known locations.
void Actor::render() const {
    engine.map -> mapConsole -> setChar( lastLocationX, lastLocationY, ch);
    engine.map -> mapConsole -> setCharForeground( lastLocationX, lastLocationY, col);
}

// Only the AI needs to be updated. The AI essentially handles all activities.
void Actor::update( std::shared_ptr<Actor> owner ) {
	if( ai ) ai -> update( owner );
}