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

#ifndef container_hpp
#define container_hpp

class Container
{
public:
    int size;
    std::vector<std::shared_ptr<Actor>> inventory;

    Container( int size );
    ~Container();
    bool add( std::shared_ptr<Actor> actor );
    void remove( std::shared_ptr<Actor> actor );
};

#endif