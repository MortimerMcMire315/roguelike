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

void Game::point_assertions(int row, int col) {
    assert(row >= 0);
    assert(row < SCREEN_HEIGHT);
    assert(col >= 0);
    assert(col < SCREEN_WIDTH);
}

void Game::set_tile(int row, int col, Tile* tile) {
    point_assertions(row, col);
    canvas[row][col] = tile;
}

void Game::set_tile(IntPoint point, Tile* tile) {
    point_assertions(point.row, point.col);
    canvas[point.row][point.col] = tile;
}

Tile* Game::get_tile(int row, int col) {
    point_assertions(row, col);
    return canvas[row][col];
}

Tile* Game::get_tile(IntPoint point) {
    point_assertions(point.row, point.col);
    return canvas[point.row][point.col];
}

bool Game::out_of_bounds(IntPoint point) {
    return out_of_bounds(point.row, point.col);
}

bool Game::out_of_bounds(int row, int col) {
    return (col < 0 || col >= GAME_WIDTH ||
            row < 0 || row >= GAME_HEIGHT);
}

const std::vector<std::vector<Tile*> >& Game::get_canvas() {
    return canvas;
}


std::vector<Character*> Game::get_vis_characters() {
    std::vector<Character*> temp = std::vector<Character*>();
    for(int i=0;i<character_list.size();i++) {
        IntPoint chunk = IntPoint(character_list[i]->get_chunk_y(), character_list[i]->get_chunk_x());
        IntPoint coords = IntPoint(character_list[i]->get_y(), character_list[i]->get_x());
        IntPoint main_char_coords = IntPoint(main_char.get_y(), main_char.get_x());
        IntPoint radius  = IntPoint(GAME_HEIGHT/2, GAME_WIDTH/2);
        if(utility::in_range(chunk, coords, main_char.get_chunk(), main_char_coords, radius) &&
                character_list[i]->get_depth() == main_char.get_depth()) {
            temp.push_back(character_list[i]);
        }
    }
    return temp;
}


bool Game::is_vis(IntPoint coords)
{
    bool y = coords.row < GAME_HEIGHT && coords.row >= 0;
    bool x = coords.col < GAME_WIDTH && coords.col >=0;
    return x && y;
}

IntPoint Game::get_canvas_coords(IntPoint chunk, IntPoint coords){
    IntPoint tl_abs = utility::get_abs(main_char.get_chunk(),
               IntPoint(main_char.get_y() - CHUNK_HEIGHT/2, main_char.get_x() - CHUNK_WIDTH/2));
    IntPoint abs = utility::get_abs(chunk, coords);
    return IntPoint(abs.row - tl_abs.row, abs.col - tl_abs.col);
}
