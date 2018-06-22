/**
 *  GAME.CPP
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

#include "game.h"

Item* Game::item_at_coords(IntPoint coords, IntPoint chunk, int depth) {

    vector<Item*>* item_list = chunk_map.get_chunk_abs(chunk.row,chunk.col)->get_items(depth);
    Item* temp_item = NULL;
    for(int i=0;i<item_list->size();i++) {

        if(item_list->at(i)->get_coords() == coords) {
            temp_item = item_list->at(i);
        }
    }
    return temp_item;
}

Chunk* Game::get_current_chunk() {
    return chunk_map.get_center_chunk();
    //return &chunk_map[main_char.get_chunk().row][main_char.get_chunk().col];
}

void Game::update_chunk_map(IntPoint shift_dir) {
    chunk_map.shift_matrix(shift_dir, world_map.get_map());
}

Plant* Game::get_plant(IntPoint chunk, IntPoint coords, int depth)
{
    Chunk* t_chunk = chunk_map.get_chunk_abs(chunk);
    return t_chunk->get_plant(coords, depth);
}

