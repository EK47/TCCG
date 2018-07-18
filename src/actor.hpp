#ifndef actor_hpp
#define actor_hpp

class Actor
{
public:
    int x, y;
    int ch;
    TCODColor col;
    const char *name;
    bool blocks;
    std::shared_ptr<Attacker> attacker;
    std::shared_ptr<Destructible> destructible;
    std::shared_ptr<Ai> ai;
    std::shared_ptr<Pickable> pickable;
    std::shared_ptr<Container> container;
    int turnSinceFight;
    int lastLocationX = NULL;
    int lastLocationY = NULL;

    Actor( int x, int y, int ch, const char *name, const TCODColor &col );
    ~Actor();
    float getDistance( int cx, int cy ) const;
    void update( std::shared_ptr<Actor> owner );
    void render() const;
};

#endif