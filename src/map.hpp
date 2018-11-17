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

#ifndef map_hpp
#define map_hpp
#pragma once

static const int ROOM_MAX_SIZE = 15;
static const int ROOM_MIN_SIZE = 7;
static const int MAX_ROOM_MONSTERS = 5;
static const int MAX_ROOM_ITEMS = 2;

struct Tile 
{
   bool explored; // has the player already seen this tile ?
   int tileChar = (char)'#';
   Tile() : explored( false ) { }
};

class Map
{
public:
    int width, height;
    unsigned long seed;
    TCODRandom *rng;
    std::shared_ptr<TCODPath> path;
    std::shared_ptr<TCODMap> map;
    TCODConsole *mapConsole;
    //std::shared_ptr<LightMask> lightmask;
    std::vector<float> walls;
    std::vector< std::vector<int> > rooms;
    Map( int width, int height );
    ~Map();
    bool isWall( int x, int y ) const;
    bool isInFov( int x, int y ) const;
    bool EnemyInFov() const;
    bool isExplored( int x, int y ) const;
    bool canWalk( int x, int y ) const;
    void getPointInCircle( int radius, int *x, int *y ) const;
    void computeFov();
    void render() const;

    bool generateDungeon();
    void dig( int x1, int y1, int x2, int y2 );
    void createRoom( bool first, int x1, int y1, int x2, int y2 );
    void addMonster( int x, int y );
    void addItem( int x, int y );
    void addObject( int x1, int y1, int x2, int y2 );
    Tile *tiles;
protected:
    friend class RoomBuilder;
    friend class CorridorBuilder;
};

class CorridorBuilder : public ITCODBspCallback
{
private:
    Map &map;
    std::vector< std::vector<int> > &rooms;

public:
    CorridorBuilder( Map &map, std::vector< std::vector<int> > &rooms ) : map( map ), rooms( rooms ) {}

    bool visitNode( TCODBsp *node, void *userData )
    {
        if( !node -> isLeaf() )
        {
            TCODBsp *leftSon = node -> getLeft();
            TCODBsp *rightSon = node -> getRight();

            int leftSonNum = 0;
            int rightSonNum = 0;

            if( !( leftSon && rightSon ) )
            {
                return true;
            }

            for( int i = 0; i < rooms.size(); i++ )
            {
                if( leftSon -> contains( rooms[i][1] + rooms[i][3] / 2, rooms[i][2] + rooms[i][4] / 2 ) )
                {
                    leftSonNum = i;
                }
            }

            for( int i = 0; i < rooms.size(); i++ )
            {
                if( rightSon -> contains( rooms[i][1] + rooms[i][3] / 2, rooms[i][2] + rooms[i][4] / 2 ) )
                {
                    rightSonNum = i;
                }
            }

            if( leftSonNum == rightSonNum )
            {
                return true;
            }

            // These numbers are for corridor placement calculations.
            int lX = rooms[leftSonNum][1];
            int lY = rooms[leftSonNum][2];
            int lW = rooms[leftSonNum][3] - 1;
            int lH = rooms[leftSonNum][4] - 1;

            int rX = rooms[rightSonNum][1];
            int rY = rooms[rightSonNum][2];
            int rW = rooms[rightSonNum][3] - 1;
            int rH = rooms[rightSonNum][4] - 1;

            // Rooms cannot share both an x and y value since all room are rectangles, so the only special case is when a room shares
            // no X or Y values, in which case you have to do a corner turn corridor.

            // If the rooms share an X value, make a corridor from one room to another.
            if( lX >= rX && lX <= rX + rW )
            {
                map.dig( lX, std::abs( lY - rY ) < std::abs( lY - rY + rH ) ? rY : rY + rH, lX, lY );
            // If the rooms share a Y value, make a corridor from one room to another.
            } else if( lY >= rY && lY <= rY + rH )
            {
                map.dig( std::abs( lX - rX ) < std::abs( lX - rX + rW ) ? rX : rX + rW, lY, lX, lY );
            } else
            {
                // This digs from the center of the offset room to the upwards to a y height of the center of the target room. It then makes another
                // corridor to the middle of the target room, making a corner room.
                map.dig( lX + lW / 2, lY + lH / 2, lX + lW / 2, rY + rH / 2 );
                map.dig( lX + lW / 2, rY + rH / 2, rX + rW / 2, rY + rH / 2 );
            }
        }
        return true;
    }
};

class RoomBuilder : public ITCODBspCallback
{
private:
    Map &map;
    int roomNum = 0;
    int lastx, lasty;
    std::vector< std::vector< int > > &rooms;

public:
    RoomBuilder( Map &map, std::vector< std::vector< int > > &rooms ) : map( map ), rooms( rooms ) {}

    bool visitNode( TCODBsp *node, void *userData )
    {
        if( node->isLeaf() )
        {
            int x, y, w, h;
            // dig a room
            w = map.rng -> getInt( ROOM_MIN_SIZE, node -> w - 3 );
            h = map.rng -> getInt( ROOM_MIN_SIZE, node -> h - 3 );
            x = map.rng -> getInt( node -> x + 1, node -> x + node -> w - w - 1 );
            y = map.rng -> getInt( node -> y + 1, node -> y + node -> h - h - 1 );
            map.createRoom( roomNum == 0, x, y, x + w - 1, y + h - 1 );

            std::vector< int > room;

            room.push_back( roomNum );
            room.push_back( x );
            room.push_back( y );
            room.push_back( w );
            room.push_back( h );

            rooms.push_back( room );

            /*if ( roomNum != 0 )
            {
                // Dig a corridor from last room
                map.( lastx, lasty, x + w / 2, lasty, true );
                map.( x + w / 2, lasty, x + w / 2, y + h / 2, true );
            }

            lastx = ( (bool)map.rng -> getFloat( 0.0, 1.0, 0.4 ) ) ? x + w / 2 : x;
            lasty = ( (bool)map.rng -> getFloat( 0.0, 1.0, 0.4 ) ) ? y + h / 2 : y;*/
            roomNum++;
        }
        return true;
    }
};

#endif