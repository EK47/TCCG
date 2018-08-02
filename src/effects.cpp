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

void renderLine( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> target, float multiplier, const TCODColor &col )
{
    engine.render();
    // Current x and y;
    int cX = owner -> x - engine.camera -> topLeftX;
    int cY = owner -> y - engine.camera -> topLeftY;
    // Create a line from the curent to the target. Subtract camera coordinates to get proper map position.
    TCODLine::init( cX, cY, target -> x - engine.camera -> topLeftX, target -> y - engine.camera -> topLeftY );
    do
    {
        // Get a random integer to cause color change in the line.
        TCODRandom *rng = TCODRandom::getInstance();
        float val = rng -> getInt( 1, 3 );
        val = val * ( 0.2f * multiplier );
        engine.camera -> cameraConsole -> setCharBackground( cX, cY, col * val );
    } while( !TCODLine::step( &cX, &cY ) );
    
    TCODConsole::flush();
}