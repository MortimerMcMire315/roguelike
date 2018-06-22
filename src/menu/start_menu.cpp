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

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/filesystem.hpp>
#include <assert.h>
#include <sstream>

#include "menu.h"
#include "defs.h"
#include "game.h"
#include "world_map_gui.h"


using namespace boost::posix_time;
namespace fs=boost::filesystem;

StartMenu::StartMenu(int padding, Tile _border, Game &g, WorldMapGUI &wmg) : Menu(padding, _border)
{
    game = &g;
    world_map_gui = &wmg;
    id = menu_id::START_MENU;
    next_screen = MAP_SCREEN;
    title = "Boots and Kobolds";
    options.push_back("New Game");
    options.push_back("Continue");
}

Menu* StartMenu::make_selection() {
    switch(selection) {
        case 0:
            toggle_exit();
            create_save_folder();
            next_screen = MAP_SCREEN;
            return this;
            break;
        case 1:
            next_screen = GAME_SCREEN;
            return new LoadMenu(1, Tileset::get("BLOCK_WALL"), *game, *world_map_gui);
            break;
        default:
            toggle_exit();
            return this;
            break;
    }
}

void StartMenu::create_save_folder() {
    stringstream ss;
    ss<<SAVE_DIR<<"/";
    time_facet *facet = new time_facet("%Y%m%d_%H%M%S");
    ss.imbue(locale(cout.getloc(), facet));
    ss<<second_clock::local_time();

    string full_save_folder_path = ss.str();

    fs::path savedir(full_save_folder_path);
    if(!fs::create_directory(savedir)) {
        cout<<"ERROR: Could not create save directory."<<endl;
        assert(false);
    }

    game->set_name(full_save_folder_path);
}
