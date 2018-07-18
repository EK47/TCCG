#include <stdio.h>
#include "main.hpp"

Attacker::Attacker( float power ) : power( power )
{

}

void Attacker::attack( std::shared_ptr<Actor> attacker, std::shared_ptr<Actor> target)
{
	if ( target->destructible && !target->destructible->isDead() )
	{
		// If the strength of the owner is larger than the target
		if ( power - target -> destructible -> defense > 0 )
		{
			// Blah hits blah. Red if player, grey if not
			engine.gui -> message( attacker == engine.player ? TCODColor::red : TCODColor::lightGrey,
				"%s attacks %s for %g hit points.", attacker->name, target->name, 
				power - target->destructible->defense);
		} else {
			engine.gui->message(TCODColor::lightGrey,
				"%s attacks %s but it has no effect!", attacker->name, target->name);			
		}
		// The target takes damage
		target -> destructible -> takeDamage( target, power );
	} else {
		engine.gui->message(TCODColor::lightGrey,
			"%s attacks %s in vain.",attacker->name,target->name);
	}
}