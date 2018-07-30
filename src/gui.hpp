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

#ifndef gui_hpp
#define gui_hpp
#pragma once

class Menu
{
public:
	Menu();
	~Menu();
	enum menuOptions 
	{
		NONE,
		NEW_GAME,
		CONTINUE,
		OPTIONS,
		CREDITS,
		QUIT
	};
	enum optionOptions
	{
		ONONE,
		CAMERA,
		BACK,
	};
	void clear();
	void addItem( menuOptions currentOption, optionOptions option, const char* label );
	menuOptions pick();
	optionOptions oPick();
	struct MenuItem
	{
		menuOptions code;
		optionOptions option;
		const char *label;
	};
	std::vector<std::shared_ptr<MenuItem>> items;
};

class Gui {
public:
	Menu menu;
	Menu options;
	Gui();
	~Gui();
	void render();
	void clear();
	void message(const TCODColor &col, const char *text, ...);
	void renderLook( int x, int y );
protected:
	TCODConsole *guiConBottom;
	TCODConsole *guiConRight;
	struct Message {
		char *text;
		TCODColor col;
		Message(const char *text, const TCODColor &col);
		~Message();
	};
	std::vector<std::shared_ptr<Message>> log;

	void renderBar(int x, int y, int width, int height, const char *name,
		float value, float maxValue, const TCODColor &barColor,
		const TCODColor &backColor);
	void renderMouseLook();
};

#endif