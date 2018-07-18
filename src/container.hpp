#ifndef container_hpp
#define container_hpp

class Container
{
public:
    int size;
    std::vector<std::shared_ptr<Actor>> inventory;

    Container( int size );
    ~Container();
    bool add( std::shared_ptr<Actor> actor );
    void remove( std::shared_ptr<Actor> actor );
};

#endif