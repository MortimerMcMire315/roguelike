/**
 *  @file TILE_LOAD.CPP
 *  @author Michael Yoder, Seth Yoder
 *
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

#include <iostream>
#include <boost/filesystem.hpp>
#include <stdexcept>

#include "utility.h"
#include "tile_load.h"
#include "string.h"
#include "ini.h"
#include "conf_util.h"
#include "color_load.h"


namespace fs=boost::filesystem;

namespace tile_load {

    static int handle_ini_entry(void* tilecolordefs, const char* section, const char* name, const char* value) {
        #define MATCH(s, n) strcmp(s, n)==0
        int color_code;

        tileset_colorset_t* tcd = (tileset_colorset_t*) tilecolordefs;

        std::unordered_map<std::string, Tile>* tilemap = &tcd->tiledefs;
        std::unordered_map<std::string, int>* colormap = &tcd->colordefs;

        //(*tilemap)[section].tilename=section;
        if(MATCH(name,"char_count")) {
            (*tilemap)[section].char_count=stoi(value);
        } else if (MATCH(name, "tile_id")) {
            (*tilemap)[section].tile_id=stoi(value);
        } else if (MATCH(name, "color")) {
            try {
                color_code = (*colormap).at(value);
            } catch (const std::out_of_range& oor) {
                cout<<"ERROR: Could not find color "<<value<<" for tile "<<section<<". Exiting."<<endl;
                exit(EXIT_FAILURE);
            }
            (*tilemap)[section].color=color_code;
        } else if (MATCH(name, "can_be_moved_through")) {
            (*tilemap)[section].can_be_moved_through=stoi(value);
        } else if (MATCH(name, "visible")) {
            (*tilemap)[section].visible=stoi(value);
        } else if (MATCH(name, "opaque")) {
            (*tilemap)[section].opaque=stoi(value);
        } else if (MATCH(name, "seen")) {
            (*tilemap)[section].seen=stoi(value);
        } else if (MATCH(name, "can_build_overtop")) {
            (*tilemap)[section].can_build_overtop=stoi(value);
        } else {
            cout<<"Could not load configuration: name="<<name<<"; value="<<value<<"."<<endl;
            return 0;
        }

        return 1;
    }

    void print_tile(Tile& theTile) {
        //cout<<"Tile name: "<<theTile.tilename<<endl;
        cout<<"Font character (char_count): "<<theTile.char_count<<endl;
        cout<<"Tile ID: "<<theTile.tile_id<<endl;
        cout<<"Color: "<<theTile.color<<endl;
        cout<<"Corporeal: "<<theTile.can_be_moved_through<<endl;
        cout<<"Visible: "<<theTile.visible<<endl;
        cout<<"Opaque: "<<theTile.opaque<<endl;
        cout<<"Seen: "<<theTile.seen<<endl;
        cout<<"Can build overtop: "<<theTile.can_build_overtop<<endl;
    }

    std::unordered_map<std::string, Tile> load_conf() {
        std::unordered_map<std::string, Tile> tilemap;
        tileset_colorset_t tc = {tilemap, color_load::load_conf()};


        if (conf_util::conf_exists(TILEINI)) {
            fs::path tileconf(TILEINI);

            //handle_ini_entry is passed as a callback to ini_parse. It will keep
            //a pointer to tilemap for us, which will be populated after the .ini
            //file is parsed.
            if (ini_parse(tileconf.string().c_str(), handle_ini_entry, &tc) < 0) {
                printf("Can't load test.ini\n");
                exit(EXIT_FAILURE);
            }
        } else {
            exit(EXIT_FAILURE);
        }

        return tc.tiledefs;
    }

}
