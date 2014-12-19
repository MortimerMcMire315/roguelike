/**
 *  @file LOAD_MENU.CPP
 *  @author Seth Yoder
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

LoadMenu::LoadMenu(int padding, Tile _border) : Menu(padding, _border)
{
    id = menu_id::LOAD_MENU;
    next_screen = GAME_SCREEN;
    options.push_back("Nothing.");
    title="    Load something    ";
}

Menu* LoadMenu::make_selection()
{
    switch (selection)
    {
        case 0:
            toggle_exit();
            return this;
            break;
        default:
            toggle_exit();
            return this;
            break;
    }
}
