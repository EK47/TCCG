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

#include <stdio.h>
#include <stdarg.h>
#include "main.hpp"

static const int HP_WIDTH = 26;
static const int MSG_X = 1;

Gui::Gui() {
	guiConBottom = new TCODConsole( "../assets/gui1.xp" );
	guiConRight = new TCODConsole( "../assets/gui2.xp" );
}

Gui::~Gui() {
	delete guiConBottom;
	delete guiConRight;
	log.clear();
}

void Gui::clear()
{
	log.clear();
}

void Gui::render() {
	// clear the GUI consoles
	guiConBottom -> clear();
	guiConRight -> clear();

	guiConBottom -> loadXp( "../assets/gui1.xp" );
	guiConRight -> loadXp( "../assets/gui2.xp" );

	// draw the health bar
	renderBar( 2, 2, HP_WIDTH, 3, "HP", engine.player->destructible->hp,
		engine.player->destructible->maxHp,
		TCODColor::lightRed,TCODColor::darkerRed);

	// draw the message log
	int y=1;
	float colorCoef=0.5f;
	for ( auto &msg : log ) {
		guiConBottom->setDefaultForeground( msg->col * colorCoef );
		guiConBottom->print(MSG_X, y, msg->text);
		y++;
		if ( colorCoef < 1.0f ) {
			colorCoef+=0.05f;
		}
	}

	// mouse look
	renderMouseLook();
	if( engine.camera -> lookMode )
	{
		renderLook( engine.camera -> cameraWidth / 2, engine.camera -> cameraHeight / 2 );
	}
	// blit the GUI console on the root console
	TCODConsole::blit( guiConBottom, 0, 0, engine.camera -> cameraWidth, 25, TCODConsole::root, 0, 75 );
	TCODConsole::blit( guiConRight, 0, 0, engine.screenWidth - engine.camera -> cameraWidth, engine.screenHeight, TCODConsole::root, engine.camera -> cameraWidth, 0 );
}

void Gui::renderBar(int x, int y, int width, int height, const char *name,
	float value, float maxValue, const TCODColor &barColor,
	const TCODColor &backColor) {
	// fill the background
	guiConRight->setDefaultBackground(backColor);
	guiConRight->rect(x,y,width,height,false,TCOD_BKGND_SET);

	int barWidth = (int)(value / maxValue * width);
	if ( barWidth > 0 ) {
		// draw the bar
		guiConRight->setDefaultBackground(barColor);
		guiConRight->rect(x,y,barWidth,height,false,TCOD_BKGND_SET);
	}
	// print text on top of the bar
	guiConRight->setDefaultForeground(TCODColor::white);
	guiConRight->printEx(x+width/2,y+height/2,TCOD_BKGND_NONE,TCOD_CENTER,
		"%s : %g/%g", name, value, maxValue);
}

Gui::Message::Message(const char *text, const TCODColor &col) :
	text(strdup(text)),col(col) {	
}

Gui::Message::~Message() {
	free(text);
}

void Gui::renderMouseLook() {
	if (! engine.map->isInFov(engine.mouse.cx + engine.camera -> topLeftX, engine.mouse.cy + engine.camera -> topLeftY ) ) {
		// if mouse is out of fov, nothing to render
		return;
	}
	char buf[256]="";
	bool first=true;
	for ( auto &actor : engine.actors ) {
		// find actors under the mouse cursor
		if ( actor->x == engine.mouse.cx + engine.camera -> topLeftX && actor->y == engine.mouse.cy + engine.camera -> topLeftY ) {
			if (! first) {
				strcat(buf,", ");
			} else {
				first=false;
			}
			strcat(buf,actor->name);
		}
	}
	// display the list of actors under the mouse cursor
	guiConRight->setDefaultForeground( worldEvents );
	guiConRight->print(2,51,buf);
}

// Renders the area under the current camera cursor
void Gui::renderLook( int x, int y )
{
	char buf[256] = "";
	bool first = true;
	for( auto &actor : engine.actors )
	{
		if( actor -> x == x + engine.camera -> topLeftX && actor -> y == y + engine.camera -> topLeftY && engine.map -> isInFov( x + engine.camera -> topLeftX, y + engine.camera -> topLeftY ) )
		{
			if( !first )
			{
				strcat( buf, ", " );
			} else
			{
				first = false;
			}
			strcat( buf, actor -> name );
		} else if( actor -> lastLocationX != NULL && actor -> lastLocationY != NULL )
		{
			if( actor -> lastLocationX == x + engine.camera -> topLeftX && actor -> lastLocationY == y + engine.camera -> topLeftY )
			{
				if( !first )
				{
					strcat( buf, ", " );
				} else
				{
					first = false;
				}
				strcat( buf, actor -> name );
			}
		}
	}
	guiConRight -> setDefaultForeground( worldEvents );
	guiConRight -> print( 2, 51, buf );
}

void Gui::message(const TCODColor &col, const char *text, ...) {
	// build the text
	va_list ap;
	char buf[256];
	va_start(ap,text);
	vsprintf(buf,text,ap);
	va_end(ap);

	char *lineBegin=buf;
	char *lineEnd;
	do {
		// make room for the new message
		if ( log.size() == engine.screenHeight - engine.camera -> cameraHeight - 2 ) {
			log.erase(begin(log));
		}

		// detect end of the line
		lineEnd=strchr(lineBegin,'\n');
		if ( lineEnd ) {
			*lineEnd='\0';
		}

		// add a new message to the log
		std::shared_ptr<Message> msg = std::make_shared<Message>( lineBegin, col );
		log.push_back(msg);

		// go to next line
		lineBegin=lineEnd+1;
	} while ( lineEnd );
}

	// 77, 57

Menu::Menu()
{

}

Menu::~Menu()
{
	clear();
}

void Menu::clear()
{
	items.clear();
}

void Menu::addItem( Menu::menuOptions code, Menu::optionOptions option, const char *label )
{
	std::shared_ptr<MenuItem> item = std::make_shared<MenuItem>();
	item -> code = code;
	item -> option = option;
	item -> label = label;
	items.push_back( item );
}

Menu::menuOptions Menu::pick()
{
    std::shared_ptr<TCODConsole> background = std::make_shared<TCODConsole>( "../assets/background.xp" );
    int selectedItem = 0;
	while( !TCODConsole::isWindowClosed() )
    {
		int currentOption = 0;

        TCODConsole::blit( background.get(), 0, 0, 120, 100, TCODConsole::root, 0, 0 );
		
		for( auto item : items )
		{
			if( currentOption == selectedItem )
			{
				TCODConsole::root -> setDefaultForeground( TCODColor::grey );
			} else
			{
				TCODConsole::root -> setDefaultForeground( TCODColor::white );
			}
			TCODConsole::root -> print( 77, 57 + currentOption * 2, item -> label );
			currentOption++;
		}

        TCODConsole::flush();

        TCOD_key_t key = TCODConsole::checkForKeypress( TCOD_KEY_PRESSED );
        
        switch( key.vk )
        {
            case TCODK_UP:
                selectedItem--;
				if( selectedItem < 0 )
				{
					selectedItem = items.size() - 1;
				}
            break;
            case TCODK_DOWN:
                selectedItem = ( selectedItem + 1 ) % items.size();
            break;

            case TCODK_ENTER:
				return items.at( selectedItem ) -> code;
            break;
            default: break;
        }
    }
	return menuOptions::NONE;
}

Menu::optionOptions Menu::oPick()
{
	std::shared_ptr<TCODConsole> option = std::make_shared<TCODConsole>( "../assets/options.xp" );
	int selectedItem = 0;
	while( !TCODConsole::isWindowClosed() )
	{
		int currentOption = 0;

		TCODConsole::blit( option.get(), 0, 0, 43, 31, TCODConsole::root, 75, 47 );

		for( auto item : items )
		{
			if( currentOption == selectedItem )
			{
				TCODConsole::root -> setDefaultForeground( TCODColor::grey );
			} else
			{
				TCODConsole::root -> setDefaultForeground( TCODColor::white );
			}
			TCODConsole::root -> print( 77, 57 + currentOption * 2, item -> label );
			currentOption++;
		}

		TCODConsole::flush();

		TCOD_key_t key = TCODConsole::checkForKeypress( TCOD_KEY_PRESSED );

		switch( key.vk )
		{
			case TCODK_UP:
                selectedItem--;
				if( selectedItem < 0 )
				{
					selectedItem = items.size() - 1;
				}
            break;
            case TCODK_DOWN:
                selectedItem = ( selectedItem + 1 ) % items.size();
            break;

            case TCODK_ENTER:
				return items.at( selectedItem ) -> option;
            break;
            default: break;
		}
	}
	return optionOptions::ONONE;
}