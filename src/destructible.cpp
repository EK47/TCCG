#include <stdio.h>
#include "main.hpp"

Destructible::Destructible(float maxHp, float defense, const char *corpseName) : maxHp(maxHp), hp(maxHp), defense(defense), corpseName(corpseName) {
}

float Destructible::takeDamage( std::shared_ptr<Actor> owner, float damage) {
	// Damage being dealt minus defense leaves amount to be subtracted from HP.
	damage -= defense;
	// If damage is actually done
	if ( damage > 0 ) {
		hp -= damage;
		// If the HP drops, the thing dies
		if ( hp <= 0 ) {
			die( owner );
		}
	} else {
		// Damage is none
		damage = 0;
	}

	return damage;
}

float Destructible::heal( float amount )
{
    hp += amount;
    if( hp > maxHp )
    {
        amount -= hp - maxHp;
        hp = maxHp;
    }
    return amount;
}

void Destructible::naturalHeal( std::shared_ptr<Actor> owner, int turnSinceFight )
{
	if( turnSinceFight % 5 == 0 && owner -> destructible -> hp < owner -> destructible -> maxHp )
	{
		turnSinceFight = 0;
		owner -> destructible -> hp = owner -> destructible -> hp + 1;
	}
}

void Destructible::die( std::shared_ptr<Actor> owner ) {
	// transform the actor into a corpse!
	owner->ch='~';
	owner->col=TCODColor::darkRed;	
	owner->name=corpseName;
	owner->blocks=false;
	// Prioritize creatures over corpses. Should be obselete when the multi-actor tile switch system is done.
	// engine.actors.erase( std::remove( engine.actors.begin(), engine.actors.end(), owner ), engine.actors.end() );
	// engine.actors.insert( engine.actors.begin(), owner );
}

MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
	Destructible(maxHp,defense,corpseName) {
}

void MonsterDestructible::die( std::shared_ptr<Actor> owner ) {
	// transform it into a nasty corpse! it doesn't block, can't be
	// attacked and doesn't move
	engine.gui->message(TCODColor::lightGrey,"%s is dead",owner->name);
	Destructible::die( owner );
}

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
	Destructible(maxHp,defense,corpseName) {
}

void PlayerDestructible::die( std::shared_ptr<Actor> owner ) {
	engine.gui->message(TCODColor::red,"You died!");
	Destructible::die(owner);
	engine.gameStatus=Engine::DEFEAT;
}

NPCDestructible::NPCDestructible( float maxHp, float defense, const char* corpseName ) :
	Destructible( maxHp, defense, corpseName )
{

}

void NPCDestructible::die( std::shared_ptr<Actor> owner )
{
	engine.gui -> message( TCODColor( 102, 102, 102 ), "Poor helpess %s died.", owner -> name );
	Destructible::die( owner );
}