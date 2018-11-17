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

class Door : public Actor
{
public:
    enum State
    {
        OPEN,
        CLOSED
    };
    
    Door( int x, int y, int ch, const char *name, const TCODColor &col );
    ~Door();
    
    State state { State::CLOSED };

    void genericInteraction();
    int returnInfo();
};