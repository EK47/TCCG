#include "main.hpp"
#include <unistd.h>

static const bool FULLSCREEN = false;

Engine::Engine( int screenWidth, int screenHeight ) : gameStatus( STARTUP ), screenWidth( screenWidth ), screenHeight( screenHeight )
{

    // Limits FPS as to protect CPU usage
    TCODSystem::setFps( 60 );
    TCODConsole::initRoot( 80, 50, "The Calvin Chronicles Game", FULLSCREEN );
    // Creates the player
    player = new Actor( 40, 25, '@', "Calvin", TCODColor ( 250, 240, 190 ) );
    player -> destructible = new PlayerDestructible( 30, 2, "Your Corpse" );
    player -> attacker = new Attacker( 5 );
    player -> ai = new PlayerAi();
    player -> container = new Container( 26 );
    actors.push( player );
    map = new Map( 80, 45 );
    gui = new Gui();

    gui->message( TCODColor::red, "Save us, Calvin!\n""You're our only hope!" );
}

Engine::~Engine()
{
    actors.clearAndDelete();
    delete map;
    delete gui;
}

void Engine::update() {
	if ( gameStatus == STARTUP ) map->computeFov();
   	gameStatus=IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&lastKey,&mouse);    player->update();
    if ( gameStatus == NEW_TURN ) {
	    for (Actor **iterator=actors.begin(); iterator != actors.end();
	        iterator++) {
	        Actor *actor=*iterator;
	        if ( actor != player ) {
	            actor->update();
	        }
	    }
	}
}

void Engine::render()
{
    TCODConsole::root -> clear();
    map -> render();
    for( Actor **iterator = actors.begin(); iterator != actors.end(); iterator++ )
    {
        Actor *actor = *iterator;
        if( map -> isInFov( actor -> x, actor -> y ) )
        {
            actor -> render();
        }
    }
    player -> render();
    gui -> render();
}

void Engine::sendToBack( Actor *actor )
{
    actors.remove( actor );
    actors.insertBefore( actor, 0 );
}

Actor *Engine::getClosestMonster( int x, int y, float range ) const
{
    Actor *closest = NULL;
    float bestDistance = 1E6f;

    for( Actor **iterator = actors.begin(); iterator != actors.end(); iterator++ )
    {
        Actor *actor = *iterator;
        if( actor != player && actor -> destructible )
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


bool Engine::pickATile(int *x, int *y, float maxRange) {
	while ( !TCODConsole::isWindowClosed() ) {
		render();
		// highlight the possible range
		for (int cx = 0; cx < map->width; cx++) {
			for (int cy=0; cy < map->height; cy++) {
				if ( map->isInFov( cx, cy )
					&& ( maxRange == 0 || player->getDistance(cx,cy) <= maxRange) ) {
                    TCODColor col = TCODConsole::root->getCharBackground(cx,cy);
					col = col * 1.2f;
					TCODConsole::root->setCharBackground( cx, cy, col);
				}
			}
		}
        TCODSystem::checkForEvent( TCOD_EVENT_ANY,&lastKey,&mouse );
        if( map->isInFov(mouse.cx,mouse.cy) && ( maxRange == 0 || player->getDistance(mouse.cx,mouse.cy) <= maxRange ) ) {
			TCODConsole::root->setCharBackground(mouse.cx,mouse.cy,TCODColor::white);
			if ( mouse.lbutton_pressed ) {
				*x=mouse.cx;
				*y=mouse.cy;
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