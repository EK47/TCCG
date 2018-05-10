#ifndef ai_hpp
#define ai_hpp

class Ai
{
public:

    virtual void update( Actor *owner ) = 0;
    virtual ~Ai() {};
};

class PlayerAi : public Ai
{
public:
    void update( Actor *owner );

protected:
    bool moveOrAttack( Actor *owner, int targetx, int targety );
    void handleActionKey( Actor *owner, int ascii );
    Actor *choseFromInventory( Actor *owner );
};

class MonsterAi: public Ai
{
public:
    void update( Actor *owner );
    MonsterAi();

protected:
    void moveOrAttack( Actor *owner, int targetx, int targety );
    int moveCount;
};

class ConfusedAi : public Ai
{
public:
    ConfusedAi( int nbTurns, Ai *oldAi );
    void update( Actor *owner );
protected:
    int nbTurns;
    Ai *oldAi;
};

#endif