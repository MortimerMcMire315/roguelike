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

void Game::run_spawners() {
    std::vector<Spawner>* spawners;
    Chunk* chunk;
    IntPoint chunk_coords;
    int accum = 0;
    for(int i=main_char.get_chunk().row-1;i<=main_char.get_chunk().row+1;i++) {
        for(int j=main_char.get_chunk().col-1;j<=main_char.get_chunk().col+1;j++) {
            chunk = chunk_map.get_chunk_abs(IntPoint(i, j));

            if(chunk->get_depth()>main_char.get_depth() && chunk->get_type().does_spawn) {
                chunk->run_spawners(main_char.get_depth());
                std::vector<Character*> charas = chunk->get_character_queue(main_char.get_depth());
                for(int index=0;index<charas.size();index++)
                {
                    add_character(charas[index], IntPoint(i, j));
                }
                chunk->clear_character_queue(main_char.get_depth());
            }
        }
    }
}

void Game::clear_character_queue()
{
    character_queue.clear();
}


std::vector<Character*> Game::flush_characters()
{
    return character_queue;
}

void Game::add_character(Character* character, IntPoint chunk)
{
    character->set_chunk(chunk);
    character_list.push_back(character);
    character_queue.push_back(character);
    character_to_index(character);
}
