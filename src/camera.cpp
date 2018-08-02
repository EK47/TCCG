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
    // Initialize a new console with specified width and height, and a position on the root console.
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
    // The camera always has the player in the center.
    topLeftX = engine.player -> x - (int)cameraWidth / 2;
    topLeftY = engine.player -> y - (int)cameraHeight / 2;
    

    if( engine.trackPlayer )
    {
        // The camera is more loose, and keeps the player in a certain proximity to the center.
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
        // Movement directions for the camera. ; to recenter, and ' to leave camera mode.
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

        // Render the camera as usual
        render();

        // Render the GUI
        engine.gui -> render();
        // Flush the root console to display all things
        TCODConsole::flush();
    }
}

void Camera::render()
{   
    // If this camera is the main one, center on the player.
    if( mainCamera && !lookMode )
    {
        playerOutRange();
    }
    
    // Blit the map onto this camera, and the camera onto the root console.
    TCODConsole::blit( engine.map -> mapConsole, topLeftX, topLeftY, cameraWidth, cameraHeight, cameraConsole, 0, 0 );

    // Camera Processing Effects. Lighting is added here.

    for( int x = 0; x < cameraWidth; ++x )
    {
        for( int y = 0; y < cameraHeight; ++y )
        {
            TCODColor colorF = engine.map -> mapConsole -> getCharForeground( x + topLeftX, y + topLeftY );
            TCODColor colorB = engine.map -> mapConsole -> getCharBackground( x + topLeftX, y + topLeftY );

            float brightnessValue = engine.map -> lightmask -> mask[ ( x + topLeftX ) + engine.map -> width * ( y + topLeftY ) ];

            colorF = colorF * ( TCODColor::lightYellow * brightnessValue * 0.5 );

            colorB = colorB * ( TCODColor::lightYellow * brightnessValue );

            cameraConsole -> setCharForeground( x, y, colorF );
            cameraConsole -> setCharBackground( x, y, colorB );
        }
    }

    if( lookMode )
    {
        // Put an X of guiForeground color onto the center of the camera.
        cameraConsole -> putChar( cameraWidth / 2, cameraHeight / 2, 'x' );
        cameraConsole -> setCharForeground( cameraWidth / 2, cameraHeight / 2, guiForeground );
    }

    TCODConsole::blit( cameraConsole, 0, 0, cameraWidth, cameraHeight, TCODConsole::root, cameraPosX, cameraPosY );
}