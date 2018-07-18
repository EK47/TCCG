#include "main.hpp"

Map::Map( int width, int height ) : width( width ), height( height )
{
    dungeonL1 = Mix_LoadMUS("../assets/music/LoneBit.mp3");
    if(Mix_PlayMusic( dungeonL1, -1) ==-1)
    {
        printf("Mix_PlayMusic: %s\n", Mix_GetError());
        // well, there's no music, but most games don't break without music...
    }
    Mix_VolumeMusic( 110 );
    tiles = new Tile[ width * height ];
    map = std::make_shared<TCODMap>( width, height );
    path = std::make_shared<TCODPath>( map.get(), 1.0f );
    TCODBsp bsp( 0, 0, width, height );
    bsp.splitRecursive( NULL, 8, ROOM_MAX_SIZE, ROOM_MIN_SIZE, 1.5f, 1.5f );
    BspListener listener( *this );
    bsp.traverseInvertedLevelOrder( &listener, NULL );
}

Map::~Map()
{
    delete [] tiles;
    Mix_HaltMusic();
    delete dungeonL1;
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

bool Map::LivingThingInFov() const
{
    for( auto &actor : engine.actors )
	{
		if( actor -> name != engine.player -> name && actor -> destructible != nullptr && actor -> destructible -> isAlive() && engine.map -> isInFov( actor -> x, actor -> y ) )
		{
            return true;
		}
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

    for( auto& actor : engine.actors )
    {
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
    if( val < 25 )
    {
        std::shared_ptr<Actor> Cockroach = std::make_shared<Actor>( x, y, 'c', "Cockroach", TCODColor::darkerSepia );
        Cockroach -> destructible = std::make_shared<NPCDestructible>( 1, 0, "Bug Stain" );
        Cockroach -> ai = std::make_shared<NPCAi>( Cockroach );
        engine.actors.push_back( Cockroach );
    } else if( val < 60 )
    {
        std::shared_ptr<Actor> orc = std::make_shared<Actor>( x, y, 'o', "Orc", TCODColor::desaturatedGreen );
        orc -> destructible = std::make_shared<MonsterDestructible>( 10, 0, "Dead Orc" );
        orc -> attacker = std::make_shared<Attacker>( 3 );
        orc -> ai = std::make_shared<MonsterAi>();
        engine.actors.push_back( orc );
    } else if( val < 80 )
    {
        std::shared_ptr<Actor> troll = std::make_shared<Actor>( x, y, 'T', "Troll", TCODColor::darkerGreen );
        troll -> destructible = std::make_shared<MonsterDestructible>( 16, 1, "Troll Carcass" );
        troll -> attacker = std::make_shared<Attacker>( 4 );
        troll -> ai = std::make_shared<MonsterAi>();
        engine.actors.push_back( troll );
    } else if( val < 90 )
    {
        std::shared_ptr<Actor> demon = std::make_shared<Actor>( x, y, 'D', "Demon", TCODColor::darkerRed );
        demon -> destructible = std::make_shared<MonsterDestructible>( 25, 2, "Scorched Corpse" );
        demon -> attacker = std::make_shared<Attacker>( 4 );
        demon -> ai = std::make_shared<MonsterAi>();
        engine.actors.push_back( demon );
    } else if( val < 100 )
    {
        std::shared_ptr<Actor> bob = std::make_shared<Actor>( x, y, 'B', "Big Bob", TCODColor::darkBlue );
        bob -> destructible = std::make_shared<MonsterDestructible>( 12, 2, "Fatty Cadaver" );
        bob -> attacker = std::make_shared<Attacker>( 6 );
        bob -> ai = std::make_shared<MonsterAi>();
        engine.actors.push_back( bob );
    }
}

void Map::addItem( int x, int y )
{
    TCODRandom *rng = TCODRandom::getInstance();
    int dice = rng -> getInt( 0, 100 );
    if( dice < 40 )
    {
        std::shared_ptr<Actor> healthPotion = std::make_shared<Actor>( x, y, 15, "slice of \'fresh\' bread", TCODColor ( 30, 175, 30 ) );
        healthPotion -> blocks = false;
        healthPotion -> pickable = std::make_shared<Healer>( 8 );
        engine.actors.push_back( healthPotion );
    } else if( dice < 50 )
    {
        std::shared_ptr<Actor> scrollOfLightningBolt = std::make_shared<Actor>( x, y, 21, "Lightning Scroll", TCODColor::lightYellow );
        scrollOfLightningBolt -> blocks = false;
        scrollOfLightningBolt -> pickable = std::make_shared<LightningBolt>( 5, 20 );
        engine.actors.push_back( scrollOfLightningBolt );
    } else if( dice < 60 )
    {
        std::shared_ptr<Actor> scrollOfFireball = std::make_shared<Actor>( x, y, 21, "Fireball Scroll", TCODColor::lightYellow );
        scrollOfFireball -> blocks = false;
        scrollOfFireball -> pickable = std::make_shared<Fireball>( 3, 10 );
        engine.actors.push_back( scrollOfFireball );
    } else if( dice < 65 )
    {
        std::shared_ptr<Actor> scrollOfConfusion = std::make_shared<Actor>( x, y, 21, "Confusion Scroll", TCODColor::lightYellow );
        scrollOfConfusion -> blocks = false;
        scrollOfConfusion -> pickable = std::make_shared<Confuser>( 10, 8 );
        engine.actors.push_back( scrollOfConfusion );
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