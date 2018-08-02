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

#ifndef actor_hpp
#define actor_hpp

// Everything that is alive, dead, or an item in the game is an actor.
class Actor
{
public:
    int x, y;
    int ch;
    TCODColor col;
    const char *name;
    bool blocks;
    std::shared_ptr<Attacker> attacker;
    std::shared_ptr<Destructible> destructible;
    std::shared_ptr<Ai> ai;
    std::shared_ptr<Pickable> pickable;
    std::shared_ptr<Container> container;
    int turnSinceFight;
    int lastLocationX = NULL;
    int lastLocationY = NULL;

    Actor( int x, int y, int ch, const char *name, const TCODColor &col );
    ~Actor();
    float getDistance( int cx, int cy ) const;
    void update( std::shared_ptr<Actor> owner );
    void render() const;
};

#endif