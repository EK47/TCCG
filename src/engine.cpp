#include "main.hpp"

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

    gui->message( TCODColor::red, "Save us, Calvin!\n You're our only hope!" );
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
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL );   
    player->update();
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