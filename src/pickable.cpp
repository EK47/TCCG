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

#include "main.hpp"

void Pickable::drop( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer )
{
    if( wearer -> container )
    {
        wearer -> container -> remove( owner );
        engine.actors.push_back( owner );
        owner -> x = wearer -> x;
        owner -> y = wearer -> y;
        //engine.map -> walls[ owner -> x + engine.map -> width * owner -> y ] = 0.3f;
        engine.gui -> message( worldEvents, "%s drops a %s.", wearer -> name, owner -> name );
    }
}

bool Pickable::state()
{
    return false;
}

bool Pickable::use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer )
{
    if( wearer -> container )
    {
        wearer -> container -> remove( owner );
        return true;
    }
    return false;
}

bool Pickable::pick( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer ) {
	if ( wearer->container && wearer->container->add(owner) ) {
		auto it = find( engine.actors.begin(), engine.actors.end(), owner );
		engine.actors.erase( it );
        return true;
	}
	return false;
}

Healer::Healer( float amount ) : amount( amount )
{

}

bool Healer::state()
{
    return false;
}

bool Healer::use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer )
{
    if( wearer -> destructible )
    {
        float amountHealed = wearer -> destructible -> heal( amount );
        if( amountHealed > 0 )
        {
            engine.gui -> message( worldEvents,"%s heals %g health", wearer->name, amountHealed);
            return Pickable::use( owner, wearer );
        }
    }
    return false;
}

LightningBolt::LightningBolt( float range, float damage ) : range( range ), damage( damage )
{

}

bool LightningBolt::state()
{
    return false;
}

bool LightningBolt::use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer )
{
    std::shared_ptr<Actor> closestMonster = engine.getClosestMonster( wearer -> x, wearer -> y, range );
    if( ! closestMonster )
    {
        engine.gui -> message(worldEvents, "The spell fizzles without a target." );
        return false;
    }
    engine.gui -> message( TCODColor::cyan, "A loud flash is seen as %s is struck with lightning!\n"
        "It is hit for %g damage!", closestMonster -> name, damage );
    closestMonster -> destructible -> takeDamage( closestMonster, damage );
    engine.render();
    for( int time = 0; time < 150; time++ )
    {
        float multiplier { 0 };
        TCODRandom *multRng = TCODRandom::getInstance();
        multiplier = sqrt( time );
        multiplier = multiplier / multRng -> getInt( 2, 4 );
        renderLine( wearer, closestMonster, multiplier, goodThing );
    }
    return Pickable::use( owner, wearer );
}


Fireball::Fireball(float range, float damage) : LightningBolt( range, damage )
{
    
}

bool Fireball::state()
{
    return false;
}

bool Fireball::use(std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer) {
	engine.gui->message( guiForeground, "Hover over a target and left-click to fire!\nRight-click to cancel.");
	int x,y;
	if( !engine.pickATile(&x, &y, range, range * 2 ) )
    {
		return false;
	}
	// burn everything in <range> (including player)
	engine.gui->message(TCODColor::orange,"It implodes, setting everything %g blocks around it on fire!",range);
	for ( auto &actor : engine.actors ) {
		if ( actor->destructible && !actor->destructible->isDead()
			&& actor->getDistance(x,y) <= range) {
			engine.gui->message(TCODColor::orange,"The %s gets burned for %g hit points.",
				actor->name,damage);
			actor->destructible->takeDamage(actor,damage);
		}
	}
	return Pickable::use(owner,wearer);
}

Confuser::Confuser( int nbTurns, float range ) : nbTurns( nbTurns ), range( range )
{

}

bool Confuser::state()
{
    return false;
}

bool Confuser::use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer )
{
    engine.gui -> message( guiForeground, "Left-click an enemy to confuse it\nor right-click to cancel.");
    int x, y;
    std::shared_ptr<Ai> confusedAi;
    if( !engine.pickATile( &x, &y, 0.0, range ) )
    {
        return false;
    }
    std::shared_ptr<Actor> target = engine.getActor( x, y );
    if( !target )
    {
        return false;
    }
    confusedAi = std::make_shared<ConfusedAi>( nbTurns, target->ai );
    target -> ai = confusedAi;
    target -> lastLocationX = NULL;
    target -> lastLocationY = NULL;
    engine.gui -> message( TCODColor::lightGreen, "The eyes of the %s look empty,\nas it falls into a walking trance!", target -> name );
    return Pickable::use( owner, wearer );
}

/*Lamp::Lamp( float brightness ) : brightness( brightness )
{
    
}

bool Lamp::state()
{
    return onOrOff;
}

bool Lamp::use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer )
{
    engine.gui -> message( TCODColor::yellow, "%s switched the lamp.", wearer -> name );
    onOrOff = !onOrOff;
    return true;
}*/