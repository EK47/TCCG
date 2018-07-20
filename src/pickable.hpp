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

#ifndef pickable_hpp
#define pickable_hpp

class Pickable
{
public:
    void drop( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
    bool pick( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
    virtual bool use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
};

class Healer : public Pickable
{
public:
    float amount;

    Healer( float amount );
    bool use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
};

class LightningBolt : public Pickable
{
public:
    float range, damage;
    LightningBolt( float range, float damage );
    bool use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
};

class Fireball : public LightningBolt
{
public:
    Fireball( float range, float damage );
    bool use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
};

class Confuser : public Pickable
{
public:
    int nbTurns;
    float range;
    Confuser( int nbTurns, float range );
    bool use( std::shared_ptr<Actor> owner, std::shared_ptr<Actor> wearer );
};

#endif