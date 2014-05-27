/**
 *  CHUNK_MATRIX.CPP
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

#include "chunk_matrix.h"

ChunkMatrix::ChunkMatrix() {

}

ChunkMatrix::ChunkMatrix(int _diameter, IntPoint center_chunk, MapTileMatrix& world_map) {
    diameter = _diameter;
    assert(diameter%2 != 0);

    model = vector<vector<Chunk> >(diameter, vector<Chunk>(diameter));
    int offset_dist_from_center = (diameter - 1) / 2;

    offset.row = center_chunk.row - offset_dist_from_center;
    offset.col = center_chunk.col - offset_dist_from_center;

    populate_initial(center_chunk, world_map);
}

void ChunkMatrix::populate_initial(IntPoint center_chunk, MapTileMatrix& world_map) {
    int world_row, world_col;
    for(int row = 0; row < diameter; row++) {
        world_row = row + offset.row;
        for(int col = 0; col < diameter; col++) {
            world_col = col + offset.col;
            model[row][col].init(world_map[world_row][world_col],
                                world_row, world_col);
        }
    }
}

/**
 * Determines whether or not the given local coordinates on this small chunk map
 * are out of bounds.
 */
bool ChunkMatrix::out_of_bounds(IntPoint chunk_point) {
    return (chunk_point.row < 0 || chunk_point.row >= diameter ||
            chunk_point.col < 0 || chunk_point.col >= diameter);
}

/**
 * Will return the underlying data structure. Probably won't be used.
 */
vector<vector<Chunk> >& ChunkMatrix::get_matrix() {
    return model;
}

/**
 * Return an IntPoint representing the amount by which the small ChunkMatrix is
 * offset from the top left corner of the world.
 */
IntPoint ChunkMatrix::get_offset() {
    return offset;
}

/**
 * Set the offset to the given point.
 */
void ChunkMatrix::set_offset(IntPoint point) {
    offset = point;
}

/**
 * PRE: Will be given absolute chunk coordinates on the world map and a chunk to
 * copy.
 * POST: Will copy over the chunk if possible.
 */
void ChunkMatrix::set_chunk_abs(IntPoint abs_chunk_loc, Chunk data) {
    IntPoint localized = IntPoint(abs_chunk_loc.row - offset.row,
                                  abs_chunk_loc.col - offset.col);
    if(out_of_bounds(localized)) {
        cout<<"Absolute point at "<<abs_chunk_loc.row<<", "<<abs_chunk_loc.col
            <<" was out of bounds: Localized to "<<localized.row<<", "
            <<localized.col<<" (setter)";
    } else {
        model[localized.row][localized.col] = data;
    }
}

/**
 * PRE: Will be given the absolute chunk coordinates on the world map
 * POST: Will return a reference to the chunk from the local buffer.
 */
Chunk* ChunkMatrix::get_chunk_abs(IntPoint abs_chunk_loc) {
    IntPoint localized = IntPoint(abs_chunk_loc.row - offset.row,
                                  abs_chunk_loc.col - offset.col);
    if(out_of_bounds(localized)) {
        cout<<"Absolute point at "<<abs_chunk_loc.row<<", "<<abs_chunk_loc.col
            <<" was out of bounds: Localized to "<<localized.row<<", "
            <<localized.col<<endl;

        return &model[0][0];
    }

    return &model[localized.row][localized.col];
}

Chunk* ChunkMatrix::get_chunk_abs(int row, int col) {
    return get_chunk_abs(IntPoint(row, col));
}

/**
 * Copy chunk data to the given local location.
 */
void ChunkMatrix::set_chunk(IntPoint chunk_loc, Chunk data) {
    assert(!out_of_bounds(chunk_loc));
    model[chunk_loc.row][chunk_loc.col] = data;
}

/**
 * Return a reference to the chunk at the given location.
 */
Chunk* ChunkMatrix::get_chunk(IntPoint chunk_loc) {
    assert(!out_of_bounds(chunk_loc));
    return &model[chunk_loc.row][chunk_loc.col];
}

Chunk* ChunkMatrix::get_center_chunk() {
    int rowcol = (diameter - 1) / 2;
    cout<<"Center chunk = "<<rowcol<<endl;
    return &model[rowcol][rowcol];
}

/**
 * PRE: Will be given an IntPoint representing the direction to shift the
 * matrix: (1, 0) means one chunk to the right; (-1, -1) means one chunk to the
 * left and one chunk up; (0, -1) means 1 chunk up... etc. Additionally, will be
 * given a reference to the world map.
 *
 * POST: Will shift the chunk buffer around on the world map, agnostic of the
 * chunk buffer diameter. The concept of the chunk buffer is shown below.
 * Periods are unloaded chunks on the world map, and x's are chunks within the
 * chunk buffer (so, they are loaded). The buffer will move around, loading and
 * unloading chunks as needed. Serialization and deserialization should all be
 * handled in this function and the functions that it causes.
 *
 *    . . . . . .
 *    . . . . . .
 *    . . x x x .
 *    . . x x x .
 *    . . x x x .
 *    . . . . . .
 *
 */
void ChunkMatrix::shift_matrix(IntPoint directions, MapTileMatrix &world_map) {
    int world_row, world_col;

    if(directions.row == 1) {
        //For everything but the last row, we will just copy the next row into the
        //current row.
        for(int row = 0; row < (diameter - 1); row++) {
            for(int col = 0; col < diameter; col++) {
                model[row][col] = model[row + 1][col];
            }
        }

        assert((offset.row + (diameter - 1)) < WORLD_HEIGHT);
        //For the last row, we will first have to serialize and clear.
        int row = diameter - 1;
        for(int col = 0; col < diameter; col++) {
            model[row][col].serialize();
            model[row][col] = Chunk();
        }

        //Create or deserialize new chunks for the last row.
        world_row = offset.row + (diameter - 1);
        for(int col = 0; col < diameter; col++) {
            world_col = offset.col + col;
            model[row][col].init(world_map[world_row][world_col],
                                 world_row, world_col);
        }

    } else if (directions.row == -1) {
        //For everything but the first row, copy the previous row into the
        //current row.
        for(int row = 1; row < diameter; row++) {
            for(int col = 0; col < diameter; col++) {
                model[row][col] = model[row - 1][col];
            }
        }

        assert(offset.row > 0);
        //For the first row, serialize and clear.
        int row = 0;
        for(int col = 0; col < diameter; col++) {
            model[row][col].serialize();
            model[row][col] = Chunk();
        }

        //Create or deserialize new chunks for the first row.
        world_row = offset.row;
        for(int col = 0; col < diameter; col++) {
            world_col = offset.col + col;
            model[row][col].init(world_map[world_row][world_col],
                                 world_row, world_col);
        }
    }

    if(directions.col == 1) {
        //For everything but the rightmost column, copy the right neighbor into
        //the current column.
        for(int row = 0; row < diameter; row++) {
            for(int col = 0; col < (diameter - 1); col++) {
                model[row][col] = model[row][col + 1];
            }
        }

        assert((offset.col + (diameter - 1)) < WORLD_WIDTH);
        //For the last column, serialize and clear.
        int col = (diameter - 1);
        for(int row = 0; row < diameter; row++) {
            model[row][col].serialize();
            model[row][col] = Chunk();
        }

        //Create or deserialize new chunks for the last column.
        world_col = offset.col + (diameter - 1);
        for(int row = 0; row < diameter; row++) {
            world_row = offset.row + row;
            model[row][col].init(world_map[world_row][world_col],
                                 world_row, world_col);
        }
    } else if (directions.col == -1) {
        //For everything but the leftmost column, copy the left neighbor into
        //the current column.
        for(int row = 0; row < diameter; row++) {
            for(int col = 1; col < diameter; col++) {
                model[row][col] = model[row][col - 1];
            }
        }

        assert(offset.col > 0);
        //For the first column serialize and clear.
        int col = 0;
        for(int row = 0; row < diameter; row++) {
            model[row][col].serialize();
            model[row][col] = Chunk();
        }

        //Create or deserialize new chunks for the first column.
        world_col = offset.col;
        for(int row = 0; row < diameter; row++) {
            world_row = offset.row + row;
            model[row][col].init(world_map[world_row][world_col],
                                 world_row, world_col);
        }
    }

    offset.row += directions.row;
    offset.col += directions.col;
    cout<<"New offset: "<<offset.row<<", "<<offset.col<<endl;
}
