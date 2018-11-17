/*

    The Calvin Chronicle's Game
    Copyright (C) 2018 Ethan Kelly

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include "main.hpp"

Destructible::Destructible( float maxHp, float defense, const char *corpseName, const TCODColor &col, char corpseChar, int healTime ) : maxHp(maxHp), hp(maxHp), defense(defense), corpseName(corpseName), corpseColor( col ), corpseChar( corpseChar), healTime( healTime )
{
	if( healTime == 0 )
	{
		healTime = 15;
	}
}

float Destructible::takeDamage( std::shared_ptr<Actor> owner, float damage ) {
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
	// Health is healed by a certain amount. Health cannot exceed maximum HP
    hp += amount;
    hp = std::min( hp, maxHp );
    return amount;
}

void Destructible::naturalHeal( std::shared_ptr<Actor> owner )
{
	// Every [BLANK] turns, the creature is naturally healed by one HP
	if( owner -> turnSinceFight % healTime == 0 && owner -> destructible -> hp < owner -> destructible -> maxHp )
	{
		owner -> destructible -> hp += 1;
	}
}

void Destructible::die( std::shared_ptr<Actor> owner ) {
	// Transform the actor into a corpse!
	owner -> ch = corpseChar;
	owner -> col = corpseColor;	
	owner -> name = corpseName;
	owner -> blocks = false;
	// Moves item towards the beginning of the actors vector, so creatures are rendered first.
	engine.actors.erase( std::remove( engine.actors.begin(), engine.actors.end(), owner ), engine.actors.end() );
	engine.actors.insert( engine.actors.begin(), owner );
}

MonsterDestructible::MonsterDestructible( float maxHp, float defense, const char *corpseName, const TCODColor &col, char corpseChar ) : Destructible( maxHp, defense, corpseName, col, corpseChar )
{

}

void MonsterDestructible::die( std::shared_ptr<Actor> owner ) {
	// Kill the creature.
	engine.gui -> message( worldEvents, "%s is dead", owner -> name );
	Destructible::die( owner );
}

PlayerDestructible::PlayerDestructible( float maxHp, float defense, const char *corpseName, const TCODColor &col, char corpseChar ) : Destructible( maxHp, defense, corpseName, col, corpseChar )
{

}

void PlayerDestructible::die( std::shared_ptr<Actor> owner )
{
	// Display that the player has died, and kill the player.
	engine.gui -> message( badThing, "You died!" );
	owner -> destructible -> hp = 0;
	Destructible::die( owner );
	engine.gameStatus=Engine::DEFEAT;
}

NPCDestructible::NPCDestructible( float maxHp, float defense, const char *corpseName, const TCODColor &col, char corpseChar ) : Destructible( maxHp, defense, corpseName, col, corpseChar )
{

}

void NPCDestructible::die( std::shared_ptr<Actor> owner )
{
	// Kill the NPC
	engine.gui -> message( worldEvents, "Poor helpless %s died.", owner -> name );
	Destructible::die( owner );
}

ObjectDestructible::ObjectDestructible( float maxHp, float defense, const char *corpseName, const TCODColor &col, char corpseChar ) : Destructible( maxHp, defense, corpseName, col, corpseChar )
{

}

void ObjectDestructible::die( std::shared_ptr<Actor> owner )
{
	engine.gui -> message( worldEvents, "The %s breaks into pieces!", owner -> name );
	Destructible::die( owner );
}