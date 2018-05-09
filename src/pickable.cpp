#include "main.hpp"

bool Pickable::use( Actor *owner, Actor *wearer )
{
    if( wearer -> container )
    {
        wearer -> container -> remove( owner );
        delete owner;
        return true;
    }
    return false;
}

bool Pickable::pick(Actor *owner, Actor *wearer) {
	if ( wearer->container && wearer->container->add(owner) ) {
		engine.actors.remove(owner);
		return true;
	}
	return false;
}

Healer::Healer( float amount ) : amount( amount )
{

}

bool Healer::use( Actor *owner, Actor *wearer )
{
    if( wearer -> destructible )
    {
        float amountHealed = wearer -> destructible -> heal( amount );
        if( amountHealed > 0 )
        {
            engine.gui -> message( TCODColor::lighterGrey ,"%s heals %g health", wearer->name, amountHealed);
            return Pickable::use( owner, wearer );
        }
    }
    return false;
}