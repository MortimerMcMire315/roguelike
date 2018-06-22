/**
 *  @file MAIN_MENU.CPP
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

MainMenu::MainMenu(int padding, Tile _border, Game* _game) : Menu(padding, _border)
{
    id = menu_id::MAIN_MENU;
    next_screen = GAME_SCREEN;
    game = _game;
    options.push_back("Equipment");
    options.push_back("Inventory");
    options.push_back("Stats");
    options.push_back("Messages");
    if(game->main_char.get_new_levels() > 0)
    {
        options.push_back("Level");
    }
    options.push_back("Return");
    title = "Main Menu";
}

Menu* MainMenu::make_selection()
{
    if(options[selection] == "Equipment")
    {
        return new EquipmentMenu(1, Tileset::get("BLOCK_WALL"), game);
    }
    else if(options[selection] == "Inventory")
    {
        return new InventoryMenu(1, Tileset::get("BLOCK_WALL"), game);
    }
    else if(options[selection] == "Stats")
    {
       return this;
    }
    else if(options[selection] == "Messages") 
    {
        return new MessageMenu(1, Tileset::get("BLOCK_WALL"), game, 1, 0);
    }
    else if(options[selection] == "Level")
    {
        return new LevelMenu(1, Tileset::get("BLOCK_WALL"), game);
    }
    else if(options[selection] == "Return")
    {
        toggle_exit();
        return this;
    }
    else
    {
        toggle_exit();
        return this;
    }
}
