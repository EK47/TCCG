#ifndef engine_hpp
#define engine_hpp

class Engine
{
public:
    enum GameStatus
    {
        STARTUP,
        IDLE,
        NEW_TURN,
        VICTORY,
        DEFEAT
    } gameStatus;

    TCODList<Actor *> actors;
    Actor *player;
    Map *map;
    int screenWidth;
    int screenHeight;
    Gui *gui;
    TCOD_key_t lastKey;
    TCOD_mouse_t mouse;

    Engine( int screenWidth, int screenHeight );
    ~Engine();
    void update();
    void render();
    void sendToBack( Actor *actor );
    Actor *getClosestMonster( int x, int y, float range ) const;
    Actor *getActor( int x, int y ) const;
    bool pickATile( int *x, int *y, float maxRange = 0.0f );
private:
    bool computeFov;
};

extern Engine engine;

#endif