/**
 *  @file EQUIP_ITEM.CPP
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

EquipMenu::EquipMenu(int padding, Tile _border, Game* _game, int _item) : Menu(padding, _border)
{
    id = menu_id::EQUIP_ITEM;
    game = _game;
    item = _item;
    next_screen = GAME_SCREEN;
    options.push_back("Remove");
    options.push_back("Back");
    title = "Equip Menu";
}

Menu* EquipMenu::make_selection()
{
    if(options[selection] == "Remove")
    {
        game->main_char.remove_item(item);
    }
    return new EquipmentMenu(1, Tileset::get("BLOCK_WALL"), game);
}
