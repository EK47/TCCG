#ifndef pickable_hpp
#define pickable_hpp

class Pickable
{
public:
    void drop( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
    bool pick( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
    virtual bool use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
};

class Healer : public Pickable
{
public:
    float amount;

    Healer( float amount );
    bool use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
};

class LightningBolt : public Pickable
{
public:
    float range, damage;
    LightningBolt( float range, float damage );
    bool use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
};

class Fireball : public LightningBolt
{
public:
    Fireball( float range, float damage );
    bool use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
};

class Confuser : public Pickable
{
public:
    int nbTurns;
    float range;
    Confuser( int nbTurns, float range );
    bool use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
};

#endif