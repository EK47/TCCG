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

#ifndef color_hpp
#define color_hpp

    static const TCODColor worldEvents( 102, 102, 102 );
    static const TCODColor goodThing( 51, 153, 255 );
    static const TCODColor badThing( 255, 102, 255 );
    static const TCODColor magicalThing( 128, 0, 255 );
    static const TCODColor oddThing( 0, 0, 217 );
    static const TCODColor guiBackground( 0, 51, 102 );
    static const TCODColor guiForeground( 0, 134, 178 );

    static const TCODColor dungeon1Background( 20, 20, 20 );
    static const TCODColor dungeon1Wall( 50, 140, 50 );
    static const TCODColor dungeon1Floor( 60, 60, 60 );
    static const TCODColor dungeonStairs( TCODColor::sepia );

    static const TCODColor calvin1( TCODColor::lightSepia );
    static const TCODColor orc( TCODColor::desaturatedGreen );
    static const TCODColor troll( TCODColor::darkerGreen );
    static const TCODColor demon( TCODColor::darkerRed );
    static const TCODColor bob( TCODColor::darkBlue );
    static const TCODColor corpse( 140, 0, 0 );
    static const TCODColor cockroach( TCODColor::darkerSepia );

    static const TCODColor ration( 30, 175, 30 );
    static const TCODColor scroll( TCODColor::lightYellow );

    TCODColor returnUnseen( const TCODColor &col );

#endif