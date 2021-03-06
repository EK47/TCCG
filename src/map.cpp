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
    if( val < 60 )
    {
        Actor *orc = new Actor( x, y, 'o', "Orc", TCODColor::desaturatedGreen );
        orc -> destructible = new MonsterDestructible( 10, 0, "Dead Orc" );
        orc -> attacker = new Attacker( 3 );
        orc -> ai = new MonsterAi();
        engine.actors.push( orc );
    } else if( val < 80 )
    {
        Actor *troll = new Actor( x, y, 'T', "Troll", TCODColor::darkerGreen );
        troll -> destructible = new MonsterDestructible( 16, 1, "Troll Carcass" );
        troll -> attacker = new Attacker( 4 );
        troll -> ai = new MonsterAi();
        engine.actors.push( troll );
    } else if( val < 90 )
    {
        Actor *demon = new Actor( x, y, 'D', "Demon", TCODColor::darkerRed );
        demon -> destructible = new MonsterDestructible( 25, 2, "Scorched Corpse" );
        demon -> attacker = new Attacker( 4 );
        demon -> ai = new MonsterAi();
        engine.actors.push( demon );
    } else if( val < 100 )
    {
        Actor *bob = new Actor( x, y, 'B', "Big Bob", TCODColor::darkBlue );
        bob -> destructible = new MonsterDestructible( 12, 2, "Fatty Cadaver" );
        bob -> attacker = new Attacker( 6 );
        bob -> ai = new MonsterAi();
        engine.actors.push( bob );
    }
}

void Map::addItem( int x, int y )
{
    TCODRandom *rng = TCODRandom::getInstance();
    int dice = rng -> getInt( 0, 100 );
    if( dice < 40 )
    {
        Actor *healthPotion = new Actor( x, y, 15, "slice of \'fresh\' bread", TCODColor ( 30, 175, 30 ) );
        healthPotion -> blocks = false;
        healthPotion -> pickable = new Healer( 8 );
        engine.actors.push( healthPotion );
    } else if( dice < 50 )
    {
        Actor *scrollOfLightningBolt = new Actor( x, y, 21, "Lightning Scroll", TCODColor::lightYellow );
        scrollOfLightningBolt -> blocks = false;
        scrollOfLightningBolt -> pickable = new LightningBolt( 5, 20 );
        engine.actors.push( scrollOfLightningBolt );
    } else if( dice < 60 )
    {
        Actor *scrollOfFireball = new Actor( x, y, 21, "Fireball Scroll", TCODColor::lightYellow );
        scrollOfFireball -> blocks = false;
        scrollOfFireball -> pickable = new Fireball( 3, 10 );
        engine.actors.push( scrollOfFireball );
    } else if( dice < 65 )
    {
        Actor *scrollOfConfusion = new Actor( x, y, 21, "Confusion Scroll", TCODColor::lightYellow );
        scrollOfConfusion -> blocks = false;
        scrollOfConfusion -> pickable = new Confuser( 10, 8 );
        engine.actors.push( scrollOfConfusion );
    }
}

void Map::computeFov() {        // No matter what I do, this has to be a magic number
   map -> computeFov( engine.player -> x, engine.player -> y, 10, true, FOV_SHADOW );
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
    static const TCODColor visibleWall( 50, 140, 50 );
    static const TCODColor visibleGround( 60, 60, 60 );
    static const TCODColor unseenWall = visibleWall * 0.5f;
    static const TCODColor unseenGround = visibleGround * 0.5f;


    for( int x = 0; x < width; x++ )
    {
        for( int y = 0; y < height; y++ )
        {
            if ( isInFov(x,y) ) {
                TCODConsole::root->setCharForeground( x, y, isWall(x,y) ? visibleWall : visibleGround );
                TCODConsole::root->setCharBackground( x, y, TCODColor( 20, 20, 20 ) );
                if( isWall( x, y ) )
                {
                    TCODConsole::root -> setChar( x, y, '#' );
                } else
                {
                    TCODConsole::root -> setChar( x, y, 250 );
                }

            } else 
            {
                if ( isExplored(x, y) ) {
                    TCODConsole::root->setCharForeground(x, y, isWall(x,y) ? unseenWall : unseenGround );
                    TCODConsole::root->setCharBackground( x, y, TCODColor( 10, 10, 10 ) );

                    if( isWall( x, y ) )
                    {
                        TCODConsole::root -> setChar( x, y, '#' );
                    } else
                    {
                        TCODConsole::root -> setChar( x, y, 250 );
                    }
                }
            } 
        }
    }
}