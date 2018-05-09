#ifndef pickable_hpp
#define pickable_hpp

class Pickable
{
public:
    bool pick( Actor *owner, Actor *wearer );
    virtual bool use( Actor *owner, Actor *wearer );
};

class Healer : public Pickable
{
public:
    float amount;

    Healer( float amount );
    bool use( Actor *owner, Actor *wearer );
};

class LightningBolt : public Pickable
{
public:
    float range, damage;
    LightningBolt( float range, float damage );
    bool use( Actor *owner, Actor *wearer );
};

class Fireball : public LightningBolt
{
public:
    Fireball( float range, float damage );
    bool use( Actor *owner, Actor *wearer );
};

#endif