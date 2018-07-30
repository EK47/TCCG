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

Camera::Camera( int width, int height, bool mainCamera ) : lookMode( false ), mainCamera( mainCamera )
{
    cameraConsole = new TCODConsole( cameraWidth, cameraHeight );
    cameraWidth = width;
    cameraHeight = height;
    if( mainCamera )
    {
        cameraPosX = 0;
        cameraPosY = 0;
    }
}

Camera::~Camera()
{
    delete cameraConsole;
}

void Camera::playerOutRange()
{
    topLeftX = engine.player -> x - (int)cameraWidth / 2;
    topLeftY = engine.player -> y - (int)cameraHeight / 2;
    
    if( engine.trackPlayer )
    {
        topLeftX = std::min( std::max( engine.player -> x - (int)cameraWidth / 2, 0 ), engine.map -> width - cameraWidth );
        topLeftY = std::min( std::max( engine.player -> y - (int)cameraHeight / 2, 0 ), engine.map -> height - cameraHeight ); 
    }
}

void Camera::moveAround()
{
    lookMode = true;
    // Center the look 'x' on the player
    topLeftX = engine.player -> x - cameraWidth / 2;
    topLeftY = engine.player -> y - cameraHeight / 2;
    while( lookMode )
    {
        TCOD_key_t key = TCODConsole::checkForKeypress( TCOD_KEY_PRESSED );
        switch( key.c )
        {
            case 'k': topLeftY -= 1; break;
            case 'u': topLeftX += 1; topLeftY -= 1; break;
            case 'l': topLeftX += 1; break;
            case 'n': topLeftX += 1; topLeftY += 1; break;
            case 'j': topLeftY += 1; break;
            case 'b': topLeftX -= 1; topLeftY += 1; break;
            case 'h': topLeftX -= 1; break;
            case 'y': topLeftX -= 1; topLeftY -= 1; break;
            case ';': 
                topLeftX = engine.player -> x - cameraWidth / 2;
                topLeftY = engine.player -> y - cameraHeight / 2;
            break;
            case '\'':
                lookMode = false;
            break;
            default: break;
        }
        TCODConsole::root -> clear();
        // Blit the map onto the camera
        TCODConsole::blit( engine.map -> mapConsole, topLeftX, topLeftY, cameraWidth, cameraHeight, cameraConsole, 0, 0 );
        // Put an X of guiForeground color onto the center of the camera.
        cameraConsole -> putChar( cameraWidth / 2, cameraHeight / 2, 'x' );
        cameraConsole -> setCharForeground( cameraWidth / 2, cameraHeight / 2, guiForeground );
        // Blit the camera onto the root console
        TCODConsole::blit( cameraConsole, 0, 0, cameraWidth, cameraHeight, TCODConsole::root, cameraPosX, cameraPosY );
        // Render the GUI
        engine.gui -> render();
        // Flush the root console to display all things
        TCODConsole::flush();
    }
}

void Camera::render()
{   
    if( mainCamera )
    {
        playerOutRange();
    }
    TCODConsole::blit( engine.map -> mapConsole, topLeftX, topLeftY, cameraWidth, cameraHeight, cameraConsole, 0, 0 );
    TCODConsole::blit( cameraConsole, 0, 0, cameraWidth, cameraHeight, TCODConsole::root, cameraPosX, cameraPosY );
}