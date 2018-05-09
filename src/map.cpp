#include <algorithm>
#include "main.hpp"

Map::Map( int width, int height ) : width( width ), height( height )
{
    tiles = new Tile[ width * height ];
    map = new TCODMap( width, height );
    TCODBsp bsp( 0, 0, width, height );
    bsp.splitRecursive( NULL, 8, ROOM_MAX_SIZE, ROOM_MIN_SIZE, 1.5f, 1.5f );
    BspListener listener( *this );
    bsp.traverseInvertedLevelOrder( &listener, NULL );
}

Map::~Map()
{
    delete [] tiles;
    delete map;
}

bool Map::isWall(int x, int y) const 
{
   return !map->isWalkable(x,y);
}
bool Map::isInFov(int x, int y) const {
   if( x < 0 || x >= width || y < 0 || y >= height )
   {
       return false;
   }
   if ( map->isInFov(x,y) ) {
       tiles[x+y*width].explored=true;
       return true;
   }
   return false;
}

bool Map::isExplored( int x, int y ) const
{
    return tiles[ x + y * width ].explored;
}

bool Map::canWalk( int x, int y ) const 
{
    if( isWall( x, y ) )
    {
        return false;
    }

    for( Actor **iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++ )
    {
        Actor *actor = *iterator;
        if( actor -> blocks && actor -> x == x && actor -> y == y )
        {
            return false;
        }
    }
    return true;
}

void Map::addMonster( int x, int y )
{
    TCODRandom *rng = TCODRandom::getInstance();
    int val = rng -> getInt( 0, 100 );
    if( val < 80 && val > 20 )
    {
        Actor *orc = new Actor( x, y, 'o', "Orc", TCODColor::desaturatedGreen );
        orc -> destructible = new MonsterDestructible( 10, 0, "Dead Orc" );
        orc -> attacker = new Attacker( 3 );
        orc -> ai = new MonsterAi();
        engine.actors.push( orc );
    } else if( val <= 20 && val > 1 )
    {
        Actor *troll = new Actor( x, y, 'T', "Troll", TCODColor::darkerGreen );
        troll -> destructible = new MonsterDestructible( 16, 1, "Troll Carcass" );
        troll -> attacker = new Attacker( 4 );
        troll -> ai = new MonsterAi();
        engine.actors.push( troll );
    } else if( val = 1 )
    {
        Actor *demon = new Actor( x, y, 'D', "Demon", TCODColor::darkerRed );
        demon -> destructible = new MonsterDestructible( 30, 2, "Scorched Flesh" );
        demon -> attacker = new Attacker( 6 );
        demon -> ai = new MonsterAi();
        engine.actors.push( demon );
    }
}

void Map::addItem( int x, int y )
{
    Actor *healthPotion = new Actor( x, y, 15, "\'fresh\' bread", TCODColor ( 30, 175, 30 ) );
    healthPotion -> blocks = false;
    healthPotion -> pickable = new Healer( 8 );
    engine.actors.push( healthPotion );
}

void Map::computeFov() {        // No matter what I do, this has to be a magic number
   map -> computeFov( engine.player -> x, engine.player -> y, 10, true, FOV_BASIC );
}


void Map::dig( int x1, int y1, int x2, int y2 )
{
    if( x2 < x1 )
    {
        std::swap( x2, x1 );
    }
    if( y2 < y1 )
    {
        std::swap( y2, y1 );
    }

    for( int tilex = x1; tilex <= x2; tilex++ )
    {
        for( int tiley = y1; tiley <= y2; tiley++ )
        {
            map -> setProperties( tilex, tiley, true, true );
        }
    }
}

void Map::createRoom( bool first, int x1, int y1, int x2, int y2 )
{
    dig( x1, y1, x2, y2 );
    if( first )
    {
        engine.player -> x = ( x1 + x2 ) / 2;
        engine.player -> y = ( y1 + y2 ) / 2;
    } else
    {
        TCODRandom *rng=TCODRandom::getInstance();
        int nbMonsters = rng -> getInt( 0, MAX_ROOM_MONSTERS );
        while( nbMonsters > 0 )
        {
            int x = rng -> getInt( x1, x2 );
            int y = rng -> getInt( y1, y2 );
            if( canWalk( x, y ) )
            {
                addMonster( x, y );
            }
            nbMonsters--;
        }

        int nbItems = rng -> getInt( 0, MAX_ROOM_ITEMS );
        while( nbItems > 0 )
        {
            int x = rng -> getInt( x1, x2 );
            int y = rng -> getInt( y1, y2 );
            if( canWalk( x, y ) )
            {
                addItem( x, y );
            }
            nbItems--;
        }
    }
}

void Map::render() const
{
    static const TCODColor darkWall( 50, 50, 50 );
    static const TCODColor darkGround( 80, 80, 80 );
    static const TCODColor lightWall( 130, 110, 50 );
    static const TCODColor lightGround( 200, 180, 50 );


    for( int x = 0; x < width; x++ )
    {
        for( int y = 0; y < height; y++ )
        {
            if ( isInFov(x,y) ) {
            TCODConsole::root->setCharBackground(x, y, isWall(x,y) ? lightWall :lightGround );
            } else 
            {
                if ( isExplored(x, y) ) {
                    TCODConsole::root->setCharBackground(x, y, isWall(x,y) ? darkWall : darkGround );
                }
            }
        }
    }
}