/**
 *  @file MESSAGE_MENU.CPP
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


MessageMenu::MessageMenu(int padding, Tile _border, Game* _game, int _page, int _selection) : Menu(padding, _border, _selection)
{
    id = menu_id::MESSAGE_MENU;
    game = _game;
    page = _page;
    per_page = 10;
    next_screen = GAME_SCREEN;
    int num_messages = MessageBoard::instance().num_messages();
    if(num_messages > (per_page * page)) {
        options.push_back("Older Messages");
    }
    if(page > 1) {
        options.push_back("New Messages");
    }
    options.push_back("Back");
    title = "Messages";
    int start = (page - 1) * per_page;
    int end = start + per_page <= num_messages ? start + per_page : num_messages;
    if(start <= num_messages &&end <= num_messages) {
        for(int i=start;i<end;i++) {
            extra_lines.push_back(MessageBoard::instance().get_message(i));
        }
    }
    std::stringstream ss;
    ss << "Page: " << page;
    extra_lines.push_back("");
    extra_lines.push_back(ss.str());

}

Menu* MessageMenu::make_selection()
{
    if(options[selection] == "Older Messages") {
        return new MessageMenu(1, Tileset::get("BLOCK_WALL"), game, page + 1, 0);
    } else if(options[selection] == "New Messages") {
        return new MessageMenu(1, Tileset::get("BLOCK_WALL"), game, page - 1, 1);
    } else {
        return new MainMenu(1, Tileset::get("BLOCK_WALL"),  game);
    }
}

