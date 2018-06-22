/**
 *  @file WORLD_MAP.H
 *  @author Seth A. Yoder
 *
 *  @section LICENSE
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

#ifndef _WORLD_MAP_H
#define _WORLD_MAP_H

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "int_point.h"
#include "color_def.h"
#include "defs.h"
#include "constants.h"
#include "utility.h"
#include "city_district.h"

/**
 * A representation of the game's world map.
 * The world map is finite and represented by a two-dimensional array, like
 * literally everything else in the game. The map takes care of its own
 * generation in its constructor, then the map is passed to the outside world by
 * a call to get_map().
 */
class WorldMap {

    /**
     * A self-explanatory typedef for the base model for this class.
     */
    typedef std::vector<std::vector<MapTile> > MapTileMatrix;
    private:

        /**
         * The base model for this class. A two-dimensional matrix storing
         * MapTiles.
         *
         * @see MapTile
         */
        std::vector<std::vector<MapTile> > map;

        /**
         * Height, which is set to WORLD_HEIGHT in the constructor. Essentially
         * just makes the variable less verbose. May be removed.
         */
        int height;

        /**
         * Width, which is set to WORLD_WIDTH in the constructor. Essentially
         * just makes the variable less verbose. May be removed.
         */
        int width;

        /**
         * A list of settlements in the world.  This is being stored in the world_map
         * because it's part of the world generation.
         */
        std::vector<CityDistrict> settlements;

        /**
         * Counts the number of occurences of the given MapTile type in the 8
         * tiles surrounding the given point.
         *
         * @param row
         * @param col
         * @param tile_type - the type of MapTile to query for
         * @return an integer result of the query.
         */
        int count_in_surrounding_tiles(int row, int col, MapTile tile_type);

        /**
         * Counts the number of tiles in the 8 adjacent squares
         * which are not of type tile_type.
         *
         * @param row
         * @param col
         * @param tile_type - the type of MapTile to query for
         * @return an integer result of the query.
         */
        int count_not_surrounding_tiles(int row, int col, MapTile tile_type);

        /**
         * Fills the world map (minus the ocean border) with randomly-placed
         * tiles, then performs the following transformation to all tiles on the
         * map:
         * \verbatim
           ?O?     ?O?
           O?O --> OOO
           ?O?     ?O?
         * \endverbatim
         *
         * @param border - the width of the ocean border surrounding the map.
         */
        void starting_noise(int border);

        /**
         * Will loop through the world map. On each tile, if the number of
         * surrounding tiles of the given type is greater or equal to the given
         * "threshold", there is a 1/(8 - num_surrounding_tiles) chance that the
         * tile in question will be changed to tile_type. This function is
         * relatively portable, and could be ported to any number of other
         * situations. When applied sequentially with different thresholds (as
         * in generate_land_mass()), will produce a passable world.
         *
         * @param tile_type - the type of tile that will be changed.
         * @param threshold - The number of surrounding tiles must be greater
         * than this value for any change to occur.
         */
        void smoothing_pass(MapTile tile_type, int threshold);

        /**
         * Generates the main land mass in the water by calling smoothing_pass
         * in sequence with different parameters on both land and water tiles.
         */
        void generate_land_mass();

        /**
         * Will generate an ocean border with the given width around the map.
         * @param border - the desired width of the border.
         */
        void ocean_borders(int border);

        /**
         * Generates beaches at the borders of all land and water tiles.
         */
        void generate_beaches();

        /**
         * Turns generic areas of cities into cities with distinct
         * areas, defined the CITIES array in defs.cpp.
         */
        void seed_cities();

        /**
         * Takes an unorder set of intpoints that correspond to
         * a contiguous city region, and turns it into a two
         * dimensional ordered vector of IntPoints which is
         * a rectangle, i.e. all of the vectors are the same
         * length.  Any tiles which are not cities are designated
         * as IntPoint(-1, -1).
         */
        std::vector<std::vector<IntPoint> > sort_city(std::vector<IntPoint> city);

        /**
         * Generates a city based on an ordered two dimensional
         * vector of IntPoints, generated from sort_city.  This
         * overlays the districts one at a time, in the order
         * that they are defined in defs.cpp, with each layer
         * generated by flooding out from a random edge point.
         */
        void generate_city(std::vector<std::vector<IntPoint> > city, int city_size);


        /**
         * Takes in a 2d vector of IntPoints corresponding to a
         * city (padded with IntPoint(-1, -1)) and returns a
         * random edge of the city.
         */
         IntPoint get_seed(std::vector<std::vector<IntPoint> > city);

         /**
          * Takes in a 2d vector of IntPoints corresponding to a
          * city, the tile to flood outward, and the number
          * of tiles left to flood outward, and the IntPoint
          * of the current tile (in terms of coords of the city
          * passed in) and flips all adjacent tiles to that kind
          * until it runs out.
          */
         void random_flood(std::vector<std::vector<IntPoint> >& flood_map, MapTile tile, int remaining, IntPoint cur_tile);

    public:
        /**
         * The main constructor, which also generates the world's continents.
         */
        WorldMap();

        /**
         * Returns a reference to the world map.
         * @return a reference to the world map.
         */
        std::vector<std::vector<MapTile> >& get_map();

        /**
         * Determines whether a given row-column pair is out of bounds on the
         * world map matrix.
         *
         * @param row
         * @param col
         * @return a boolean denoting whether the point is out of bounds.
         */
        bool out_of_bounds(int row, int col);

        /**
         * Returns the tile at a given location.
         */
        MapTile tile_at(int row, int col);

        /**
         * Gets a list of lists of points that make up
         * contiguous regions for the given target
         * tile type.
         */
        std::vector<std::vector<IntPoint> > find_contiguous(MapTile target);

        /**
         * Given a starting tile, it will check every
         * tile around it, and add any to the a list
         * of contiguous tiles, recursively calling
         * this function on every one of those tiles.
         */
        void flood(IntPoint start_point, std::vector<IntPoint>& closed, std::vector<IntPoint>& cur_contig, MapTile target);

        /**
         * Gets a random tile based on the weights that
         * they  have been assigned in the defs.cpp.
         */
        MapTile random_weighted_tile();
};

#endif
