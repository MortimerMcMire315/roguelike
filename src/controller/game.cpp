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
#include <iostream>
#include <assert.h>
#include <stdlib.h>

#include "game.h"

using namespace std;

/*
Explanation of how the screen works: There is a chunk map, which holds all of
the chunks.  We need a way to go from chunk->screen.  However, since we always
want the character to be centered, there will be times when the screen consists
of half of one chunk and half of another.

Let's say that this is an array of chunks:

111222333
111222333
111222333

and the screen is 3x3, to give us a view of:

222
2X2
222

with X being the character.  If the character moves left, then we get:

122
1X2
122

So, there is a buffer, which essentially holds all of the surrounding chunks.
A buffer might look like:

123
456
789

Where each number is a different chunk.  But, we only want part of this to get
written to the screen.  So, we have a canvas, which is the size of the screen
taken from the buffer with the character at the center.

*/

/* ========= PUBLIC METHODS ==========*/

/*--------------------Base Model Functions--------------------------*/

Game::Game() {
    initialized = false;
    paused = false;
}

Game::~Game()
{
    for(int i=0;i<character_list.size();i++)
    {
        delete character_list[i];
    }
}


void Game::init(const WorldMap& _world_map, IntPoint selected_chunk) {
    world_map = _world_map;

    //Give me a buffer size of 150x300 (tiles, which are 8x16 pixels)
    //The buffer is what the screen draws from.
    buffer = TilePointerMatrix(CHUNK_HEIGHT * 3, vector<Tile*>(CHUNK_WIDTH * 3));
    character_index = std::vector<std::vector<Character*> >(CHUNK_HEIGHT * 3, std::vector<Character*>(CHUNK_WIDTH * 3));
    clear_character_index();

    //Each chunk holds an overworld and several
    //dungeons, which are generated upon chunk creation.
    //This is the "starting" chunk (arbitrary).

    //TODO put this clutter somewhere else
    int main_stat_array[NUM_STATS] = {100, 2, 100, 0, 1, 10, 10, 10};
    std::vector<int> main_stats(&main_stat_array[0], &main_stat_array[0] + NUM_STATS);
    main_char = Character(main_stats, 50, 25, Tileset::get("MAIN_CHAR"), misc::player_corpse, selected_chunk.col, selected_chunk.row, 0, 0, 70, -1, "You!", weapons::fist);
    main_char.add_item(new Consumable(main_char.get_chunk(), consumables::potato));
    main_char.add_item(new Weapon(main_char.get_chunk(), weapons::wood_axe));
    Enemy* wolf = new Enemy(49, 25, 0, enemies::wolf_companion);
    wolf->set_master(&main_char);
    wolf->set_chunk(main_char.get_chunk());

    character_list.push_back(wolf);
    character_queue.push_back(wolf);
    character_to_index(wolf);

    IntPoint buffer_coords = get_buffer_coords(main_char.get_chunk(), main_char.get_coords());
    character_index[buffer_coords.row][buffer_coords.col] = &main_char;

    // Out of bounds tiles look like a generic BLOCK_WALL.
    buffer_tile_placeholder=Tileset::get("BLOCK_WALL");


    //What gets drawn to the screen
    canvas = TilePointerMatrix(GAME_HEIGHT, vector<Tile*>(GAME_WIDTH));

    //Eventually, this should be based on screen size.
    chunk_map = ChunkMatrix(3, selected_chunk, world_map.get_map(), name);
    update_buffer(main_char.get_chunk());
    recalculate_visibility_lines(15);
    refresh();

    visibility_on = true;
    initialized = true;

}


string Game::get_name() {
    return name;
}

void Game::set_name(string n) {
    name = n;
}

bool Game::is_initialized() {
    return initialized;
}


bool Game::is_paused() {
    return paused;
}

void Game::pause() {
    paused = true;
}

void Game::toggle_pause() {
    paused = !paused;
}


void Game::unpause() {
    paused = false;
}

void Game::act(long delta_ms) {
    tick_animations(delta_ms);
    main_char.act(delta_ms);
}

/*
 * Updates the canvas with the area around the character in terms of buffer coordinates.
 * Draws visibility lines.
 * This is to refresh the screen whenever the character moves.
 */
void Game::refresh() {
    for(int i = 0; i < GAME_HEIGHT; i++) {
        for (int j = 0; j < GAME_WIDTH; j++) {
            //aight, this needs some comments becuase ain't nobody can
            //read this

            //if you suddenly start getting weird visual errors when you
            //try to make the buffer hold more than 3 chunks at a time,
            //this is why.  Right now, it assumes that you're always in
            //the middle chunk of 3 (+ 1 chunk_height)

            //Explanation for the following:
            //Here is the buffer:
            //123
            //456
            //789
            //You are always in chunk 5, because it updates when you move.
            //Therefore, you are always + 1 chunk_size (height or width)
            //relative to the top  of the buffer.
            //To get the screens coordinates, you add 1 chunk_size to
            //your coords, then subtract half of the screen that you want
            //to display, and then add the index.
            int buffer_tile_row = (CHUNK_HEIGHT + main_char.get_y()) -
                (GAME_HEIGHT/2) + i;
            int buffer_tile_col = (CHUNK_WIDTH + main_char.get_x()) -
                (GAME_WIDTH/2) + j;
            set_tile(i, j, buffer[buffer_tile_row][buffer_tile_col]);
        }
    }
    draw_visibility_lines();
}



//--------------------VISIBILITY STUFF-----------------------//

/**
 * PRE: Will be given :int size:, the radius of the FOV circle
 * POST: Will set bresenham_lines, a vector of IntPoint vectors containing
 * points for raytraced lines.
 */
void Game::recalculate_visibility_lines(int size) {
    IntPoint true_center = IntPoint(0, 0);
    std::vector<IntPoint> circle_points = bresenham_circle(true_center, size);
    std::vector<IntPoint> smaller_circle = bresenham_circle(true_center, size - 1);

    for(int i = 0; i < smaller_circle.size(); i++) {
        circle_points.push_back(smaller_circle[i]);
    }

    std::vector<IntPoint> line_points;

    for(size_t i = 0; i < circle_points.size(); i++) {
        line_points = bresenham_line(true_center, circle_points[i]);
        bresenham_lines.push_back(line_points);
    }
}




/*
 * PRE: bresenham_lines should already be set by recalculate_visibility_lines().
 * POST: Draws a field-of-vision around the player - sets tiles' visibility
 * to true if they have been seen by the player.
 */
void Game::draw_visibility_lines() {
    IntPoint m_char = IntPoint(GAME_HEIGHT/2, GAME_WIDTH/2);
    Tile* current_chunk_tile;
    IntPoint current_point;
    int row, col;

    for(size_t i = 0; i < bresenham_lines.size(); i++) {
        for(size_t j = 1; j < bresenham_lines[i].size(); j++) {
            current_point = bresenham_lines[i][j];
            row = current_point.row + m_char.row;
            col = current_point.col + m_char.col;

            if(!out_of_bounds(IntPoint(row, col))) {
                current_chunk_tile = get_tile(row, col);
                current_chunk_tile->visible = true;
                current_chunk_tile->seen = true;
                if(current_chunk_tile->opaque) {
                    break;
                }
            }
        }
    }
}

void Game::undo_visibility() {
    IntPoint m_char = IntPoint(GAME_HEIGHT/2, GAME_WIDTH/2);
    Tile* current_chunk_tile;
    IntPoint current_point;
    int row, col;

    for(size_t i = 0; i < bresenham_lines.size(); i++) {
        for(size_t j = 1; j < bresenham_lines[i].size(); j++) {
            current_point = bresenham_lines[i][j];
            row = current_point.row + m_char.row;
            col = current_point.col + m_char.col;

            if(!out_of_bounds(IntPoint(row, col))) {
                current_chunk_tile = get_tile(row, col);
                current_chunk_tile->visible = false;
                current_chunk_tile->seen = true;
                /* *current_chunk_tile = ROOM_WALL;
                 *current_chunk_tile->visible = true;
                 *current_chunk_tile->can_be_moved_through = true;
                 */
                if(current_chunk_tile->opaque) {
                    break;
                }
            }
        }
    }
}


