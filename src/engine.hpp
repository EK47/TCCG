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

#ifndef engine_hpp
#define engine_hpp

// The game engine. The object that manages all of the going ons

class Engine
{
public:
    enum GameStatus
    {
        STARTUP,
        IDLE,
        NEW_TURN,
        VICTORY,
        DEFEAT
    } gameStatus;
    std::vector<std::shared_ptr<Actor>> actors;
    std::shared_ptr<Actor> player;
    std::shared_ptr<Map> map;
    std::shared_ptr<Camera> camera;
    int screenWidth;
    int screenHeight;
    int FOVRadius;
    bool trackPlayer;
    std::shared_ptr<Gui> gui;
    TCOD_key_t lastKey;
    TCOD_mouse_t mouse;

    Engine( int screenWidth, int screenHeight );
    ~Engine();
    void load();
    void save();
    void update();
    void render();
    std::shared_ptr<Actor> getClosestMonster( int x, int y, float range ) const;
    std::shared_ptr<Actor> getActor( int x, int y ) const;
    float getMouseDistance( int cx, int cy );
    bool pickATile( int *x, int *y, float displayRange = 0.0f, float maxRange = 0.0f );

    void adventureInit();
    void restartAdventure();
private:
    bool computeFov;
    int defaultMapSize;
};

extern Engine engine;

#endif