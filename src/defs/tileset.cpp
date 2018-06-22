/*
 *  @file TILESET.CPP
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

#include "tile_load.h"
#include "tileset.h"
#include <iostream>

std::unordered_map<std::string, Tile> Tileset::tileset = tile_load::load_conf();
Tileset* Tileset::s_instance = 0;

Tileset::Tileset() {

}

std::unordered_map<std::string, Tile>& Tileset::get_tileset() {
    return tileset;
}

Tile Tileset::get(std::string tilename) {
    if (!s_instance) {
        s_instance = new Tileset;
    }
    return tileset[tilename];
}

Tileset* Tileset::instance() {
    if (!s_instance) {
        s_instance = new Tileset;
    }
    return s_instance;
}
