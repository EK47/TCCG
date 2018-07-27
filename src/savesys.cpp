#include "main.hpp"

void Engine::adventureInit()
{
    // Creates the player
    player = std::make_shared<Actor>( 1, 1, '@', "Calvin", calvin1 );
    player -> destructible = std::make_shared<PlayerDestructible>( 30, 2, "Your Corpse" );
    player -> attacker = std::make_shared<Attacker>( 5 );
    player -> ai = std::make_shared<PlayerAi>();
    player -> container = std::make_shared<Container>( 10 );
    actors.push_back( player );
    map = std::make_shared<Map>( 100, 100 );
    camera = std::make_shared<Camera>( (int)( 3 * screenWidth ) / 4, (int)( 3 * screenHeight ) / 4, true );
    gui = new Gui();
}

void Engine::restartAdventure()
{
    actors.clear();
    map.reset();
    camera.reset();
    delete gui;
    
    adventureInit();
}

void Engine::load()
{
    
}

void Engine::save()
{

}