/**
 *  WORLD_MAP_GUI.CPP
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

#include "world_map_gui.h"

typedef std::vector<std::vector<MapTile> > MapTileMatrix;
WorldMapGUI::WorldMapGUI() {
    height = WORLD_HEIGHT;
    width = WORLD_WIDTH;
    world_map = WorldMap();
    top_layer = std::vector<TopLayerData>();
    canvas = std::vector<std::vector<MapTile> >(height,
            std::vector<MapTile>(width));

    TopLayerData cursor = {5, 5, map_tile::CURSOR};
    top_layer.push_back(cursor);
    refresh();
}

void WorldMapGUI::new_world() {
    world_map = WorldMap();
    refresh();
}

void WorldMapGUI::refresh() {
    MapTileMatrix raw_map = world_map.get_map();
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            canvas[i][j] = raw_map[i][j];
        }
    }
    add_layers();
}

void WorldMapGUI::add_layers() {
    for(int i = 0; i < top_layer.size(); i++) {
        canvas[top_layer[i].row][top_layer[i].col] = top_layer[i].tile;
    }
}

void WorldMapGUI::move_cursor(int col_change, int row_change) {
    int new_row, new_col;
    TopLayerData* cursor = &top_layer[0];

    new_row = cursor->row + row_change;
    new_col = cursor->col + col_change;

    if(new_row >= 1 && new_row < (height - 1) &&
       new_col >= 1 && new_col < (width - 1)) {
        cursor->row = new_row;
        cursor->col = new_col;
    }
    refresh();
}

const MapTileMatrix& WorldMapGUI::get_map() {
    return world_map.get_map();
}

const MapTileMatrix& WorldMapGUI::get_canvas() {
    return canvas;
}

IntPoint WorldMapGUI::get_selected_chunk() {
    return IntPoint(top_layer[0].row, top_layer[0].col);
}

const WorldMap& WorldMapGUI::get_world_map()
{
    return world_map;
}
