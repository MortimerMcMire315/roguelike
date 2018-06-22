/**
 *  @file EQUIPMENT_MENU.CPP
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

#include <unordered_map>

#include "menu.h"
#include "item.h"
#include "game.h"
#include "defs.h"



EquipmentMenu::EquipmentMenu(int padding, Tile _border, Game* _game) : Menu(padding, _border)
{
    id = menu_id::EQUIPMENT_MENU;
    game = _game;
    vector<Item*>* items = game->main_char.get_equipment();
    for(int i=0;i<items->size();i++)
    {
        if(items->at(i) != NULL)
        {
            options.push_back(BODY_PARTS[i] + std::string(": ")+items->at(i)->get_name());
        }
        else
        {
            options.push_back(BODY_PARTS[i] + std::string(": None"));
        }
    }
    options.push_back("Back");
    next_screen = GAME_SCREEN;
    title = "Equipment";
}

Menu* EquipmentMenu::make_selection()
{
    vector<Item*>* items = game->main_char.get_equipment();
    if(selection == options.size() - 1)
    {
        return new MainMenu(1, Tileset::get("BLOCK_WALL"), game);
    }
    else
    {
        if(items->at(selection) != NULL)
        {
            return new EquipMenu(1, Tileset::get("BLOCK_WALL"), game, selection);
        }
        else
        {
            return this;
        }
    }
    return this;
}
