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

Map::Map( int width, int height ) : width( width ), height( height )
{
    seed = time( 0 );
    // seed = 314159;
    rng = new TCODRandom( seed );
    mapConsole = new TCODConsole( width, height );
    
    // Lighting stuff
    /*lightmask = std::make_shared<LightMask>( width, height );
    lightmask -> setIntensity( 40.0f );
    lightmask -> setAmbient( 0.0f );*/

    /*for( int i = 0; i < width * height; ++i )
    {
        walls.push_back( 1.0f );
    }*/

    tiles = new Tile[ width * height ];
    map = std::make_shared<TCODMap>( width, height );
    path = std::make_shared<TCODPath>( map.get(), 1.0f );
    //DungeonBuilder build( *this, width, height );
    //build.manager( 20 ); 
    generateDungeon();
}

Map::~Map()
{
    delete rng;
    delete mapConsole;
    delete [] tiles;
}

bool Map::generateDungeon()
{
    TCODBsp bsp( 0, 0, width, height );
    bsp.splitRecursive( rng, 6, ROOM_MIN_SIZE, ROOM_MAX_SIZE, 1.15f, 1.15f );

    RoomBuilder Roomlistener( *this, rooms );
    bsp.traverseInvertedLevelOrder( &Roomlistener, NULL );

    CorridorBuilder CorridorListener( *this, rooms );
    bsp.traverseLevelOrder( &CorridorListener, NULL );

    // Make sure the outside edges are filled 
    for( int x = 0; x <= width; x++ )
    {
        for( int y = 0; y <= height; y += height - 1 )
        {
            tiles[ x + y * width ].tileChar = '#';
            map -> setProperties( x, y, false, false );
        }
    }
    for( int y = 0; y <= height; y++ )
    {
        for( int x = 0; x <= width; x += width - 1 )
        {
            tiles[ x + y * width ].tileChar = '#';
            map -> setProperties( x, y, false, false );
        }
    }

    // Places doors at the outsides of every room

    for( int i = 0; i < rooms.size(); i++ )
    {
        int rX = rooms[i][1];
        int rY = rooms[i][2];
        int rW = rooms[i][3];
        int rH = rooms[i][4];

        for( int x = rX - 1; x < rX + rW + 2; x++ )
        {
            // I am using this first if as the example for the others
            if( !isWall( x, rY - 1 ) && ( x > 0 && x < width ) && ( rY - 1 > 0 && rY - 1 < height ) )
            {
                // Creates a door object.
                std::shared_ptr<Door> door = std::make_shared<Door>( x, rY - 1, '=', "Door", TCODColor::darkerSepia );
                door -> blocks = true;
                // Makes sure the door is at the beginning of the actors vector, so other things render over it
                engine.actors.insert( engine.actors.begin(), door );
                // Make sure the door starts off as closed in the view of the map.
                map -> setProperties( x, rY - 1, false, false );
            }
            if( !isWall( x, rY + rH ) && ( x > 0 && x < width ) && ( rY + rH > 0 && rY + rH < height ) )
            {
                std::shared_ptr<Door> door = std::make_shared<Door>( x, rY + rH, '=', "Door", TCODColor::darkerSepia );
                door -> blocks = true;
                engine.actors.insert( engine.actors.begin(), door );
                map -> setProperties( x, rY + rH, false, false );
            }
        }
        for( int y = rY - 1; y < rY + rH + 2; y++ )
        {
            if( !isWall( rX - 1, y ) && ( rX - 1 > 0 && rX - 1 < width ) && ( y > 0 && y < height ) )
            {
                std::shared_ptr<Door> door = std::make_shared<Door>( rX - 1, y, '=', "Door", TCODColor::darkerSepia );
                door -> blocks = true;
                engine.actors.insert( engine.actors.begin(), door );
                map -> setProperties( rX - 1, y, false, false );
            }
            if( !isWall( rX + rW, y ) && ( rX + rW > 0 && rX + rW < width ) && ( y > 0 && y < height ) )
            {
                std::shared_ptr<Door> door = std::make_shared<Door>( rX + rW, y, '=', "Door", TCODColor::darkerSepia );
                door -> blocks = true;
                engine.actors.insert( engine.actors.begin(), door );
                map -> setProperties( rX + rW, y, false, false );
            }
        }
    }
}

bool Map::isWall( int x, int y ) const 
{
   return !( map -> isWalkable( x, y ) && map -> isTransparent( x, y ) );
}

bool Map::isInFov( int x, int y ) const
{
   if( x < 0 || x >= width || y < 0 || y >= height )
   {
       return false;
   }
   if ( map -> isInFov( x, y ) )
   {
       tiles[ x + y * width ].explored = true;
       return true;
   }
   return false;
}

bool Map::EnemyInFov() const
{
    for( auto &actor : engine.actors )
	{
		if( actor -> name != engine.player -> name && actor -> destructible != nullptr && actor -> destructible -> isAlive() && engine.map -> isInFov( actor -> x, actor -> y ) && dynamic_cast<MonsterAi*> ( actor -> ai.get() ) != nullptr )
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
    int val = rng -> getInt( 0, 100 );
    if( val < 25 )
    {
        std::shared_ptr<Actor> Cockroach = std::make_shared<Actor>( x, y, 'c', "Cockroach", TCODColor::darkerSepia );
        Cockroach -> destructible = std::make_shared<NPCDestructible>( 1, 0, "Bug Stain", corpse, 1 );
        Cockroach -> ai = std::make_shared<NPCAi>( Cockroach );
        engine.actors.push_back( Cockroach );
    } else if( val < 60 )
    {
        std::shared_ptr<Actor> orc = std::make_shared<Actor>( x, y, 'o', "Orc", TCODColor::desaturatedGreen );
        orc -> destructible = std::make_shared<MonsterDestructible>( 10, 0, "Dead Orc", corpse, 1 );
        orc -> attacker = std::make_shared<Attacker>( 3 );
        orc -> ai = std::make_shared<MonsterAi>();
        engine.actors.push_back( orc );
    } else if( val < 80 )
    {
        std::shared_ptr<Actor> troll = std::make_shared<Actor>( x, y, 'T', "Troll", TCODColor::darkerGreen );
        troll -> destructible = std::make_shared<MonsterDestructible>( 16, 1, "Troll Carcass", corpse, 1 );
        troll -> attacker = std::make_shared<Attacker>( 4 );
        troll -> ai = std::make_shared<MonsterAi>();
        engine.actors.push_back( troll );
    } else if( val < 90 )
    {
        std::shared_ptr<Actor> demon = std::make_shared<Actor>( x, y, 'D', "Demon", TCODColor::darkerRed );
        demon -> destructible = std::make_shared<MonsterDestructible>( 25, 2, "Scorched Corpse", corpse, 1 );
        demon -> attacker = std::make_shared<Attacker>( 4 );
        demon -> ai = std::make_shared<MonsterAi>();
        engine.actors.push_back( demon );
    } else if( val < 100 )
    {
        std::shared_ptr<Actor> bob = std::make_shared<Actor>( x, y, 'B', "Big Bob", TCODColor::darkBlue );
        bob -> destructible = std::make_shared<MonsterDestructible>( 12, 2, "Fatty Cadaver", corpse, 1 );
        bob -> attacker = std::make_shared<Attacker>( 6 );
        bob -> ai = std::make_shared<MonsterAi>();
        engine.actors.push_back( bob );
    }
}

void Map::addItem( int x, int y )
{
    int dice = rng -> getInt( 0, 100 );
    /*if( dice < 10 )
    { 
        std::shared_ptr<Actor> lamp = std::make_shared<Actor>( x, y, 20, "lamp", TCODColor::yellow );
        lamp -> blocks = false;
        lamp -> pickable = std::make_shared<Lamp>( 0.4f );
        engine.actors.push_back( lamp );
    } else*/if( dice < 40 )
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

void Map::addObject( int x1, int y1, int x2, int y2 )
{
    // Adds a light in opposite corners of every room
    /*int lights = rng -> getInt( 1, 3 );
    for( int l = 0; l < lights; ++l )
    {
        // Adds a light in a random spot in the room.
        tiles[ rng -> getInt( x1, x2 ) + width * rng -> getInt( y1, y2 ) ].tileChar = 156;
    }*/
}

void Map::computeFov()
{
   map -> computeFov( engine.player -> x, engine.player -> y, std::max( engine.currentFOV, 2 ), true, FOV_RESTRICTIVE );
}

void Map::getPointInCircle( int radius, int *x, int *y ) const
{
    // This gets a random point in a circle, and returns the cartesian coordinates for said point.
    float randFloat = ((float) rand()) / (float) RAND_MAX;
    float r = radius * sqrt( randFloat );
    float theta = randFloat * 2 * M_PI;
    *x = (int)floor( r * cos( theta ) );
    *y = (int)floor( r * sin( theta ) );
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
            //walls[ tilex + width * tiley ] = 0.0f;
            tiles[ tilex + width * tiley ].tileChar = 250;
        }
    }
}

void Map::createRoom( bool first, int x1, int y1, int x2, int y2 )
{
    dig( x1, y1, x2, y2 );

    addObject( x1, y1, x2, y2 );

    if( first )
    {
        engine.player -> x = ( x1 + x2 ) / 2;
        engine.player -> y = ( y1 + y2 ) / 2;
        std::cout << x1 << ", " << y1 << "\n" << abs( x2 - x1 ) << ", " << abs( y2 - y1 ) << std::endl;
    } else
    {
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
                //walls[ x + width * y ] = 0.3f;
            }
            nbItems--;
        }
    }
}

void Map::render() const
{   
    if( engine.gameStatus == engine.GameStatus::NEW_TURN )
    {
        //lightmask -> reset();
    }

    for( int x = 0; x < width; x++ )
    {
        for( int y = 0; y < height; y++ )
        {
            // Character Setting

            mapConsole -> setChar( x, y, tiles[ x + width * y ].tileChar );
            
            // Color setting

            if( isInFov( x, y ) )
            {
                switch( tiles[ x + width * y ].tileChar )
                {
                    case (int)'#':
                        mapConsole -> setCharForeground( x, y, dungeon1Wall );
                    break;
                    case 156:
                        mapConsole -> setCharForeground( x, y, TCODColor::yellow );
                    break;
                    case 250:
                        mapConsole -> setCharForeground( x, y, dungeon1Floor );
                    break;
                    default: break;
                } 

                mapConsole -> setCharBackground( x, y, dungeon1Background );
                
            } else if( isExplored( x, y ) )
            {
                switch( tiles[ x + width * y ].tileChar )
                {
                    case (int)'#':
                        mapConsole -> setCharForeground( x, y, returnUnseen( dungeon1Wall ) );
                    break;
                    case 156:
                        mapConsole -> setCharForeground( x, y, returnUnseen( TCODColor::yellow ) );
                    break;
                    case 250:
                        mapConsole -> setCharForeground( x, y, returnUnseen ( dungeon1Floor ) );
                    break;
                    default: break;
                } 

                mapConsole -> setCharBackground( x, y, returnUnseen( dungeon1Background ) );

            }
            if( engine.gameStatus == engine.GameStatus::NEW_TURN )
            {
                if( tiles[ x + width * y ].tileChar == 156 /*&& 
                ( ( x > engine.camera -> topLeftX - 1.5 * engine.camera -> cameraWidth && x < engine.camera -> topLeftX + 1.5 * engine.camera -> cameraWidth )
                && ( y > engine.camera -> topLeftY - 1.5 * engine.camera -> cameraHeight && x < engine.camera -> topLeftY + 1.5 * engine.camera -> cameraHeight ) )*/ ) // Render light if in the camera view.
                {
                    //lightmask -> addLight( x, y, 1.0f );
                }
            }
        }
    }
	if( engine.gameStatus == engine.GameStatus::NEW_TURN )
    {
        /*for( auto &actor : engine.player -> container -> inventory )
	    {
	        if( actor -> pickable -> state() && actor -> name == "lamp" )
	        {
	        	lightmask -> addLight( engine.player -> x, engine.player -> y, 1.0f );
	        }
	    }*/
    }
    // Computation Function used in Camera.cpp

}


// Old Render Code
/*
    for( int x = 0; x < width; x++ )
    {
        for( int y = 0; y < height; y++ )
        {
            if ( isInFov(x,y) )
            {
                TCODConsole::root->setCharForeground( x, y, isWall(x,y) ? dungeon1Wall : dungeon1Floor );
                TCODConsole::root->setCharBackground( x, y, dungeon1Background );
                if( isWall( x, y ) )
                {
                    TCODConsole::root -> setChar( x, y, '#' );
                } else
                {
                    TCODConsole::root -> setChar( x, y, 250 );
                }

            } else if ( isExplored(x, y) )
            {
                TCODConsole::root->setCharForeground(x, y, isWall(x,y) ? returnUnseen( dungeon1Wall ) : returnUnseen( dungeon1Floor ) );
                TCODConsole::root->setCharBackground( x, y, returnUnseen( dungeon1Background ) );
                
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
*/
/*
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
    TCODRandom *rng = TCODRandom::getInstance();
    dig( x1, y1, x2, y2 );
    if( first )
    {
        engine.player -> x = ( x1 + x2 ) / 2;
        engine.player -> y = ( y1 + y2 ) / 2;
    } else
    {
        int nbMonsters = rng -> getInt( 0, MAX_ROOM_MONSTERS );
        while( nbMonsters > 0 )
        {
            std::uniform_int_distribution<> xdistr( x1, x2 );
            std::uniform_int_distribution<> ydistr( y1, y2 );
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
*/