/**
 *  @file ESCAPE_MENU.CPP
 *  @author Michael Yoder
 *
 *  @section LICENSE
 *  This file is part of ROGUELIKETHING.
 *
 *  ROGUELIKETHING is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROGUELIKETHING is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROGUELIKETHING.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "menu.h"
#include "game.h"
#include "defs.h"

EscapeMenu::EscapeMenu(int padding, Tile _border, Game* _game) : Menu(padding, _border)
{
    id = menu_id::ESCAPE_MENU;
    game = _game;
    next_screen = GAME_SCREEN;
    options.push_back("Resume");
    options.push_back("Audio");
    options.push_back("Font");
    options.push_back("Quit");
    title = "Escape Menu";
}

Menu* EscapeMenu::make_selection()
{
    if(options[selection] == "Resume")
    {
        exit = true;
        return this;
    }
    else if(options[selection] == "Audio")
    {
        return new AudioMenu(1, Tileset::get("BLOCK_WALL"), game);
    }
    else if(options[selection] == "Font")
    {
        return new FontMenu(1, Tileset::get("BLOCK_WALL"), game);
    }
    else if(options[selection] == "Quit")
    {
        //Not entirely sure what to do with this
        exit = true;
        return NULL;
    }
}

