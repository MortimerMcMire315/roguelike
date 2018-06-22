/**
 *  @file CORRUPTIBLE_PBLIND_DB.CPP
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

#include "corruptible_pblind_db.h"
#include "chunk_layer.h"
#include "tileset.h"

using namespace std;
namespace db=dungeon_builder;

namespace corruptible_pblind_db {
    void corrupt_corners(vector<IntPoint> corners, db::dungeon_meta &dm) {
        int vert = 0;
        int horiz = 0;
        int wall_blocks_found = 0;
        //push some corners in.
        //std::vector.size() returns an unsigned long...
        for(size_t j = 0; j < corners.size(); j++) {
            if (db::rolled_over(20)) {
                dm.main_dungeon->set_tile(corners[j], Tileset::get("EMPTY"));
                return;
            }

            if (db::rolled_over(40)) {
                vert = 0; horiz = 0; wall_blocks_found = 0;
                //There are a number of ways I could go about doing this.
                //I'm going with the weirdest, because YOLO.
                //This is for the first rule listed above corrupt_walls().

                //If the block below this point is a ROOM_WALL block...
                if((corners[j].row + 1 < dm.main_dungeon->height) &&
                        (dm.main_dungeon->get_tile(corners[j].row + 1, corners[j].col)
                         == Tileset::get("ROOM_WALL"))) {
                    vert = 1;
                    wall_blocks_found++;
                }
                if ((corners[j].row - 1 >= 0) &&
                        (dm.main_dungeon->get_tile(corners[j].row - 1, corners[j].col)
                         == Tileset::get("ROOM_WALL"))) {
                    vert = -1;
                    wall_blocks_found++;
                }
                if ((corners[j].col + 1 < dm.main_dungeon->width) &&
                        (dm.main_dungeon->get_tile(corners[j].row, corners[j].col + 1)
                         == Tileset::get("ROOM_WALL"))) {
                    horiz = 1;
                    wall_blocks_found++;
                }
                if ((corners[j].col + 1 < dm.main_dungeon->width) &&
                        (dm.main_dungeon->get_tile(corners[j].row, corners[j].col - 1)
                         == Tileset::get("ROOM_WALL"))) {
                    horiz = -1;
                    wall_blocks_found++;
                }

                if (wall_blocks_found != 2) {
                    return;
                } else {
                    dm.main_dungeon->set_tile(corners[j], Tileset::get("EMPTY"));
                    dm.main_dungeon->set_tile(corners[j].row + vert,
                            corners[j].col + horiz,
                            Tileset::get("ROOM_WALL"));
                }
            }
        }
    }

    void corrupt_walls(db::dungeon_meta &dm) {
        //Loop through the rooms.
        for(int i = 0; i < dm.num_rooms; i++) {
            Room& current_room = dm.main_dungeon->rooms[i];
            IntPoint tl = current_room.tl;
            IntPoint tr = IntPoint(current_room.tl.row, current_room.br.col);
            IntPoint bl = IntPoint(current_room.br.row, current_room.tl.col);
            IntPoint br = current_room.br;
            vector<IntPoint> corners = vector<IntPoint>(4);
            corners[0] = tl; corners[1] = tr; corners[2] = bl; corners[3] = br;
            corrupt_corners(corners, dm);
        }
    }

    void build_dungeon(int width, int height, int target, ChunkLayer &cl) {
        db::dungeon_meta dm;
        dm.width = width;
        dm.height = height;
        dm.num_rooms = 0;
        dm.main_dungeon = &cl;
        db::reset(dm);

        bool dungeon_is_awesome;
        pblind_db::build_start_room(dm);
        int tries = 0;
        do {
            tries++;
            dungeon_is_awesome = true;
            pblind_db::build_dungeon_recursive(target, dm);
            corrupt_walls(dm);

            if (dm.num_rooms < (target - 3)) {
                dungeon_is_awesome = false;
                dm.main_dungeon->clear();
                dm.num_rooms = 0;
            }
        } while(!dungeon_is_awesome);
    }
}
