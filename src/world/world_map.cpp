/**
 *  WORLD_MAP.CPP
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

#include <world_map.h>

using namespace std;

WorldMap::WorldMap() {
    srand(time(NULL));
    height = WORLD_HEIGHT;
    width = WORLD_WIDTH;
    map = std::vector<std::vector<MapTile> >(height,
            std::vector<MapTile>(width));

    generate_land_mass();
    generate_beaches();
}

const std::vector<std::vector<MapTile> >& WorldMap::get_map() {
    return map;
}

/**
 * PRE: Will be given two integers representing a row and a column.
 * POST: Will determine if that point is out of bounds on the world map.
 */
bool WorldMap::out_of_bounds(int row, int col) {
    return (row < 0 ||
            row >= height ||
            col < 0 ||
            col >= width);
}

/**
 * PRE: Will be given :int row:, :int col:, :int mod:, and :bool more_water:.
 * Row and column are self-explanatory. :int mod: represents the modulus for
 * rand(), or the inverse chance that the given tile will be turned into water.
 *
 * POST: If :more_water: is true, then the given tile will possibly be turned
 * into water, and vice versa. :int mod: is used to decide how likely it is that
 * the tile will change.
 */
void WorldMap::set_land_or_water(int row, int col,
                                 int mod, bool more_water) {
    if(more_water) {
        if(rand() % mod == 0) {
            map[row][col] = map_tile::MAP_FOREST;
        } else {
            map[row][col] = map_tile::MAP_WATER;
        }
    } else {
        if(rand() % mod == 0) {
            map[row][col] = map_tile::MAP_WATER;
        } else {
            map[row][col] = map_tile::MAP_FOREST;
        }
    }
}

/**
 * PRE: Will be given :int border:, the width of the generated border.
 * POST: Will generate an ocean border of width :border: around the world.
 */
void WorldMap::ocean_borders(int border) {
    for(int i = 0; i < border; i++) {
        for(int j = 0; j < width; j++) {
            set_land_or_water(i, j, 15, true);
        }
    }

    for(int i = (height - border); i < height; i++) {
        for(int j = 0; j < width; j++) {
            set_land_or_water(i, j, 15, true);
        }
    }

    for(int i = border; i < (height - border); i++) {
        for(int j = 0; j < border; j++) {
            set_land_or_water(i, j, 15, true);
        }

        for(int j = (width - border); j < width; j++){
            set_land_or_water(i, j, 15, true);
        }
    }

}

/**
 * PRE: Will be given :int row:, :int col:, and :MapTile tile_type:.
 * POST: Will return the number of occurences of tiles of type :tile_type: in
 * the tiles surrounding the given point.
 */
int WorldMap::count_in_surrounding_tiles(int row, int col,
                                        MapTile tile_type) {
    int num = 0;
    int newrow, newcol;

    for(int i = -1; i < 2; i++) {
        for(int j = -1; j < 2; j++) {
            if(!(i==0 && j==0)) {
                newrow = row + i;
                newcol = col + j;
                if(!out_of_bounds(newrow, newcol)) {
                    if(map[newrow][newcol] == tile_type) {
                        num++;
                    }
                }
            }
        }
    }

    return num;
}

/**
 * PRE: Will be given :int border:, which represents the width of the ocean
 * border.
 * POST: Will fill the rest of the map with random land/water tiles, then
 * performs the following change for every tile:
 *
 * ?O?    ?O?
 * O?O -> OOO
 * ?O?    ?O?
 *
 * where the O can represent either land or water tiles.
 */
void WorldMap::starting_noise(int border) {
    for(int i = border; i < (height - border); i++) {
        for(int j = border; j < (width - border); j++) {
            set_land_or_water(i, j, 3, false);
        }
    }

    for(int i = 1; i < (height - 1); i++) {
        for(int j = 1; j < (width - 1); j++) {
            if(map[i+1][j] == map[i][j+1] &&
                map[i-1][j] == map[i][j-1] &&
                map[i][j+1] == map[i-1][j]) {

                map[i][j] = map[i][j+1];
            }
        }
    }
}

/**
 * PRE: Will be given a tile type and :threshold:
 * POST: Will loop through the world map. On each tile, if the number of
 * surrounding tiles of type :tile_type: is greater than or equal to
 * :threshold:, there is a 1/(8 - num_surrounding_tiles) chance that the tile in
 * question will be changed to :tile_type:.
 */
void WorldMap::smoothing_pass(MapTile tile_type, int threshold) {
    int num;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            num = count_in_surrounding_tiles(i, j, tile_type);
            if(num >= threshold) {
                if(rand() % (8 - (num - 1)) == 0) {
                    map[i][j] = tile_type;
                }
            }
        }
    }
}

/**
 * PRE:
 * POST: Will generate a land mass. First, create an ocean border around the
 * map. Fill the rest with random noise, then do a series of smoothing passes
 * with certain parameters.
 */
void WorldMap::generate_land_mass() {
    int border_size = 5;
    ocean_borders(border_size);
    starting_noise(border_size);
    cout<<height<<" "<<width<<endl;

    //Tweaking any values here can give vastly different results.
    for(int i = 0; i < 15; i++) {
        smoothing_pass(map_tile::MAP_WATER, 4);
    }
    for(int i = 0; i < 17; i++) {
        smoothing_pass(map_tile::MAP_FOREST, 3);
    }
    for(int i = 0; i < 17; i++) {
        smoothing_pass(map_tile::MAP_WATER, 2);
    }
    smoothing_pass(map_tile::MAP_FOREST, 2);
    for(int i = 0; i < 12; i++) {
        smoothing_pass(map_tile::MAP_FOREST, 3);
    }
}

void WorldMap::generate_beaches() {
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(map[i][j] != map_tile::MAP_WATER && count_in_surrounding_tiles(i, j, map_tile::MAP_WATER) > 0)
            {
                map[i][j] = map_tile::MAP_BEACH;
            }
        }
    }
}
