/*
 *  @file TILESET.H
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

#ifndef TILESET_H
#define TILESET_H
#include "tile.h"

#include <unordered_map>

/**
 * A singleton class which is responsible for the tileset that is loaded.
 *
 * Essentially, there are two ways of getting copies of tileset tiles through
 * this class:
 *
 * 1. Theoretically slower, but easier to remember: Use the "get" method, which
 *    performs the "singleton check".
 *
 *    Tileset::get("BLOCK_WALL");
 *
 * 2. Faster, but more of a pain - Get a pointer to the tileset for use in
 *    highly-repetitive operations:
 *
 *    std::unordered_map<std::string, Tile>* ts_pointer = &Tileset::instance()->get_tileset();
 *    for(big ol' loop) {
 *        do_something_with((*ts_pointer)["BLOCK_WALL"]);
 *    }
 *
 *    Remember, this is a pointer to the global tileset. With great power comes
 *    great responsibility. Never, ever use it as an lvalue.
 *
 *
 * IMPORTANT: Accessing the map with the [] operator is dangerous. It will not
 * throw an error. If you want to be safe, you must use tileset.at(), which will
 * throw an error.
 *
 */
class Tileset
{
    private:
        static std::unordered_map<std::string, Tile> tileset;
        static Tileset* s_instance;
        /**
         * Default constructor, does nothing, but is not accessible.
         */
        Tileset();

    public:
        /**
         * @return the underlying unordered_map
         */
        std::unordered_map<std::string, Tile>& get_tileset();

        /**
         * @param tilename the type of the tile to return
         * @return a copy of that type of tile after performing a singleton
         *         check.
         */
        static Tile get(std::string tilename);

        /**
         * @return an instance of this singleton.
         */
        static Tileset* instance();
};

#endif
