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

static const int PANEL_HEIGHT=12;
static const int BAR_WIDTH=engine.screenWidth;
static const int HP_X=15;
static const int MSG_X=HP_X + 2;
static const int MSG_HEIGHT=PANEL_HEIGHT-1;

Gui::Gui() {
	guiCon = new TCODConsole(engine.screenWidth,PANEL_HEIGHT);
}

Gui::~Gui() {
	delete guiCon;
	log.clear();
}

void Gui::clear()
{
	log.clear();
}

void Gui::render() {
	// clear the GUI console
	guiCon->setDefaultBackground(TCODColor::black);
	guiCon->clear();

	// draw the health bar
	renderBar(1,1,HP_X,"HP",engine.player->destructible->hp,
		engine.player->destructible->maxHp,
		TCODColor::lightRed,TCODColor::darkerRed);

	// draw the message log
	int y=1;
	float colorCoef=0.4f;
	for ( auto &msg : log ) {
		guiCon->setDefaultForeground( msg->col * colorCoef);
		guiCon->print(MSG_X,y, msg->text);
		y++;
		if ( colorCoef < 1.0f ) {
			colorCoef+=0.3f;
		}
	}

	// mouse look
	renderMouseLook();
	if( engine.camera -> lookMode )
	{
		renderLook( engine.camera -> cameraWidth / 2, engine.camera -> cameraHeight / 2 );
	}
	// blit the GUI console on the root console
	TCODConsole::blit(guiCon,0,0,engine.screenWidth,PANEL_HEIGHT,
		TCODConsole::root,0,engine.screenHeight-PANEL_HEIGHT);
}

void Gui::renderBar(int x, int y, int width, const char *name,
	float value, float maxValue, const TCODColor &barColor,
	const TCODColor &backColor) {
	// fill the background
	guiCon->setDefaultBackground(backColor);
	guiCon->rect(x,y,width,1,false,TCOD_BKGND_SET);

	int barWidth = (int)(value / maxValue * width);
	if ( barWidth > 0 ) {
		// draw the bar
		guiCon->setDefaultBackground(barColor);
		guiCon->rect(x,y,barWidth,1,false,TCOD_BKGND_SET);
	}
	// print text on top of the bar
	guiCon->setDefaultForeground(TCODColor::white);
	guiCon->printEx(x+width/2,y,TCOD_BKGND_NONE,TCOD_CENTER,
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
	guiCon->setDefaultForeground( worldEvents );
	guiCon->print(1,0,buf);
}

// Renders the area under the current camera cursor
void Gui::renderLook( int x, int y )
{
	if( !engine.map -> isInFov( x + engine.camera -> topLeftX, y + engine.camera -> topLeftY ) )
	{
		return;
	}
	char buf[256] = "";
	bool first = true;
	for( auto &actor : engine.actors )
	{
		if( actor -> x == x + engine.camera -> topLeftX && actor -> y == y + engine.camera -> topLeftY )
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
	guiCon -> setDefaultForeground( worldEvents );
	guiCon -> print( 1, 0, buf );
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
		if ( log.size() == MSG_HEIGHT ) {
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