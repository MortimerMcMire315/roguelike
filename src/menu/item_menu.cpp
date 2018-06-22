/**
 *  @file ITEM_MENU.CPP
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
#include "item.h"
#include "game.h"
#include "defs.h"


ItemMenu::ItemMenu(int padding, Tile _border, Game* _game, Item* _item) : Menu(padding, _border)
{
    id = menu_id::ITEM_MENU;
    game = _game;
    item = _item;
    next_screen = GAME_SCREEN;
    if(item->can_use)
    {
        options.push_back("Use");
    }
    if(item->can_equip || item->can_wield)
    {
        options.push_back("Equip");
    }
    if(item->can_consume)
    {
        options.push_back("Consume");
    }
    options.push_back("Info");
    options.push_back("Drop");
    options.push_back("Destroy");
    options.push_back("Back");
    title = item->get_name();
}

Menu* ItemMenu::make_selection()
{
    if(options[selection] == "Use")
    {
        //use the item...not entirely sure what this will look like.
    }
    else if(options[selection] == "Equip")
    {
        game->main_char.equip_item(item);
    }
    else if(options[selection] == "Consume")
    {
        game->main_char.consume_item(item);
    }
    else if(options[selection] == "Info")
    {
        return new InfoMenu(1, Tileset::get("BLOCK_WALL"), game, item);
    }
    else if(options[selection] == "Drop")
    {
        game->drop_item(item, &game->main_char);
    }
    else if(options[selection] == "Destroy")
    {
        game->main_char.destroy_item(item);
    }
    return new InventoryMenu(1, Tileset::get("BLOCK_WALL"),  game);
}

