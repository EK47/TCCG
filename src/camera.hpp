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

#ifndef camera_hpp
#define camera_hpp

class Camera
{
public:
    int cameraWidth, cameraHeight;
    int cameraPosX, cameraPosY;
    int topLeftX, topLeftY;
    bool lookMode;
    TCODConsole *cameraConsole;
    Camera( int width, int height, bool mainCamera );
    ~Camera();
    void render();
    void playerOutRange();
    void moveAround();
private:

};

#endif