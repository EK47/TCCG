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
    Attacker *attacker;
    Destructible *destructible;
    Ai *ai;
    Pickable *pickable;
    Container *container;
    int turnSinceFight;
    int lastLocationX = -100;
    int lastLocationY = -100;

    Actor( int x, int y, int ch, const char *name, const TCODColor &col );
    ~Actor();
    float getDistance( int cx, int cy ) const;
    void update();
    void render() const;
};

#endif