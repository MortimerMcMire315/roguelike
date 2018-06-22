/**
 *  @file SETTLEMENT.H
 *  @author Michael and Seth Yoder
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

#include "settlement.h"

Settlement::Settlement(std::vector<<IntPoint> points)
{
    districts_from_intpoints(points);
}

void Settlement::districts_from_intpoints(std::vector<IntPoint> points)
{
    //sort the intpoints.  Note: this is padded!
    std::vector<std::vector<IntPoint> > sorted = sort_intpoints(points);

    //make a place to hold our maptiles
    std::vector<std::vector<MapTile> > tiles;
    tiles.resize(sorted.size());
    for(int i=0;i<sorted.size();i++)
    {
        tiles[i].resize(sorted[i].size());
    }

    int total = points.size();

    //set everything to the first tile
    for(int i=0;i<sorted.size();i++)
    {
        for(int j=0;j<sorted[i].size();j++)
        {
            //A little weird...because the area of the city
            //is not necessarily rectangular, but our vectors
            //should be, we have padding on either side.
            //Wherever there is not supposed to be a tile, the
            //sorted vector will insert an IntPoint(-1, -1).
            if(sorted[i].row != -1)
            {
                tiles[i][j] = map_tile::CITIES[0];
            }
            else
            {
                //Pad any junk areas of the tiles with generic
                //CITY tiles.
                tiles[i][j] = map_tile::CITY;
            }
        }
    }
    for(int i=1;i<map_tile::NUM_CITY_TILES;i++)
    {
        //replace areas on the map with tiles from the city
        seed_map(tiles, map_tile::CITIES[i], total/map_tile::NUM_CITY_TILES);
    }

    for(int i=0;i<tiles.size();i++)
    {
        for(int j=0;j<tiles.size();j++)
        {
            if(tiles[i][j] != map_tile::CITY)
            {
                districts.push_back(CityDistrict(CHUNK_WIDTH, CHUNK_HEIGHT, sorted[i][j].row, sorted[i][j].col, tiles[i][j])
            }
        }
    }
}

void Settlement::seed_map(std::vector<std::vector<MapTile> >& tiles, MapTile seeder, int num_tiles)
{

