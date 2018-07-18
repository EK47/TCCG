#include "main.hpp"
#include <unistd.h>

static const bool FULLSCREEN = false;

Engine::Engine( int screenWidth, int screenHeight ) : gameStatus( STARTUP ), screenWidth( screenWidth ), screenHeight( screenHeight )
{
    musicStart();
    // Limits FPS as to protect CPU usage
    TCODSystem::setFps( 60 );
    TCODConsole::initRoot( screenWidth, screenHeight, "The Calvin Chronicles Game", FULLSCREEN );
    TCODConsole::setCustomFont("terminal16x16_gs_ro.png", TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW );
    // Creates the player
    player = std::make_shared<Actor>( 1, 1, '@', "Calvin", TCODColor( 250, 240, 190 ) );
    player -> destructible = std::make_shared<PlayerDestructible>( 30, 2, "Your Corpse" );
    player -> attacker = std::make_shared<Attacker>( 5 );
    player -> ai = std::make_shared<PlayerAi>();
    player -> container = std::make_shared<Container>( 10 );
    actors.push_back( player );
    map = std::make_unique<Map>( (3 * screenWidth ) / 4, ( 3 * screenHeight ) / 4 );
    gui = new Gui();

    gui->message( TCODColor::red, "Venture into the depths, Calvin," );
    gui->message( TCODColor::darkerRed, "and feed us your soul." );
}

Engine::~Engine()
{
    actors.clear();
    Mix_CloseAudio();
    delete gui;
}

void Engine::musicStart()
{
    // Load SDL sound engine
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024 ) == -1 )
    {
        printf("%s", Mix_GetError() );
    }
}

void Engine::update() {
	if ( gameStatus == STARTUP ) map->computeFov();
   	gameStatus=IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&lastKey,&mouse);
    player->update( player );
    if ( gameStatus == NEW_TURN ) {
	    for ( auto &actor : actors ) {
	        if ( actor->name != "Calvin" && actor->destructible ) {
	            if( !actor->destructible->isDead() )
                {
                    actor->update( actor );
                }
	        }
	    }
	}
}

void Engine::render()
{
    TCODConsole::root -> clear();
    map -> render();
    for( auto &actor : actors )
    {
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
    player -> render();
    gui -> render();
}

std::shared_ptr<Actor> Engine::getClosestMonster( int x, int y, float range ) const
{
    std::shared_ptr<Actor> closest = NULL;
    float bestDistance = 1E6f;

    for( auto &actor : actors )
    {
        if( actor != player && actor -> destructible && ! actor -> destructible -> isDead() )
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
        if( actor -> x == x && actor -> y == y && actor -> destructible && !actor -> destructible -> isDead() )
        {
            return actor;
        }
    }
    return NULL;
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
					col = col * 1.75f;
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