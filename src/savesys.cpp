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

void Engine::adventureInit()
{
    actors.clear();
    // Creates the player
    player = std::make_shared<Actor>( 1, 1, '@', "Calvin", calvin1 );
    player -> destructible = std::make_shared<PlayerDestructible>( 30, 2, "Your Corpse" );
    player -> attacker = std::make_shared<Attacker>( 5 );
    player -> ai = std::make_shared<PlayerAi>();
    player -> container = std::make_shared<Container>( 10 );
    actors.push_back( player );
    map = std::make_shared<Map>( defaultMapSize, defaultMapSize );
    camera = std::make_shared<Camera>( (int)( 3 * screenWidth ) / 4, (int)( 3 * screenHeight ) / 4, true );
}

void Engine::restartAdventure()
{
    actors.clear();
    player = std::make_shared<Actor>( 1, 1, '@', "Calvin", calvin1 );
    player -> destructible = std::make_shared<PlayerDestructible>( 30, 2, "Your Corpse" );
    player -> attacker = std::make_shared<Attacker>( 5 );
    player -> ai = std::make_shared<PlayerAi>();
    player -> container = std::make_shared<Container>( 10 );
    actors.push_back( player );
    map = std::make_shared<Map>( defaultMapSize, defaultMapSize );
    gui -> clear();

    gameStatus = STARTUP;
}

void Engine::load()
{
    TCODConsole::root -> clear();
    engine.gui -> menu.clear();
    engine.gui -> options.clear();
    engine.gui -> menu.addItem( Menu::NEW_GAME, Menu::optionOptions::ONONE, "New Game" );
    if( TCODSystem::fileExists("game.sav") || ( engine.map && engine.player -> destructible -> isAlive() ) )
    {
        engine.gui -> menu.addItem( Menu::CONTINUE, Menu::optionOptions::ONONE, "Continue" );
    }
    engine.gui -> menu.addItem( Menu::OPTIONS, Menu::optionOptions::ONONE, "Options" );
    //engine.gui -> menu.addItem( Menu::CREDITS, Menu::optionOptions::ONONE, "Credits" );
    engine.gui -> menu.addItem( Menu::QUIT, Menu::optionOptions::ONONE, "Quit Game" );

    engine.gui -> options.addItem( Menu::NONE, Menu::CAMERA, "Camera Type" );
    engine.gui -> options.addItem( Menu::NONE, Menu::BACK, "Go Back" );

    while( true )
    {
        if( TCODConsole::isWindowClosed() )
        {
            exit(0);
        }

        Menu::menuOptions menuItem = engine.gui -> menu.pick();

        if( menuItem == Menu::menuOptions::QUIT )
        {
            exit(0);
        } else if( menuItem == Menu::NEW_GAME )
        {
            if( engine.camera )
            {
                restartAdventure();
            } else
            {
                adventureInit();
            }
            break;
        } else if( menuItem == Menu::CONTINUE )
        {
            break;
        } else if( menuItem == Menu::OPTIONS )
        {

            Menu::optionOptions optionItem = engine.gui -> options.oPick();

            if( optionItem == Menu::CAMERA )
            {
                trackPlayer = !trackPlayer;
            } else if( optionItem == Menu::BACK )
            {
            }
        } else if( menuItem == Menu::CREDITS )
        {

        }
    }
}

void Engine::save()
{

}