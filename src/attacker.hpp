#ifndef attacker_hpp
#define attacker_hpp

class Attacker
{
public:
    float power;

    Attacker( float power );
    void attack( std::shared_ptr<Actor> attacker, std::shared_ptr<Actor> target );
};

#endif