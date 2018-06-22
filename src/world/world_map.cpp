/**
 *  @file WORLD_MAP.CPP
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

#include <sstream>

#include "world_map.h"

using namespace std;

WorldMap::WorldMap() {
    srand(time(NULL));
    height = WORLD_HEIGHT;
    width = WORLD_WIDTH;
    map = std::vector<std::vector<MapTile> >(height,
            std::vector<MapTile>(width));

    generate_land_mass();
    generate_beaches();
    seed_cities();
}

std::vector<std::vector<MapTile> >& WorldMap::get_map() {
    return map;
}

bool WorldMap::out_of_bounds(int row, int col) {
    return (row < 0 ||
            row >= height ||
            col < 0 ||
            col >= width);
}

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

int WorldMap::count_not_surrounding_tiles(int row, int col, MapTile tile_type)
{
    return (8 - count_in_surrounding_tiles(row, col, tile_type));
}

void WorldMap::starting_noise(int border) {
    for(int i = border; i < (height - border); i++) {
        for(int j = border; j < (width - border); j++) {
            map[i][j] = random_weighted_tile();
        }
    }
}

void WorldMap::smoothing_pass(MapTile tile_type, int threshold) {
    int num;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            num = count_in_surrounding_tiles(i, j, tile_type);
            if(num >= threshold) {
                //what is this doing?!
                if(rand() % (8 - (num - 1)) == 0) {
                    map[i][j] = tile_type;
                }
            }
        }
    }
}

void WorldMap::generate_land_mass() {
    int border_size = 5;
    ocean_borders(border_size);
    starting_noise(border_size);
    for(int i=0;i<map_tile::NUM_WORLD_TILES;i++)
    {
        MapTile tile = map_tile::WORLD_CONSTRUCTION_INDEX[i];
        if(tile.smoothed)
        {
            for(int j = 0;j < 10; j++) {
                smoothing_pass(map_tile::WORLD_CONSTRUCTION_INDEX[i], tile.smooth_amount);
            }
        }
    }
    for(int i = 0; i < 17; i++) {
        smoothing_pass(map_tile::MAP_WATER, 3);
    }
    //Tweaking any values here can give vastly different results.
    /**
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
    **/
}


void WorldMap::ocean_borders(int border) {
    for(int i = 0; i < border; i++) {
        for(int j = 0; j < width; j++) {
            map[i][j] = map_tile::MAP_WATER;
        }
    }

    for(int i = (height - border); i < height; i++) {
        for(int j = 0; j < width; j++) {
            map[i][j] = map_tile::MAP_WATER;
        }
    }

    for(int i = border; i < (height - border); i++) {
        for(int j = 0; j < border; j++) {
            map[i][j] = map_tile::MAP_WATER;
        }

        for(int j = (width - border); j < width; j++){
            map[i][j] = map_tile::MAP_WATER;
        }
    }

}

void WorldMap::generate_beaches() {
    for(int i=1;i<height - 2;i++) {
        for(int j=1;j<width - 2;j++) {
            //get the number of beaches
            int num_beaches = count_in_surrounding_tiles(i, j, map_tile::MAP_BEACH);
            //get the non water tiles
            int num_land = count_not_surrounding_tiles(i, j, map_tile::MAP_WATER);

            //we don't want beaches to count towards making another beach, there's already
            //one there.
            int countable_land = num_land - num_beaches;

            if((map[i][j] == map_tile::MAP_WATER) && countable_land > 0 && countable_land < 6) {
                map[i][j] = map_tile::MAP_BEACH;
            }
        }
    }
}

void WorldMap::seed_cities()
{
    std::vector<std::vector<IntPoint> > cities = find_contiguous(map_tile::CITY);
    for(int i=0;i<cities.size();i++)
    {
        std::vector<std::vector<IntPoint> > city = sort_city(cities[i]);
        generate_city(city, cities[i].size());
    }
}

std::vector<std::vector<IntPoint> > WorldMap::sort_city(std::vector<IntPoint> city)
{
    //get the minimum/maximum x and y for the city
    //this is for the rectangular bounding box around the city
    int min_x = city[0].col;
    int min_y = city[0].row;
    int max_x = city[0].col;
    int max_y = city[0].row;
    for(int i=0;i<city.size();i++)
    {
        if(city[i].row < min_y)
        {
            min_y = city[i].row;
        }
        if(city[i].col < min_x)
        {
            min_x = city[i].col;
        }
        if(city[i].row > max_y)
        {
            max_y = city[i].row;
        }
        if(city[i].col > max_x)
        {
            max_x = city[i].col;
        }
    }

    //Turn it into a two dimensional vector of IntPoints where
    //-1, -1 is a non-city tile
    int accum = 0;
    std::vector<std::vector<IntPoint> > sorted_city;
    for(int i=min_y;i<=max_y;i++)
    {
        sorted_city.push_back(std::vector<IntPoint>());
        for(int j=min_x;j<=max_x;j++)
        {
            if(tile_at(i, j) != map_tile::CITY)
            {
                sorted_city[accum].push_back(IntPoint(-1, -1));
            }
            else
            {
                sorted_city[accum].push_back(IntPoint(i, j));
            }
        }
        accum++;
    }
    return sorted_city;
}

void WorldMap::generate_city(std::vector<std::vector<IntPoint> > city, int city_size)
{
    IntPoint seed = get_seed(city);
    if(seed != IntPoint(-1, -1)) {
        random_flood(city, map_tile::CITIES[0], city_size, seed);
        for(int i=1;i<map_tile::NUM_CITY_TILES;i++)
        {
            random_flood(city, map_tile::CITIES[i], (city_size/(i * 2)), seed);
        }
    }
}

IntPoint WorldMap::get_seed(std::vector<std::vector<IntPoint> > city)
{
    //get our y coordinate
    int y_coord = rand() % city.size();

    //if our y coordinate is on an edge, get any x on
    //those edges
    if(y_coord == 0 || y_coord == (city.size() - 1))
    {
       int x_coord;
       do {
           x_coord = rand() % city[y_coord].size();
       } while(city[y_coord][x_coord] == IntPoint(-1, -1));

       return IntPoint(y_coord, x_coord);
    }

    //if the y coordinate isn't on an edge
    //get an x that's on an edge
    int x_direction = rand() % 2 ? -1 : 1;
    int start_x = (city[y_coord].size() - 1) * (x_direction == -1);
    int end_x = city[y_coord].size() * (x_direction == 1);
    for(int i=start_x;i!=end_x;i+=x_direction)
    {
        if(city[y_coord][i] != IntPoint(-1, -1))
        {
            return IntPoint(y_coord, i);
        }
    }

    //if we failed at that for some reason, return the first coordinate
    for(int i=0;i<city.size();i++) {
        for(int j=0;j<city[i].size();j++) {
            if(city[i][j] != IntPoint(-1, -1)) {
                return IntPoint(i, j);
            }
        }
    }
    //if we're still failing, return an unusable cordinate
    return IntPoint(-1, -1);
}



MapTile WorldMap::tile_at(int row, int col)
{
    return map[row][col];
}

std::vector<std::vector<IntPoint> > WorldMap::find_contiguous(MapTile target)
{
    std::vector<IntPoint> closed_list;
    std::vector<std::vector<IntPoint> > contiguous;
    for(int i=0;i<map.size();i++)
    {
        for(int j=0;j<map[i].size();j++)
        {
            if(map[i][j] == target && !utility::is_in(closed_list, IntPoint(i, j)))
            {
                std::vector<IntPoint> cur_contig;
                flood(IntPoint(i, j), closed_list, cur_contig, target);
                contiguous.push_back(cur_contig);
            }
        }
    }
    return contiguous;
}

void WorldMap::flood(IntPoint start_point, std::vector<IntPoint>& closed, std::vector<IntPoint>& cur_contig, MapTile target)
{
    cur_contig.push_back(start_point);
    closed.push_back(start_point);
    for(int i=(start_point.row - 1);i<(start_point.row + 2);i++)
    {
        for(int j=(start_point.col - 1);j<(start_point.col + 2); j++)
        {
            if(map[i][j] == target && !utility::is_in(closed, IntPoint(i, j)))
            {
                flood(IntPoint(i, j), closed, cur_contig, target);
            }
        }
    }
}

//We're effectively working with two coordinate systems here:
//the map coords, and the coordinates within the city grid.
//The cur_tile starts as the coordinates within the city grid
void WorldMap::random_flood(std::vector<std::vector<IntPoint> >& flood_map, MapTile tile, int remaining, IntPoint cur_tile)
{
    if(remaining >= 0)
    {
        //change it to the map coordinates
        IntPoint mrmeseekstile = flood_map[cur_tile.row][cur_tile.col];
        //set the map tile to the tile that we want to use
        map[mrmeseekstile.row][mrmeseekstile.col] = tile;

        for(int i=(cur_tile.row - 1);i<(cur_tile.row + 2);i++)
        {
            for(int j=(cur_tile.col - 1);j<(cur_tile.col + 2); j++)
            {
                //do it for the rest
                if(i >= 0 && i < flood_map.size() && j >= 0 && j < flood_map[i].size())
                {
                    IntPoint tile_coords = flood_map[i][j];
                    if(tile_coords != IntPoint(-1, -1) && map[tile_coords.row][tile_coords.col] != tile)
                    {
                        remaining -= 1;
                        random_flood(flood_map, tile, remaining, IntPoint(i, j));
                    }
                }
            }
        }
    }
}


MapTile WorldMap::random_weighted_tile()
{
    //it seems ridiculous that I'm constructing this
    //so many times when I'll be relying on it...should
    //I just pass these in?
    std::vector<MapTile> tile_list;
    int total_tiles = 0;
    for(int i=0;i<map_tile::NUM_MAP_TILE;i++)
    {
        for(int j=0;j<map_tile::MAP_TILE_INDEX[i].weight;i++)
        {
            tile_list.push_back(map_tile::MAP_TILE_INDEX[i]);
            total_tiles += 1;
        }
    }
    return tile_list[rand() % total_tiles];
}

