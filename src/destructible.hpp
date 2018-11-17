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

#ifndef destructible_hpp
#define destructible_hpp

// The destructible class. Handles dying. Meant for inheritance.
class Destructible
{
public:
    float maxHp;
    float hp;
    float defense;
    int healTime;
    const char *corpseName;
    TCODColor corpseColor;
    char corpseChar;

    Destructible( float maxHp, float defense, const char *corpseName, const TCODColor &col, char corpseChar, int healTime = 15 );
    inline bool isDead() { return hp <= 0; }
    inline bool isAlive() { return hp >= 1; }

    float takeDamage( std::shared_ptr<Actor> owner, float damage );
    float heal( float amount );
    void naturalHeal( std::shared_ptr<Actor> owner );
    virtual void die( std::shared_ptr<Actor> owner );
    virtual ~Destructible() {};
};

// Hostile creature death. Confused Ai death also is part of this.
class MonsterDestructible : public Destructible 
{
public:
   MonsterDestructible( float maxHp, float defense, const char *corpseName, const TCODColor &col, char corpseChar );
   void die( std::shared_ptr<Actor> owner );
};

// Player death
class PlayerDestructible : public Destructible 
{
public:
   PlayerDestructible( float maxHp, float defense, const char *corpseName, const TCODColor &col, char corpseChar );
   void die( std::shared_ptr<Actor> owner );
};

// NPC death
class NPCDestructible : public Destructible
{
public:
    NPCDestructible( float maxHp, float defense, const char *corpseName, const TCODColor &col, char corpseChar );
    void die( std::shared_ptr<Actor> owner );
};

class ObjectDestructible : public Destructible
{
public:
    ObjectDestructible( float maxHp, float defense, const char *corpseName, const TCODColor &col, char corpseChar );
    void die( std::shared_ptr<Actor> owner );
};

#endif