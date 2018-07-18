#ifndef destructible_hpp
#define destructible_hpp


class Destructible
{
public:
    float maxHp;
    float hp;
    float defense;
    const char *corpseName;

    Destructible( float maxHp, float defense, const char *corpseName );
    inline bool isDead() { return hp <= 0; }
    inline bool isAlive() { return hp >= 1; }

    float takeDamage( std::shared_ptr<Actor> owner, float damage );
    float heal( float amount );
    void naturalHeal( std::shared_ptr<Actor> owner, int turnsSinceFight );
    virtual void die( std::shared_ptr<Actor> owner );
    virtual ~Destructible() {};
};

class MonsterDestructible : public Destructible 
{
public:
   MonsterDestructible(float maxHp, float defense, const char *corpseName);
   void die( std::shared_ptr<Actor> owner);
};

class PlayerDestructible : public Destructible 
{
public:
   PlayerDestructible(float maxHp, float defense, const char *corpseName);
   void die( std::shared_ptr<Actor> owner);
};

class NPCDestructible: public Destructible
{
public:
    NPCDestructible( float maxHp, float defense, const char *corpseName );
    void die( std::shared_ptr<Actor> owner );
};

#endif