#ifndef attacker_hpp
#define attacker_hpp

class Attacker
{
public:
    float power;

    Attacker( float power );
    void attack( Actor *owner, Actor *target );
};

#endif