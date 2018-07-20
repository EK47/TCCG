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

#ifndef ai_hpp
#define ai_hpp

class Ai
{
public:
    virtual void update( std::shared_ptr<Actor> owner ) = 0;
    virtual ~Ai() {};
    bool seen { false };
};

class PlayerAi : public Ai
{
public:
    void update( std::shared_ptr<Actor> owner );
protected:
    bool moveOrAttack( std::shared_ptr<Actor> owner, int targetx, int targety );
    void handleActionKey( std::shared_ptr<Actor> owner, int ascii );
    std::shared_ptr<Actor> choseFromInventory( std::shared_ptr<Actor> owner );
};

class MonsterAi: public Ai
{
public:
    // Last known X and Y of player
    int lkX { NULL }, lkY { NULL };
    MonsterAi();
    ~MonsterAi();
    void update( std::shared_ptr<Actor> owner );
protected:
    void moveOrAttack( std::shared_ptr<Actor> owner, int targetx, int targety );
    int moveCount;
};

class ConfusedAi : public Ai
{
public:
    ConfusedAi( int nbTurns, std::shared_ptr<Ai> oldAi );
    void update( std::shared_ptr<Actor> owner );
protected:
    int nbTurns;
    std::shared_ptr<Ai> oldAi;
};

class NPCAi : public Ai
{
public:
    NPCAi( std::shared_ptr<Actor> owner );
    ~NPCAi();
    void update( std::shared_ptr<Actor> owner );
protected:
    void moveOrTalk( std::shared_ptr<Actor> owner );
private:
    int xVal { NULL };
    int yVal { NULL };
    int oX;
    int oY;
};
#endif