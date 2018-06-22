/**
 *  @file OVERWORLD.CPP
 *  @author Michael Yoder
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

#include "overworld_gen.h"
#include "chunk_layer.h"
#include "building.h"
#include "city_district.h"
#include "chunk.h"
#include "block.h"
#include "bresenham.h"
#include "spawner.h"
#include "int_point.h"
#include "tileset.h"

namespace overworld_gen {
    bool smoothing_pass(int threshold, Tile tile_type, int num) {
        if(num >= threshold) {
            if (rand() % (8 - (num -1)) == 0) {
                return true;
            }
        }
        return false;
    }

    void build_land_overworld(ChunkLayer& ground) {
        std::unordered_map<std::string, Tile>* tileset = &Tileset::instance()->get_tileset();

        for(int i = 0; i < ground.height; i++) {
            for(int j = 0; j < ground.width; j++) {
                if (rand() % 8 == 0){
                    ground.set_tile(i,j,(*tileset)["TREE"]);
                } else {
                    ground.set_tile(i,j,(*tileset)["OVERWORLD_DIRT"]);
                }
            }
        }
        ground.down_stairs[0].col = rand() % ground.width;
        ground.down_stairs[0].row = rand() % ground.height;
        if(ground.has_layer_below) {
            ground.set_tile(ground.down_stairs[0], (*tileset)["DOWN_STAIR"]);
        }
        IntPoint spawn = IntPoint(rand() % ground.height, rand() % ground.width);
        ground.spawners.push_back(Spawner(spawn.col, spawn.row, 0, rabbit));
        ground.set_tile(spawn, (*tileset)["KOBOLD_SPAWNER"]);
    }

    void build_water_overworld(ChunkLayer& ground) {
        std::unordered_map<std::string, Tile>* tileset = &Tileset::instance()->get_tileset();
        for(int i = 0; i < ground.height; i++) {
            for(int j = 0; j < ground.width; j++) {
                if (rand() % 4 == 0) {
                    ground.set_tile(i,j,(*tileset)["LIGHT_WATER"]);
                } else {
                    ground.set_tile(i,j,(*tileset)["WATER"]);
                }
            }
        }
    }

    void build_beach_overworld(ChunkLayer& ground) {
        std::unordered_map<std::string, Tile>* tileset = &Tileset::instance()->get_tileset();
        for(int i = 0; i < ground.height; i++) {
            for(int j = 0; j < ground.width; j++)
            {
                if( rand() % 2 == 0) {
                    ground.set_tile(i,j,(*tileset)["SAND1"]);
                }
                else{
                    ground.set_tile(i,j,(*tileset)["SAND2"]);
                }
            }
        }
    }

    void build_forest_overworld(ChunkLayer& ground) {
        int height = ground.height;
        int width = ground.width;
        /*
         *TODO:
         * -Put in a smaller bresenham circle to fill the gaps for water in groves
         * -Probably make it so that stairs and spawners don't spawn where they shouldn't
         * -Play around with numbers for a prettier effect
         * -Bresenham ellipses?
         */

        std::unordered_map<std::string, Tile>* tileset = &Tileset::instance()->get_tileset();
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                ground.set_tile(i,j,(*tileset)["DIRT"]);
            }
        }

        //what does this do?
        IntPoint coords = IntPoint(0,0);
        do {
            int newrow = rand() % height;
            cout<<newrow<<endl;
            coords.row = newrow;
            coords.col = rand() % width;
        } while (ground.get_tile(coords).can_be_moved_through == false);


        if(rand() % 10 == 0) {
           //spawn a grove!
           int radius = (rand() % 5) + 10;
           int start_x = (rand() % (width - 2 * (radius+1)) + radius);
           int start_y = (rand() % (height - 2 * (radius+1)) + radius);
           IntPoint start = IntPoint(start_y, start_x);

           for(int i=start_y-radius;i<start_y+radius;i++) {
               for(int j=start_x-radius;j<start_x+radius;j++) {
                   ground.set_tile(i,j,(*tileset)["DIRT"]);
               }
           }

            int new_rad = radius - ((rand() % 3) + 5);
            std::vector<IntPoint> in_circle = bresenham_circle(start, new_rad);

            for(int i=0;i<in_circle.size();i++) {
                std::vector<IntPoint> line = bresenham_line(start, in_circle[i]);

                for(int j=0;j<line.size();j++) {
                    ground.set_tile(line[j], (*tileset)["WATER"]);
                }
            }

            for(int i=start_y-new_rad-1;i<start_y+new_rad+1;i++) {
                for(int j=start_x-new_rad-1;j<start_x+new_rad+1;j++) {
                    int num = 0;
                    /**
                     * MMMMMM. \todo consider sticking
                     * world_map::count_in_surrounding_tiles in the helper
                     * methods... or just include it here.
                     */
                    Tile water = Tileset::get("WATER");
                    num += (ground.get_tile(i-1, j-1) == water) +
                           (ground.get_tile(i-1, j) == water) +
                           (ground.get_tile(i-1, j+1) == water) +
                           (ground.get_tile(i, j-1) == water) +
                           (ground.get_tile(i, j+1) == water) +
                           (ground.get_tile(i+1, j-1) == water) +
                           (ground.get_tile(i+1, j) == water) +
                           (ground.get_tile(i+1, j+1) == water);
                    //a threshold of 4 or above gives normal circular pools
                    //a threshold of 2 gives slightly odd pools
                    //a threshold of 1 gives very oddly shaped pools
                    if(smoothing_pass(0, water, num)) {
                        ground.set_tile(i,j,water);
                    }
                }
            }
        }

        IntPoint down_stair;
        down_stair.col = rand() % width;
        down_stair.row = rand() % height;
        ground.down_stairs.push_back(down_stair);

        if(ground.has_layer_below) {
            ground.set_tile(down_stair, (*tileset)["DOWN_STAIR"]);
        }

        for(int i=0;i<1;i++)
        {
            if(true)//rand() % 50 == 0)
            {
                int spawn_x = rand() % (int)(width - .2 * width) + .1 * width;
                int spawn_y = rand() % (int)(height - .2 * height) + .1 * height;
                ground.spawners.push_back(Spawner(spawn_x, spawn_y, 0, kobold));
            }
            if(rand() % 10 == 0)
            {
                int spawn_x = rand() % width;
                int spawn_y = rand() % height;
                std::cout<<"x: "<<spawn_x<<", y: "<<spawn_y<<std::endl;
                ground.spawners.push_back(Spawner(spawn_x, spawn_y, 0, rabbit));
            }
        }

        /**************** TEST BUILDINGS, REMOVE FROM HERE TO END OF FUNC*********/
        /**
        CityDistrict settlement = CityDistrict(0, 0, 400, 700);
        std::vector<Block> blocks = settlement.get_blocks();

        for(int i=0;i<blocks.size();i++)
        {
            std::vector<Building> buildings = blocks[i].get_buildings();
            for(int j=0;j<buildings.size();j++)
            {
                for(int k=0;k<buildings[j].get_height();k++)
                {
                    for(int l=0;l<buildings[j].get_width();l++)
                    {
                        int y = blocks[i].get_y() + k + buildings[j].get_y();
                        int x = blocks[i].get_x() + l + buildings[j].get_x();
                        Tile chara = buildings[j].tile_at(k, l);
                        ground.set_tile(y, x, chara);
                    }
                }
            }
        }
        */
    }

    void build_city_overworld(ChunkLayer& ground)
    {
        for(int i = 0; i < ground.height; i++) {
            for(int j = 0; j < ground.width; j++) {
                ground.set_tile(i,j,Tileset::get("COBBLE"));
            }
        }
    }

    /**
     * Not sure where to put these thoughts. Chunk creation will have to
     * be kicked off by ChunkMatrix.
     *
     * From ChunkMatrix:
     * When chunk x has to be newly created:
     *     For each surrounding chunk coordinate:
     *         If the chunk is already loaded in the ChunkMatrix:
     *             Load heightmap vector with a pointer to its heightmap.
     *         Else:
     *             If a file for the chunk exists:
     *                 Do a partial load to get the heightmap (maybe put
     *                 this at the beginning of the save file). Delete
     *                 the pointer after chunk generation is finished.
     *             Else:
     *                 The pointer for its heightmap is NULL; we can do
     *                 whatever we want for chunk x.
     */

    void seed_edges(Chunk& ground, int turbidity, HeightmapPtrList surrounding_chunks) {
        return;
    }

    void create_hills(Chunk& ground, int turbidity, ChunkPtrList surrounding_chunks) {
        HeightmapPtrList heightmaps(8);
        for(int i = 0; i < 8; i++) {
            if(surrounding_chunks[i] == NULL) {
                heightmaps[i] = NULL;
            }
            heightmaps[i] = surrounding_chunks[i]->get_heightmap();
        }
        seed_edges(ground, turbidity, heightmaps);

    }
}
