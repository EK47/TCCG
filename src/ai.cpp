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
#include <math.h>
#include <algorithm>
#include "main.hpp"








//// Monster AI

MonsterAi::MonsterAi() : moveCount(0)
{ }

MonsterAi::~MonsterAi()
{ }

void MonsterAi::update( std::shared_ptr<Actor> owner ) {
	
	// If the actor can die, and the monster is dead, stop updating.
	if ( owner -> destructible && owner -> destructible -> isDead() ) {
		return;
	}

	// If the creature is in the FOV, the creature stores the player's last seen location
	if ( engine.map -> isInFov( owner -> x, owner -> y ) ) {
		lkX = engine.player -> x;
		lkY = engine.player -> y;
	}
	// If the last known location of the player isn't NULL, moveOrAttack
	if( lkX != NULL && lkY != NULL )
	{
		moveOrAttack( owner, lkX, lkY );
	}
	// Heal naturally.
	owner -> destructible -> naturalHeal( owner );
}

void MonsterAi::moveOrAttack( std::shared_ptr<Actor> owner, int targetx, int targety )
{
	// Compute the path from the current position to the target.
	engine.map -> path -> compute( owner -> x, owner -> y, targetx, targety );
	// std::cout << owner -> name << ": " << engine.map -> path -> size() << std::endl;
	int cX, cY, nX, nY;
	// Get the current X and Y, then the next X and Y
	engine.map -> path -> getOrigin( &cX, &cY );
	engine.map -> path -> get( 0, &nX, &nY );
	// If the creature can walk to the next tile, walk there.
	if( engine.map -> canWalk( nX, nY ) )
	{
		// For Lighting
		engine.map -> walls[ owner -> x + engine.map -> width * owner -> y ] = 0.0f;

		engine.map -> path -> walk( &(owner -> x), &(owner -> y), true );
		// For Lighting
		engine.map -> walls[ owner -> x + engine.map -> width * owner -> y ] = 0.5f;
		owner -> turnSinceFight += 1;
	// If the creature's next tile is the player
	} else if( engine.player -> x == nX && engine.player -> y == nY && owner -> attacker )
	{
		owner -> attacker -> attack( owner, engine.player );
		owner -> turnSinceFight = 0;
	}
	/*
	int dx = targetx - owner->x;
	int dy = targety - owner->y;
	int stepdx = (dx > 0 ? 1:-1);
	int stepdy = (dy > 0 ? 1:-1);
	float distance=sqrtf(dx*dx+dy*dy);
	if ( distance >= 2 ) {
		dx = (int)(round(dx/distance));
		dy = (int)(round(dy/distance));
		if ( engine.map->canWalk(owner->x+dx,owner->y+dy) ) {
			owner->x += dx;
			owner->y += dy;
			owner -> turnSinceFight += 1;
		} else if ( engine.map->canWalk(owner->x+stepdx,owner->y) ) {
			owner->x += stepdx;
			owner -> turnSinceFight += 1;
		} else if ( engine.map->canWalk(owner->x,owner->y+stepdy) ) {
			owner->y += stepdy;
			owner -> turnSinceFight += 1;
		}
	} else if ( owner->attacker ) {
		owner -> turnSinceFight = 0;
		owner->attacker->attack(owner,engine.player);
	}
	*/
}









//// Player AI

void PlayerAi::update( std::shared_ptr<Actor> owner) {

	// If the player is dead, stop updating.
	if ( owner->destructible && owner->destructible->isDead() ) {
    	return;
    }

	int dx = 0, dy = 0;
	// Find out which key the player pressed last
	switch(engine.lastKey.c)
	{
		case 'k':
			dy = -1;
			break;
		case 'j':
			dy = 1;
			break;
		case 'h':
			dx = -1;
			break;
		case 'l':
			dx = 1;
			break;
		case 'y':
			dy = -1;
			dx = -1;
			break;
		case 'u':
			dy = -1;
			dx = 1;
			break;
		case 'b':
			dy = 1;
			dx = -1;
			break;
		case 'n':
			dy = 1;
			dx = 1;
			break;
		// Wait one turn
		case '.':
		{
			owner -> destructible -> naturalHeal( owner );
			owner -> turnSinceFight += 1;
			engine.gameStatus = Engine::NEW_TURN;
		}
		break;
		// Rest until fully healed
		case 't':
		{
			// Extremely rudimentary and should be changed soon. Doesn't make time pass, just one turn
			if( !(engine.map -> EnemyInFov()) )
			{
				if( owner -> destructible -> hp == owner -> destructible -> maxHp )
				{
					engine.gui -> message( worldEvents, "You are already fully rested!" );
				}
				while( owner -> destructible -> hp < owner -> destructible -> maxHp )
				{
					owner -> destructible -> hp += 1;
				}
				engine.gameStatus = Engine::NEW_TURN;
			} else
			{
				engine.gui -> message( worldEvents, "You cannot rest: Creatures nearby." );
			}
		}
		break;
		case 'd': // Drop item
        {
            std::shared_ptr<Actor> item = choseFromInventory( owner );
            if( item )
            {
                item -> pickable -> drop( item, owner );
                engine.gameStatus = Engine::NEW_TURN;
            }
        }
        break;
		case 'g': // Pickup item
		{
			bool found = false;
			for ( auto &actor : engine.actors )
			{
				// If the actor is pickable, and the owner of the PlayerAi's coordinates are the same as the item's coordinates
				if ( actor -> pickable && actor -> x == owner -> x && actor -> y == owner -> y ) {
					// The name of the item you are picking up
					auto itemname = actor -> name;
					// If the player successfully picks the item
					if ( actor -> pickable -> pick( actor, owner ) )
					{
						found = true;
						engine.gui -> message( worldEvents, "You pick up the %s.", itemname );
						engine.gameStatus=Engine::NEW_TURN;
						break;
					} else if ( !found ) {
						found = true;
						engine.gui -> message( guiForeground,"Your inventory is full.");
					}
				}
			}
			if (!found) {
				engine.gui -> message( worldEvents,"There's nothing here that you can pick up.");
			}
		}
		break;
		case 'i': // display inventory
		{
			std::shared_ptr<Actor> item = choseFromInventory( owner );
			if ( item ) {
				if( item -> pickable -> use( item, owner ) )
				{
					engine.gameStatus=Engine::NEW_TURN;
				}
			}
		}
		break;

		default: break;
	}

	// Allow you to look around using the camera.
	if( engine.lastKey.c == ';' )
	{
		engine.camera -> moveAround();
	}

    if (dx != 0 || dy != 0) {
		if( !engine.map -> isWall( owner -> x + dx, owner -> y + dy ) )
		{
    		if ( moveOrAttack(owner, owner -> x + dx, owner -> y + dy ) ) 
			{
				// Only recalculate the FOV if the next position isn't something the player can attack.
    			engine.map -> computeFov();
    		}
			engine.gameStatus = Engine::NEW_TURN;
		}
	}
}

bool PlayerAi::moveOrAttack( std::shared_ptr<Actor> owner, int targetx, int targety ) {	

	owner -> turnSinceFight += 1;

	for ( auto &actor : engine.actors )
	{
		// If the actor can die, is currently alive, and has the position of the target coordinates, the player attacks the actor
		if ( actor -> destructible && actor -> destructible -> isAlive() && actor -> x == targetx && actor -> y == targety )
		{
			owner -> turnSinceFight = 0;
			owner -> attacker -> attack( owner, actor );
			return false;
		}
	}

	// Look for all pickable objects under the player, and then display the name.
	for ( auto &actor : engine.actors )
	{
        bool item = (bool)actor -> pickable;
		if ( item && actor -> x == targetx && actor -> y == targety )
		{
			engine.gui -> message( worldEvents, "There's a %s here", actor -> name );
		}
	}

	// For Lighting
	engine.map -> walls[ owner -> x + engine.map -> width * owner -> y ] = 0.0f;

	// Update the player coordinates
	owner -> x = targetx;
	owner -> y = targety;

	// For Lighting
	engine.map -> walls[ owner -> x + engine.map -> width * owner -> y ] = 0.5f;

	// Heal naturally
	owner -> destructible -> naturalHeal( owner );

	return true;
}

std::shared_ptr<Actor> PlayerAi::choseFromInventory( std::shared_ptr<Actor> owner)
{
	// We create a console that is the inventory.
	static const int INVENTORY_WIDTH=50;
	static const int INVENTORY_HEIGHT=28;
	static TCODConsole inventory( INVENTORY_WIDTH, INVENTORY_HEIGHT );

	// Set the default foreground and background, and then print the inventory name at the top.
	inventory.setDefaultForeground( guiForeground );
	inventory.setDefaultBackground( guiBackground );
	inventory.printFrame( 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "Your Inventory" );

	// Display the items with their keyboard shortcut
	int shortcut='a';
	int y=1;
	for ( auto &item : owner -> container -> inventory )
	{
		inventory.print( 2, y, "(%c) : %s", shortcut, item -> name);
		y++;
		shortcut++;
	}

	// Blit the inventory console on the root console
	TCODConsole::blit( &inventory, 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, TCODConsole::root, engine.screenWidth / 2 - INVENTORY_WIDTH / 2, engine.screenHeight / 2 - INVENTORY_HEIGHT / 2 );
	TCODConsole::flush();

	// Wait for a key press
	TCOD_key_t key;
	TCODSystem::waitForEvent( TCOD_EVENT_KEY_PRESS, &key, NULL, true);
	if ( key.vk == TCODK_CHAR )
	{
		// Get the index of the key by subtracting the char values.
		int actorIndex = key.c - 'a';
		if ( actorIndex >= 0 && actorIndex < owner -> container -> inventory.size() )
		{
			return owner -> container -> inventory.at( actorIndex );
		}
	}
	return NULL;
}








//// Confused AI

ConfusedAi::ConfusedAi( int nbTurns, std::shared_ptr<Ai> oldAi ) : nbTurns( nbTurns ), oldAi( oldAi )
{

}

void ConfusedAi::update( std::shared_ptr<Actor> owner )
{
	// This entire thing basically gets random directions and then makes the actor go in that direction.
    TCODRandom *rng = TCODRandom::getInstance();
    int dx = rng -> getInt( -1, 1 );
    int dy = rng -> getInt( -1, 1 );
    if( dx != 0 || dy != 0 )
    {
        int destx = owner -> x + dx;
        int desty = owner -> y + dy;
        if( engine.map -> canWalk( destx, desty ) )
        {
			// For Lighting
			engine.map -> walls[ owner -> x + engine.map -> width * owner -> y ] = 0.0f;
			
            owner -> x = destx;
            owner -> y = desty;
			// For Lighting
			engine.map -> walls[ owner -> x + engine.map -> width * owner -> y ] = 0.5f;
        } else
        {
            std::shared_ptr<Actor> target;
			target = engine.getActor( destx, desty );
            if( target )
            {
                owner -> attacker -> attack( owner, target );
            }
        }
    }
    nbTurns--;
    if( nbTurns == 0 )
    {
        owner -> ai = oldAi;
    }
}








//// NPC Ai

NPCAi::NPCAi( std::shared_ptr<Actor> owner )
{
	// Keep tabs on the spawn point of the creature. The creature only walks in a certain range of the spawn point
	oX = owner -> x;
	oY = owner -> y;
}

NPCAi::~NPCAi()
{

}

void NPCAi::update( std::shared_ptr<Actor> owner )
{
	/*if ( owner -> destructible && owner -> destructible -> isDead() )
	{
    	return;
    }*/
	if( engine.map -> isInFov( owner -> x, owner -> y ) )
	{
		moveOrTalk( owner );
	}
}

void NPCAi::moveOrTalk( std::shared_ptr<Actor> owner )
{
	// Get a random set of coordinates in a certain range of the creature's starting point.
	TCODRandom *rng = TCODRandom::getInstance();
    xVal = rng -> getInt( oX - 2, oX + 1 );
	yVal = rng -> getInt( oY - 1, oY + 1 );
	engine.map -> path -> compute( owner->x, owner->y, xVal, yVal );

	int nX, nY;
	// Get the next coordinate of the path, and if the creature can go there, do so.
	engine.map -> path -> get( 0, &nX, &nY );
	if( engine.map->canWalk( nX, nY ) )
	{
		// For Lighting
		engine.map -> walls[ owner -> x + engine.map -> width * owner -> y ] = 0.0f;

		engine.map -> path -> walk( &(owner -> x), &(owner -> y), true );
		// For Lighting
		engine.map -> walls[ owner -> x + engine.map -> width * owner -> y ] = 0.5f;
	}

}

