#ifndef astar_hpp
#define astar_hpp
/*
class Node 
{
public:
    enum NODETYPE
    {
        START_NODE,
        DEST_NODE,
        NORM_NODE
    };
    int x, y;
    int weight;

private:
    bool walkable;


};
*/

// Directions relative to a middle node. Goes clockwise starting from the Northern node.

static std::vector<int> xDirections { 0, 1, 1, 1, 0, -1, -1, -1 };
static std::vector<int> yDirections { 1, 1, 0, -1, -1, -1, 0, 1 };

class aStarGenPath
{
public:
    void origin( int *x, int *y );
    void destination( int *x, int *y );
    void get( int index, int *x, int *y );
    bool walk( int *x, int *y, bool recalculateWhenNeeded );
    bool calculate( int oX, int oY, int dX, int dY );
    int size();
    aStarGenPath( std::shared_ptr<TCODMap> map, float cardinalDistance, float diagonalDistance );
    ~aStarGenPath();
    std::vector<int> originDestination;
    std::deque<int> totalPath;
    std::shared_ptr<TCODMap> internalMap;
private:
    std::vector<int> closedSet;
    std::vector<int> openSet;
    std::unordered_map<int, int> cameFrom;
    std::map<int, int> gScore;
    std::map<int, int> fScore;
    int heuristic_estimate( int oX, int oY, int dX, int dY );
    float diagonalDistance { 1 };
    float cardinalDistance { 1 };
};
#endif