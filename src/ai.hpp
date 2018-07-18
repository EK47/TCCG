#ifndef ai_hpp
#define ai_hpp

class Ai
{
public:
    virtual void update( std::shared_ptr<Actor> owner ) = 0;
    virtual ~Ai() {};
    bool seen { false };
};

class PlayerAi : public Ai
{
public:
    void update( std::shared_ptr<Actor> owner );
protected:
    bool moveOrAttack( std::shared_ptr<Actor> owner, int targetx, int targety );
    void handleActionKey( std::shared_ptr<Actor> owner, int ascii );
    std::shared_ptr<Actor> choseFromInventory( std::shared_ptr<Actor> owner );
};

class MonsterAi: public Ai
{
public:
    // Last known X and Y of player
    int lkX { NULL }, lkY { NULL };
    MonsterAi();
    ~MonsterAi();
    void update( std::shared_ptr<Actor> owner );
protected:
    void moveOrAttack( std::shared_ptr<Actor> owner, int targetx, int targety );
    int moveCount;
};

class ConfusedAi : public Ai
{
public:
    ConfusedAi( int nbTurns, std::shared_ptr<Ai> oldAi );
    void update( std::shared_ptr<Actor> owner );
protected:
    int nbTurns;
    std::shared_ptr<Ai> oldAi;
};

class NPCAi : public Ai
{
public:
    NPCAi( std::shared_ptr<Actor> owner );
    ~NPCAi();
    void update( std::shared_ptr<Actor> owner );
protected:
    void moveOrTalk( std::shared_ptr<Actor> owner );
private:
    int xVal { NULL };
    int yVal { NULL };
    int oX;
    int oY;
};
#endif