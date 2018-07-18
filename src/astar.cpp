#include "main.hpp"

// TCODMap can be accessed: Is a struct!
// Use X, Y, and whether or not a space is passable to advantage.
// Location of actors, terrain condition, etc. is important.
// Std::vector<std::vector<int>> to store the current path.
// This is set to completely replace the path thing... RIP me

aStarGenPath::aStarGenPath( std::shared_ptr<TCODMap> map, float cardinalDistance, float diagonalDistance ) : internalMap( map ), cardinalDistance( cardinalDistance ), diagonalDistance( diagonalDistance )
{

}

aStarGenPath::~aStarGenPath()
{
    
}

int coordToNode( int x, int y )
{
    return ( y * engine.map -> width ) + x;
}

void nodeToCoord( int node, int *x, int *y )
{
    *x = node % engine.map -> width;
    *y = ( node - *x ) / engine.map -> width;
}

template <class T>
void coordToNode( T *storage, int x, int y )
{
    (*storage).push_back( ( y * engine.map -> width ) + x );
}

template <class T>
void nodeToCoord( T storage, int index, int *x, int *y )
{
    *x = storage[index] % engine.map -> width;
    *y = ( storage[index] - *x ) / engine.map -> width;
}

int aStarGenPath::heuristic_estimate( int oX, int oY, int dX, int dY )
{
    int x = abs( oX - dX );
    int y = abs( oY - dY );
    // Calculates heuristic from current node to end.
    return 1.001 * ( cardinalDistance * ( x + y ) + ( diagonalDistance - 2 * cardinalDistance ) * std::min( x, y ) );
}

bool aStarGenPath::calculate( int oX, int oY, int dX, int dY )
{
    // Clear out all previous data from previous calculations
    originDestination.clear();
    totalPath.clear();
    cameFrom.clear();
    closedSet.clear();
    openSet.clear();
    gScore.clear();
    fScore.clear();
    // Add coordinates of start and end to the originDestination vector
    originDestination.push_back( oX );
    originDestination.push_back( oY );
    originDestination.push_back( dX );
    originDestination.push_back( dY );

    // Put the start node into the openSet
    openSet.push_back( coordToNode( originDestination[0], originDestination[1] ) );
    // Set the gScore ( cost of getting from start to current node ) of the start to 0
    gScore[ coordToNode( originDestination[0], originDestination[1] ) ] = 0;
    // Set the fScore ( cost of getting from start to finish by travelling through the current nodes ) to Diagonal Distance directly from start to finish.
    fScore[ coordToNode( originDestination[0], originDestination[1] ) ] = heuristic_estimate( originDestination[0], originDestination[1], originDestination[2], originDestination[3] );

    while( !(openSet.empty()) )
    {
        // Find the node with the smallest fScore
        int current { 10000000 };
        for( auto &i : openSet )
        {
            if( fScore[i] < current )
            {
                current = i;
            }
        }
        // Check to see if the current node is the destination
        std::ofstream out("nodeCheck.txt");
        out << current << " : " << coordToNode( originDestination[2], originDestination[3] ) << std::endl;
        if( current == coordToNode( originDestination[2], originDestination[3] ) )
        {
            // Constructs the final path
            for( auto &myPair : cameFrom )
            {
                if( current == myPair.first )
                {
                    current = cameFrom[current];
                    totalPath.push_back( current );
                }
            }
            std::cout<<"Early Exit\n";
            totalPath.push_back( current );
            out.close();
            return true;
        }

        // Removes current from the open set and adds it to the closed set
        openSet.erase( std::remove( openSet.begin(), openSet.end(), current ), openSet.end() );
        closedSet.push_back( current );
        // Generally, 8 tiles around a node
        for( int i = 0; i < 8; i++ )
        {
            // Convert the current node to coords.
            int currentX, currentY;
            nodeToCoord( current, &currentX, &currentY );
            // If the neighboring node is already in the closed set, skip.
            if( std::find( closedSet.begin(),closedSet.end(), coordToNode( currentX + xDirections[i], currentY + yDirections[i] ) ) != closedSet.end() )
            {
                continue;
            }

            // In the loop, go through the current node's neighbor going clockwise starting at NORTH.
            if( engine.map -> canWalk( currentX + xDirections[i], currentY + yDirections[i] ) )
            {
                int tentative_gScore = gScore[current] + ( i % 2 ? cardinalDistance : diagonalDistance );

                if( !(std::find( openSet.begin(), openSet.end(), coordToNode( currentX + xDirections[i], currentY + yDirections[i] ) ) != openSet.end() ) )
                {
                    openSet.push_back( coordToNode( currentX + xDirections[i], currentY + yDirections[i] ) );
                } else if( tentative_gScore >= gScore[ coordToNode( currentX + xDirections[i], currentY + yDirections[i] ) ] )
                {
                    continue;
                }
            
                // The neighbor is the best node yet. Record it.
                cameFrom[ coordToNode( currentX + xDirections[i], currentY + yDirections[i] ) ] = current;
                gScore[ coordToNode( currentX + xDirections[i], currentY + yDirections[i] ) ] = tentative_gScore;
                fScore[ coordToNode( currentX + xDirections[i], currentY + yDirections[i] ) ] = gScore[ coordToNode( currentX + xDirections[i], currentY + yDirections[i] ) ] + heuristic_estimate( currentX + xDirections[i], currentY + yDirections[i], originDestination[2], originDestination[3] );
            
            }
        }
        
    }
}

void aStarGenPath::origin( int *x, int *y )
{
    *x = originDestination[0];
    *y = originDestination[1];
}

void aStarGenPath::destination( int *x, int *y )
{
    *x = originDestination[2];
    *y = originDestination[3];
}

bool aStarGenPath::walk( int *x, int *y, bool recalculateWhenNeeded )
{
    if( totalPath.empty() )
    {
        openSet.clear();
        closedSet.clear();
        originDestination.clear();
        cameFrom.clear();
        gScore.clear();
        fScore.clear();
        return false;
    } else 
    {
        if( recalculateWhenNeeded )
        {
            if( !(engine.map -> canWalk( totalPath[1], totalPath[1] )))
            {
                totalPath.clear();
                openSet.clear();
                closedSet.clear();
                cameFrom.clear();
                gScore.clear();
                fScore.clear();
                if( !(calculate( originDestination[0], originDestination[1], originDestination[2], originDestination[3])) )
                {
                    return false;
                }
            }
        }
        nodeToCoord( totalPath, 0, x, y );
        nodeToCoord( totalPath, 0, &originDestination[0], &originDestination[1] );
        totalPath.erase( totalPath.begin() );
        return true;
    }
}

int aStarGenPath::size()
{
    return totalPath.size();
}

void aStarGenPath::get( int index, int *x, int *y )
{
    nodeToCoord( totalPath, index, x, y );
}