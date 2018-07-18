#ifndef map_hpp
#define map_hpp

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;
static const int MAX_ROOM_MONSTERS = 5;
static const int MAX_ROOM_ITEMS = 2;

struct Tile 
{
   bool explored; // has the player already seen this tile ?
   Tile() : explored(false) {}
};

class Map
{
public:
    int width, height;
    std::shared_ptr<TCODPath> path;
    std::shared_ptr<TCODMap> map;
    Map( int width, int height );
    ~Map();
    bool isWall( int x, int y ) const;
    bool isInFov( int x, int y ) const;
    bool LivingThingInFov() const;
    bool isExplored( int x, int y ) const;
    bool canWalk( int x, int y ) const;
    void addMonster( int x, int y );
    void addItem( int x, int y );
    void computeFov();
    void render() const;
    Mix_Music *dungeonL1;
protected:
    Tile *tiles;
    friend class BspListener;
    void dig( int x1, int y1, int x2, int y2 );
    void createRoom( bool first, int x1, int y1, int x2, int y2 );
};

/*
class PathListener : public ITCODPathCallback {
public:
    float getWalkCost(int xFrom, int yFrom, int xTo, int yTo, void *userData ) const
    {
        
    }
};
*/

class BspListener : public ITCODBspCallback
{
private:
    Map &map;
    int roomNum = 0;
    int lastx, lasty;

public:
    BspListener( Map &map ) : map( map ) {}

    bool visitNode( TCODBsp *node, void *userData )
    {
        if( node->isLeaf() )
        {
            int x,y,w,h;
            // dig a room
            TCODRandom *rng=TCODRandom::getInstance();
            w=rng->getInt(ROOM_MIN_SIZE, node->w-2);
            h=rng->getInt(ROOM_MIN_SIZE, node->h-2);
            x=rng->getInt(node->x+1, node->x+node->w-w-1);
            y=rng->getInt(node->y+1, node->y+node->h-h-1);
            map.createRoom(roomNum == 0, x, y, x+w-1, y+h-1);

            if ( roomNum != 0 ) {
                // dig a corridor from last room
                map.dig(lastx,lasty,x+w/2,lasty);
                map.dig(x+w/2,lasty,x+w/2,y+h/2);
            }

            lastx = x + w / 2;
            lasty = y + h / 2;
            roomNum++;
        }
        return true;
    }
};

#endif