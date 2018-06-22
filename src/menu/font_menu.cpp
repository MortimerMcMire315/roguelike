/**
 *  @file FONT_MENU.CPP
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


#define FONTDIR DATADIR "/font/"
FontMenu::FontMenu(int padding, Tile _border, Game* _game) : Menu(padding, _border)
{
    font = "";
    id = menu_id::FONT_MENU;
    game = _game;
    next_screen = GAME_SCREEN;
    options = utility::all_files_of_type(FONTDIR, ".bmp");
    options.push_back("Back");
    title = "Font Menu";
}

Menu* FontMenu::make_selection()
{
    if(selection != options.size() - 1)
    {
        font = options[selection];
        cout<<typeid(font).name()<<endl;
        return this;
    }
    return new EscapeMenu(1, Tileset::get("BLOCK_WALL"), game);
}


string FontMenu::get_font()
{
    return font;
}
