/**
 *  @file TILE_LOAD.H
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

#ifndef TILE_LOAD_H
#define TILE_LOAD_H

#include <string.h>
#include <unordered_map>

#include "tile.h"

namespace tile_load {

    /**
     * tileset_colorset_t is a convenience struct to be passed into the
     * handle_ini_entry function. It contains two data structures; one mapping
     * color names to SDL integers (loaded from color_load.cpp), and one mapping
     * tile names to Tile objects.
     */
    typedef struct {
        std::unordered_map<std::string, Tile> tiledefs;
        std::unordered_map<std::string, int> colordefs;
    } tileset_colorset_t;

    /**
     * A callback function for the color.ini file parser.
     * @param tilecolordefs A pointer to our tileset_colorset_t struct
     * @param name The key that is currently being parsed
     * @param value The value of the key that is being parsed
     * @return 0 for success, -1 for failure.
     */
    static int handle_ini_entry(void* tilecolordefs, const char* section, 
                                const char* name, const char* value);

    void print_tile(Tile&);
    
    /**
     * Load the tile.ini file using the inih library.
     * @return the definitive tileset for the game.
     */
    std::unordered_map<std::string, Tile> load_conf();

}

#endif
