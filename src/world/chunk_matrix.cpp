/**
 *  @file CHUNK_MATRIX.CPP
 *  @author Seth Yoder
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

#include <stdlib.h>

#include "chunk_matrix.h"
#include "constants.h"


ChunkMatrix::ChunkMatrix() {

}

ChunkMatrix::ChunkMatrix(int _diameter, IntPoint center_chunk, MapTileMatrix& world_map, string _save_folder) {
    save_folder = _save_folder;
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
                                world_row, world_col, save_folder, world_map);
        }
    }
    pretty_print();
}

void ChunkMatrix::pretty_print() {
    for(int row = 0; row < diameter; row++) {
        cout<<endl<<"----------------------------"<<endl;
        for(int col = 0; col<diameter; col++) {
            cout<<"|"<<model[row][col].get_world_loc();
        }
        cout<<"|";
    }
    cout<<endl<<"----------------------------"<<endl;
}

bool ChunkMatrix::out_of_bounds(IntPoint local_chunk_point) {
    return (local_chunk_point.row < 0 || local_chunk_point.row >= diameter ||
            local_chunk_point.col < 0 || local_chunk_point.col >= diameter);
}

IntPoint ChunkMatrix::get_offset() {
    return offset;
}

void ChunkMatrix::set_offset(IntPoint point) {
    offset = point;
}


Chunk* ChunkMatrix::get_chunk_abs(IntPoint abs_chunk_loc) {
    IntPoint localized = IntPoint(abs_chunk_loc.row - offset.row,
                                  abs_chunk_loc.col - offset.col);
    if(out_of_bounds(localized)) {
        /*
        cout<<"Absolute point at "<<abs_chunk_loc.row<<", "<<abs_chunk_loc.col
            <<" was out of bounds: Localized to "<<localized.row<<", "
            <<localized.col<<endl<<"     offset = "<<offset<<endl;
            */

        return &model[0][0];
    }

    return &model[localized.row][localized.col];
}

Chunk* ChunkMatrix::get_chunk_abs(int row, int col) {
    return get_chunk_abs(IntPoint(row, col));
}

Chunk* ChunkMatrix::get_center_chunk() {
    int rowcol = (diameter - 1) / 2;
    //Umm...if diameter is 3, this will give you 1, not 2
    //If you actualy want the center, add 1 instead of subtracting
    return &model[rowcol][rowcol];
}

IntPoint ChunkMatrix::get_center()
{
    int rowcol = (diameter - 1) / 2;
    Chunk* temp = &model[rowcol][rowcol];
    return temp->get_world_loc();
}


void ChunkMatrix::shift_matrix(IntPoint directions, MapTileMatrix &world_map) {
    int world_row, world_col;

    if(directions.row == 1) {
        assert((offset.row + (diameter - 1)) < WORLD_HEIGHT);
        //For the topmost row, we will first have to serialize and clear.
        /*
        for(int col = 0; col < diameter; col++) {
            model[0][col].serialize();
        }
        */

        //For everything but the last row, we will just copy the next row into the
        //current row.
        for(int row = 0; row < (diameter - 1); row++) {
            for(int col = 0; col < diameter; col++) {
                model[row][col] = model[row + 1][col];
            }
        }

        //Create or deserialize new chunks for the last row.
        int row = diameter - 1;
        world_row = offset.row + diameter;
        for(int col = 0; col < diameter; col++) {
            world_col = offset.col + col;
            model[row][col].init(world_map[world_row][world_col],
                                 world_row, world_col, save_folder, world_map);
        }

    } else if (directions.row == -1) {
        assert(offset.row > 0);
        //For the last row, serialize.
        int row = diameter - 1;
        /*
        for(int col = 0; col < diameter; col++) {
            model[row][col].serialize();
        }
        */

        //For everything but the first row, copy the previous row into the
        //current row.
        for(int row = (diameter - 1); row > 0; row--) {
            for(int col = 0; col < diameter; col++) {
                model[row][col] = model[row - 1][col];
            }
        }

        //Create or deserialize new chunks for the first row.
        world_row = offset.row - 1;
        for(int col = 0; col < diameter; col++) {
            world_col = offset.col + col;
            model[0][col].init(world_map[world_row][world_col],
                                 world_row, world_col, save_folder, world_map);
        }
    }

    if(directions.col == 1) {
        assert((offset.col + (diameter - 1)) < WORLD_WIDTH);

        //For the leftmost column, serialize and clear.
        /*
        for(int row = 0; row < diameter; row++) {
            model[row][0].serialize();
        }
        */

        //For everything but the rightmost column, copy the right neighbor into
        //the current column.
        for(int row = 0; row < diameter; row++) {
            for(int col = 0; col < (diameter - 1); col++) {
                model[row][col] = model[row][col + 1];
            }
        }

        //Create or deserialize new chunks for the last column.
        int col = (diameter - 1);
        world_col = offset.col + diameter;
        for(int row = 0; row < diameter; row++) {
            world_row = offset.row + row;
            model[row][col].init(world_map[world_row][world_col],
                                 world_row, world_col, save_folder, world_map);
        }
    } else if (directions.col == -1) {
        assert(offset.col > 0);

        int col = diameter - 1;
        //For the rightmost column, serialize and clear.
        /*
        for(int row = 0; row < diameter; row++) {
            model[row][col].serialize();
        }
        */

        //For everything but the leftmost column, copy the left neighbor into
        //the current column.
        for(int row = 0; row < diameter; row++) {
            for(int col = (diameter - 1); col > 0; col--) {
                model[row][col] = model[row][col - 1];
            }
        }

        //Create or deserialize new chunks for the first column.
        world_col = offset.col - 1;
        for(int row = 0; row < diameter; row++) {
            world_row = offset.row + row;
            model[row][0].init(world_map[world_row][world_col],
                                 world_row, world_col, save_folder, world_map);
        }
    }

    offset.row += directions.row;
    offset.col += directions.col;
    cout<<"New offset: "<<offset.row<<", "<<offset.col<<endl;
}

/*
void ChunkMatrix::serialize_all()
{
    for(int i=0;i<diameter;i++)
    {
        for(int j=0;j<diameter;j++)
        {
            model[i][j].serialize();
        }
    }
}
*/


int ChunkMatrix::get_diameter()
{
    return diameter;
}
