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

/*
 * PRE: Takes an x and a y coordinate (chunk).
 * POST: Returns whether or not the chunk is currently in the buffer.
 */
 //umm...this operates on two different coordinate systems, fyi.  It will
 //almost never work -MJY 2/2/15
bool Game::chunk_in_buffer(int row, int col) {
    IntPoint center = chunk_map.get_center();
    int offset = (chunk_map.get_diameter() - 1)/2;
    IntPoint buffer_chunk = IntPoint(center.row - row + offset, center.col - col + offset);
    return !chunk_map.out_of_bounds(buffer_chunk);
}


bool Game::coords_in_buffer(int row, int col)
{
    bool is_in = row < buffer.size() && row >= 0 && col < buffer[row].size() && col >= 0;
    return is_in;
}

bool Game::point_in_buffer(IntPoint chunk, IntPoint coords)
{
    IntPoint buffer_coords = get_buffer_coords(chunk, coords);
    return coords_in_buffer(buffer_coords.row, buffer_coords.col);
}


IntPoint Game::get_buffer_coords(IntPoint chunk, IntPoint coords) {
    IntPoint tl_buffer = utility::get_abs(IntPoint(main_char.get_chunk().row-1, main_char.get_chunk().col-1), IntPoint(0, 0));
    IntPoint abs = utility::get_abs(chunk, coords);
    return IntPoint(abs.row - tl_buffer.row, abs.col - tl_buffer.col);
}

void Game::add_tile_to_buffer(IntPoint chunk, IntPoint coords, Tile* tile)
{
    IntPoint buffer_coords = get_buffer_coords(chunk, coords);
    if(coords_in_buffer(buffer_coords.row, buffer_coords.col))
    {
        buffer[buffer_coords.row][buffer_coords.col] = tile;
    }
}


/*
Here, we update the buffer based on the chunks surrounding the character's
current chunk.  This should be broken into several functions.  This will be
called whenever the character moves into a new chunk, so that the buffer
reflects the chunks surrounding the characters current one.
*/
void Game::update_buffer(IntPoint central_chunk) {
    int x, y;
    Chunk* current_chunk;
    Tile* buffer_tile;

    for(int row=central_chunk.row - 1;row<=central_chunk.row+1;row++) {

        for(int col=central_chunk.col-1;col<=central_chunk.col+1;col++) {
            x = col - (central_chunk.col - 1);
            y = row - (central_chunk.row - 1);
            //cout<<"Central chunk: "<<central_chunk.row<<" "<<central_chunk.col<<endl;
            current_chunk = chunk_map.get_chunk_abs(row, col);

            for (int a=0; a<CHUNK_HEIGHT; a++) {
                for (int b=0; b<CHUNK_WIDTH; b++) {
                    /**
                     *  This part is a bit confusing.  What I need is to write
                     *  the contents of the chunk to the appropriate place in
                     *  the buffer.  A and B represent the Y and X of
                     *  individual tiles.  So, for each chunk, the X and Y are
                     *  written to the buffer.  The chunks that we're iterating
                     *  through are essentially a 3x3 array.  Each chunk needs
                     *  to start being written at the appropriate location
                     *  (e.g. the second chunk needs to start where the first
                     *  one left off...), which is where the x and y variables
                     *  come in.  Starting at 0, the x and y are multiplied by
                     *  the width/height of the chunk, which is added to a and
                     *  b to get where the tile from the chunk corresponding to
                     *  a and b should be written to the buffer.  For example:
                     *  The first chunk should start writing tiles to the
                     *  buffer at 0,0.  The second chunk should start writing
                     *  tiles at 0 + CHUNK_WIDTH, and the third tile should
                     *  start writing at 0 + (CHUNK_WIDTH * 2).
                     */

                    int buffer_col = b + (x * CHUNK_WIDTH);
                    int buffer_row = a + (y * CHUNK_HEIGHT);

                    if(!current_chunk->out_of_bounds(main_char.get_depth(), a, b)) {
                        buffer_tile = current_chunk->get_tile(main_char.get_depth(), a, b);
                    } else {
                        buffer_tile = &buffer_tile_placeholder;
                    }
                    buffer[buffer_row][buffer_col] = buffer_tile;
                }
            }
        }
    }
    show_chunk_objects();
    update_character_index();
    refresh();
}

/**
 * There are two ways that things get rendered.  The first is that
 * they get written to the buffer.  The second is that they get passed
 * into the rendering function.  Essentially: things that move should
 * get passed into gui_render, because they might change every time.
 * Things that don't move can be written to the buffer.
 */
void Game::show_chunk_objects() {
    std::vector<Spawner>* spawners;
    std::vector<Item*>* items;
    std::vector<Plant>* plants;
    std::vector<Building>* buildings;
    Chunk* chunk;
    IntPoint chunk_coords;
    for(int i=main_char.get_chunk().row-1;i<=main_char.get_chunk().row+1;i++) {
        for(int j=main_char.get_chunk().col-1;j<=main_char.get_chunk().col+1;j++) {

           chunk = chunk_map.get_chunk_abs(IntPoint(i, j));
            IntPoint chunk_coords = IntPoint(i, j);

            if(chunk->get_depth()>main_char.get_depth()) {
                spawners = chunk->get_spawners(main_char.get_depth());
                items    = chunk->get_items(main_char.get_depth());
                plants   = chunk->get_plants(main_char.get_depth());
                buildings = chunk->get_buildings(main_char.get_depth());
                plants_to_buffer(plants, chunk_coords);
                items_to_buffer(items, chunk_coords);
                spawners_to_buffer(spawners, chunk_coords);
                buildings_to_buffer(buildings, chunk_coords);
            }
        }
    }
}


void Game::items_to_buffer(std::vector<Item*>* items, IntPoint chunk)
{
    for(int i=0;i<items->size();i++)
    {
        add_tile_to_buffer(chunk, items->at(i)->get_coords(), items->at(i)->get_sprite());
    }
}

void Game::spawners_to_buffer(std::vector<Spawner>* spawners, IntPoint chunk)
{
    for(int index=0;index<spawners->size();index++)
    {
        Spawner* spawner = &spawners->at(index);
        std::vector<Den> dens = spawner->get_spawn_points();
        for(int i=0;i<dens.size();i++)
        {
            Den* den = spawner->get_spawn_at(i);
            IntPoint coords = IntPoint(spawner->get_y() + den->get_y(), spawner->get_x() + den->get_x());
            den_to_buffer(den, chunk, coords);
        }
    }
}

void Game::plants_to_buffer(std::vector<Plant>* plants, IntPoint chunk)
{
    for(int i=0;i<plants->size();i++)
    {
        TileMatrix* plant_sprites = plants->at(i).get_sprites();
        for(int j=0;j<plant_sprites->size();j++)
        {
            for(int k=0;k<plant_sprites->at(j).size();k++)
            {
                IntPoint coords = plants->at(i).get_coords() + IntPoint(j, k);
                add_tile_to_buffer(chunk, coords, plants->at(i).get_sprite(j, k));
            }
        }
    }
}

void Game::den_to_buffer(Den* den, IntPoint chunk, IntPoint coords)
{
    std::unordered_map<std::string, Tile>* tileset = &Tileset::instance()->get_tileset();
    for(int i=0;i<den->get_height();i++)
    {
        for(int j=0;j<den->get_width();j++)
        {
            if(den->tile_at(i, j) != (*tileset)["EMPTY"])
            {
                IntPoint p_coords = coords + IntPoint(i - den->get_height()/2, j - den->get_width()/2);
                add_tile_to_buffer(chunk, p_coords, den->tile_pointer_at(i, j));
            }
        }
    }
}

void Game::buildings_to_buffer(std::vector<Building>* buildings, IntPoint chunk)
{
    for(int i=0;i<buildings->size();i++)
    {
        for(int j=0;j<buildings->at(i).get_height();j++)
        {
            for(int k=0;k<buildings->at(i).get_width();k++)
            {
                int y = j + buildings->at(i).get_y();
                int x = k + buildings->at(i).get_x();
                Tile* chara = buildings->at(i).tile_pointer_at(j, k);
                add_tile_to_buffer(chunk, IntPoint(y, x), chara);
            }
        }
    }
}
