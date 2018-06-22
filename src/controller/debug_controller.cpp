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

void Game::spawn_enemy(int chunk_x, int chunk_y, int x, int y, int depth, int type) {
        Enemy* temp = new Enemy(x, y, depth, ENEMY_LIST[type]);
        temp->set_chunk(IntPoint(chunk_y, chunk_x));
        character_queue.push_back(temp);
        character_list.push_back(temp);
        character_to_index(temp);
}

void Game::teleport(int chunk_x, int chunk_y, int x, int y)
{
    main_char.set_x(x);
    main_char.set_y(y);
    main_char.set_chunk(IntPoint(chunk_y, chunk_x));
    update_buffer(main_char.get_chunk());
    unsigned int delta_x = chunk_x - main_char.get_chunk().col;
    unsigned int delta_y = chunk_y - main_char.get_chunk().row;
    if(delta_x > 1 || delta_y > 1)
    {
        //chunk_map.serialize_all();
        chunk_map.populate_initial(main_char.get_chunk(), world_map.get_map());
    }
    else
    {
        update_chunk_map(IntPoint(delta_y, delta_x));
    }

}

