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

Door::Door( int x, int y, int ch, const char *name, const TCODColor &col ) : Actor( x, y, ch, name, col )
{
    
}

Door::~Door()
{

}

void Door::genericInteraction()
{
    if( state == Door::OPEN )
    {
        state = Door::CLOSED;
        blocks = true;
        ch = '=';
        engine.map -> map -> setProperties( x, y, false, false );
    } else if( state == Door::CLOSED )
    {
        state = Door::OPEN;
        blocks = false;
        ch = '/';
        engine.map -> map -> setProperties( x, y, true, true );
    }
}

int Door::returnInfo()
{
    return static_cast<int>( state );
}