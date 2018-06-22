/**
 *  @file CHUNK_MATRIX.H
 *  @author Seth Yoder
 *
 *  @section  LICENSE
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

#ifndef _CHUNK_MATRIX_H
#define _CHUNK_MATRIX_H

#include <vector>

#include "int_point.h"
#include "world_map.h"
#include "chunk.h"


/**
 * The primary data model and memory-loading bottleneck.
 *
 * The Chunk Matrix is a square matrix with a diameter that is given at
 * construction. The matrix is initialized using populate_initial, given central
 * chunk coordinates on the world map and a reference to the world map.
 *
 * For each of those chunks, that function calls the init() method, which will
 * by default construct the chunks if they haven't already been saved to the
 * filesystem or load them from the filesystem if they have.
 *
 * Every time the main character crosses a chunk boundary, shift_matrix will be
 * called. The chunk matrix will load in the proper chunks, creating or
 * deserializing chunks as needed.
 */

using namespace std;

class ChunkMatrix
{
    typedef std::vector<std::vector<MapTile> > MapTileMatrix;
    private:
        /**
         * The diameter of this matrix.
         */
        int diameter;

        /**
         * The base model where chunks are stored in a (diameter)*(diameter)
         * two-dimensional vector.
         */
        vector<vector<Chunk> > model;

        /**
         * The amount by which the top left corner of the matrix is offset from
         * that of the world.
         */
        IntPoint offset;

        string save_folder;


    public:
        ChunkMatrix();
        ChunkMatrix(int _diameter, IntPoint center_chunk, MapTileMatrix &world_map, string _save_folder);

        /**
         * Populates the chunk matrix initially, affecting the internal model.
         * @param[in] center_chunk An IntPoint representing the central chunk in
         *                         the matrix
         * @param[in] world_map The world map, passed by reference here to avoid
         *                      copying.
         */
        void populate_initial(IntPoint center_chunk, MapTileMatrix &world_map);


        /**
         * Prints a graphic of the chunk matrix and its coordinates.
         */
        void pretty_print();

        /**
         * Determines whether or not the given local coordinates on this small chunk map
         * are out of bounds.
         * @param[in] chunk_point an IntPoint representing a chunk's location on the
         * private "model" matrix.
         * @return True if the given point cannot lie on the chunk map.
         */
        bool out_of_bounds(IntPoint local_chunk_point);

        /**
         * @return an IntPoint representing the amount by which the top left corner of
         * the ChunkMatrix is offset from the top left corner of the world.
         */
        IntPoint get_offset();

        /**
         * Set the offset (amount by which the top left corner of the ChunkMatrix
         * differs from the world's top left corner).
         * @param[in] point a point representing a location on the world map.
         */
        void set_offset(IntPoint point);

        /**
         * @param abs_chunk_loc the absolute chunk coordinates on the world map.
         * @return a reference to the chunk from the local buffer.
         */
        Chunk* get_chunk_abs(IntPoint abs_chunk_loc);

        /**
         * The same as the other get_chunk_abs, but with two ints instead of an
         * IntPoint.
         */
        Chunk* get_chunk_abs(int row, int col);

        /**
         * TODO update more code to use this instead of get_chunk_abs(main_char.get_y(),
         * main_char.get_x())
         * @return the central chunk in the model.
         */
        Chunk* get_center_chunk();

        /**
         * Gets the coordinates of the center of the matrix.
         */
        IntPoint get_center();

        /**
         * Shifts the contents of the chunk matrix.
         *
         * Will shift the chunk buffer around on the world map, agnostic of the
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
         * @param directions an IntPoint representing the direction to shift the matrix:
         * (1, 0) means one chunk to the right; (-1, -1) means one chunk to the left and
         * one chunk up; (0, -1) means 1 chunk up, etc.
         * @param world_map a reference to the world map.
         *
         */
        void shift_matrix(IntPoint directions, MapTileMatrix &world_map);

        /**
         * Serializes all the chunks in the chunk_map.
         */
        void serialize_all();

        /**
         * Public accessor for the diameter.
         */
        int get_diameter();

};

#endif
