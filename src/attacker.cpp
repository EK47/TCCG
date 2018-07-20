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
			engine.gui -> message( attacker == engine.player ? badThing : worldEvents,
				"%s attacks %s for %g hit points.", attacker->name, target->name, 
				power - target->destructible->defense);
		} else {
			engine.gui->message( worldEvents, "%s attacks %s but it has no effect!", attacker->name, target->name);			
		}
		// The target takes damage
		target -> destructible -> takeDamage( target, power );
	} else {
		engine.gui->message( worldEvents, "%s attacks %s in vain.",attacker->name,target->name);
	}
}