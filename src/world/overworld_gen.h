/**
 *  @file OVERWORLD.H
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

#ifndef _OVERWORLD_H
#define _OVERWORLD_H

#include <vector>
#include <iostream>

#include "defs.h"

class ChunkLayer;
class Chunk;

typedef std::vector<Chunk*> ChunkPtrList;

namespace overworld_gen {
    typedef std::vector<std::vector<Tile> > TileMatrix;
    typedef std::vector<std::vector<std::vector<int> >* > HeightmapPtrList;
    bool smoothing_pass(int threshold, Tile tile_type, int num);

    /**
     * Constructs an overworld of "land" type on this chunk. "Land" has dirt
     * and some trees.
     */
    void build_land_overworld(ChunkLayer& ground);

    /**
     * Constructs an ocean on this chunk. Every tile becomees a water tile.
     */
    void build_water_overworld(ChunkLayer& ground);

    /**
     * Constructs a beach on this chunk. Every tile becomes a type of sand
     * tile.
     */
    void build_beach_overworld(ChunkLayer& ground);

    /**
     * Constructs a forest on this chunk. Forests have dark grass floors,
     * thick trees, and tree-lined oases.
     */
    void build_forest_overworld(ChunkLayer& ground);

    /**
     * Constructs a city object in this chunk.  For now, this will
     * only add buildings to the chunk, there won't be an actual
     * city object.
     */
    void build_city_overworld(ChunkLayer& ground);

    /**
     * Build the edges of this chunk's topmost layer based on the edges of the
     * chunks surrounding it.
     */
    void seed_edges(Chunk& ground, int turbidity, HeightmapPtrList surrounding_chunks);

    /**
     * Vary the terrain on this chunk.
     *
     * @param c this chunk
     * @param turbidity parameter that affects hill steepness
     * @param surrounding_chunks a vector of the 8 chunks surrounding
     *        this one, starting from the top-left and going clockwise.
     */
    void create_hills(Chunk& c, int turbidity, ChunkPtrList surrounding_chunks);

}

#endif
