/**
 *  WORLD_MAP_GUI.H
 *
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

#ifndef WORLD_MAP_GUI
#define WORLD_MAP_GUI

#include <vector>
#include <stdlib.h>
#include <iostream>

#include "constants.h"
#include "world_map.h"
#include "int_point.h"

using namespace std;

struct TopLayerData {
    int row;
    int col;
    MapTile tile;
};


class WorldMapGUI {
    typedef std::vector<std::vector<MapTile> > MapTileMatrix;
    public:
        WorldMapGUI();
        const MapTileMatrix& get_canvas();
        const MapTileMatrix& get_map();
        IntPoint get_selected_chunk();
        void move_cursor(int, int);
        void new_world();
        void refresh();
        const WorldMap& get_world_map();

    private:
        std::vector<TopLayerData> top_layer;
        WorldMap world_map;
        MapTileMatrix canvas;
        int width;
        int height;

        void add_layers();
};

#endif
