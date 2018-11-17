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

static const bool FULLSCREEN = false;

Engine::Engine( int screenWidth, int screenHeight ) : gameStatus( STARTUP ), screenWidth( screenWidth ), screenHeight( screenHeight ), FOVRadius( 30 ), currentFOV( FOVRadius ), trackPlayer( true ), defaultMapSize( 200 )
{
    // Limits FPS as to protect CPU usage
    TCODSystem::setFps( 60 );
    TCODConsole::initRoot( screenWidth, screenHeight, "The Calvin Chronicles Game", FULLSCREEN );
    TCODConsole::setCustomFont( "terminal16x17_gs_ro.png", TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW );
    // We must create the GUI so the main menu can be displayed
    gui = std::make_shared<Gui>();
}

Engine::~Engine()
{
    // Clear the list of actors
    actors.clear();
}

void Engine::update() {
    // Compute the FOV of the map
	if( gameStatus == STARTUP )
    {
        map -> computeFov();
    }

   	gameStatus = IDLE;

    TCODSystem::checkForEvent( TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &lastKey, &mouse );
    // Go the main menu
    if( lastKey.vk == TCODK_ESCAPE )
    {
        save();
        load();
    }
    // Update the player
    player -> update( player );
    // If the game has gone to a new turn, update all living actors that aren't the player
    if ( gameStatus == NEW_TURN )
    {
	    for ( auto &actor : actors )
        {
	        if ( actor -> name != player -> name && actor -> destructible && actor -> ai )
            {
	            if( actor -> destructible -> isAlive() )
                {
                    actor -> update( actor );
                }
	        }
	    }
        /*for( auto &actor : actors )
        {
            if ( actor->name != player -> name && actor->destructible && actor->ai )
            {
	            if( !actor->destructible->isDead() )
                {
                    map -> map -> setProperties( actor -> x, actor -> y, true, true );
                }
	        }      
        }*/
	}
}

void Engine::render()
{
    TCODConsole::root -> clear();
    map -> render();

    // Render items first, all else second.
    for( int i = 0; i <= 1; i++ )
    {
        for( auto &actor : actors )
        {
            // If it is pickable or dead, render it on the first run. If it has ai, render it on the second loop.
            if( ( (bool)i && actor -> pickable ) || ( !((bool)i) && actor -> ai ) )
            {
                continue;
            }
            if( map -> isInFov( actor -> x, actor -> y ) )
            {
                actor -> lastLocationX = actor -> x;
                actor -> lastLocationY = actor -> y;
                actor -> render();
            } else if( map -> isInFov( actor -> lastLocationX, actor -> lastLocationY ) )
            {
                actor -> lastLocationX = NULL;
                actor -> lastLocationY = NULL;
            } else if( !( map -> isInFov( actor -> x, actor -> y) ) && actor -> lastLocationX != NULL && actor -> lastLocationY != NULL )
            {
                actor -> render();
            }
        }
    }

    player -> render();
    camera -> render();
    gui -> render();
}

std::shared_ptr<Actor> Engine::getClosestMonster( int x, int y, float range ) const
{
    std::shared_ptr<Actor> closest = NULL;
    float bestDistance = 1E6;

    // Loops through actors and finds the nearest one in a range
    for( auto &actor : actors )
    {
        if( actor != player && actor -> destructible && actor -> destructible -> isAlive() )
        {
            float distance = actor -> getDistance( x, y );
            if( distance < bestDistance && ( distance <= range || range == 0.0f ) )
            {
                bestDistance = distance;
                closest = actor;
            }
        }
    }
    return closest;
}

std::shared_ptr<Actor> Engine::getActor( int x, int y ) const
{
    for( auto &actor : actors )
    {
        if( actor -> x == x && actor -> y == y && actor -> destructible && actor -> destructible -> isAlive() )
        {
            return actor;
        }
    }
    return NULL;
}

float Engine::getMouseDistance( int cx, int cy )
{
    int dx = mouse.cx - cx;
    int dy = mouse.cy - cy;
    return sqrtf( dx*dx + dy*dy );
}

bool Engine::pickATile(int *x, int *y, float displayRange, float maxRange )
{
	// While the window isn't closed, render the engine.
    while ( !TCODConsole::isWindowClosed() ) {
        render();

		// Highlight the possible range and the range of whatever is being picked.
		for (int cx = 0; cx < camera -> cameraWidth; cx++ )
        {
			for (int cy = 0; cy < camera -> cameraHeight; cy++ )
            {
				if ( map->isInFov( cx + camera -> topLeftX, cy + camera -> topLeftY ) && ( maxRange == 0 || player->getDistance(cx + camera -> topLeftX, cy + camera -> topLeftY ) <= maxRange) )
                {
                    TCODColor col = engine.camera -> cameraConsole -> getCharBackground( cx, cy );
					col = col * 1.4f;
					engine.camera -> cameraConsole ->setCharBackground( cx, cy, col );
				}
                if( displayRange != 0 && ( ( getMouseDistance( cx, cy ) <= displayRange ) && player -> getDistance( mouse.cx + camera -> topLeftX, mouse.cy + camera -> topLeftY ) <= maxRange ) )
                {
                    TCODColor col = engine.camera -> cameraConsole -> getCharBackground( cx, cy );
                    col = col * 1.4f;
                    engine.camera -> cameraConsole -> setCharBackground( cx, cy, col );
                }
			}
        }

        TCODSystem::checkForEvent( TCOD_EVENT_ANY, &lastKey, &mouse );
        if( map -> isInFov( mouse.cx + camera -> topLeftX, mouse.cy + camera -> topLeftY ) && ( maxRange == 0 || player -> getDistance( mouse.cx + camera -> topLeftX, mouse.cy + camera -> topLeftY ) <= maxRange ) )
        {
			engine.camera -> cameraConsole -> setCharBackground( mouse.cx, mouse.cy, TCODColor::white );
			if ( mouse.lbutton_pressed )
            {
				*x = mouse.cx + camera -> topLeftX;
				*y = mouse.cy + camera -> topLeftY;
				return true;
			}
		}
		if ( mouse.rbutton_pressed ) {
			return false;
		}
        
        TCODConsole::flush();
	}
	return false;
}