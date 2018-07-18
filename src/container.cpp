#include "main.hpp"

Container::Container( int size ) : size( size )
{

}

Container::~Container()
{
    inventory.clear();
}

bool Container::add( std::shared_ptr<Actor> actor )
{
    if( size > 0 && inventory.size() >= size )
    {
        return false;
    }

    inventory.push_back( actor );
    return true;
}

void Container::remove( std::shared_ptr<Actor> actor )
{
    auto it = find( inventory.begin(), inventory.end(), actor );
    inventory.erase( it );
}