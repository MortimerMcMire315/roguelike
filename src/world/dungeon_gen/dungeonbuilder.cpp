/**
 *  @file DUNGEONBUILDER.CPP
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

#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "room.h"
#include "dungeonbuilder.h"
#include "defs.h"
#include "chunk_layer.h"
#include "tileset.h"

namespace dungeon_builder {

    bool rolled_over(int given) {
        int generated = rand() % 100 + 1;
        if(generated < given) {
            return true;
        } else {
            return false;
        }
    }

    bool is_empty_space(IntPoint point, const dungeon_meta& dm) {
        return ((dm.main_dungeon->get_tile(point) == Tileset::get("BLOCK_WALL")) || (dm.main_dungeon->get_tile(point) == Tileset::get("PATH")));
    }

    bool point_is_beyond_bounds(IntPoint point, const dungeon_meta& dm) {
        if((point.row < 1) or (point.row >= (dm.main_dungeon->height - 1))) {
            return true;
        } else if((point.col < 1) or (point.col >= (dm.main_dungeon->width - 1))) {
            return true;
        }

        return false;
    }

    std::bitset<4> edges_collide_with_something(Room& r, const dungeon_meta& dm) {
        std::bitset<4> bin_string;
        for(int row = r.tl.row; row <= r.br.row; row++) {
            if (point_is_beyond_bounds(IntPoint(row, r.tl.col),dm)) {
                bin_string[3] = 1;
            } else if (dm.main_dungeon->get_tile(row, r.tl.col) == Tileset::get("ROOM_WALL")) {
                bin_string[3] = 1;
            }

            if (point_is_beyond_bounds(IntPoint(row, r.br.col),dm)) {
                bin_string[1] = 1;
            } else if (dm.main_dungeon->get_tile(row, r.br.col) == Tileset::get("ROOM_WALL")) {
                bin_string[1] = 1;
            }
        }

        for(int col = r.tl.col; col <= r.br.col; col++) {
            if (point_is_beyond_bounds(IntPoint(r.tl.row, col),dm)) {
                bin_string[0] = 1;
            } else if (dm.main_dungeon->get_tile(r.tl.row, col) == Tileset::get("ROOM_WALL")) {
                bin_string[0] = 1;
            }

            if (point_is_beyond_bounds(IntPoint(r.br.row, col),dm)) {
                bin_string[2] = 1;
            } else if (dm.main_dungeon->get_tile(r.br.row, col) == Tileset::get("ROOM_WALL")) {
                bin_string[2] = 1;
            }
        }
        return bin_string;
    }

    int determine_which_wall(IntPoint point, const dungeon_meta& dm) {
        int direction = 0;

        if(point.col == 0) {
            return 3;
        } else if (point.row == 0) {
            return 0;
        } else if (point.col == dm.main_dungeon->width - 1) {
            return 1;
        } else if (point.row == dm.main_dungeon->height - 1) {
            return 2;
        } else if((dm.main_dungeon->get_tile(point.row, point.col - 1) == Tileset::get("ROOM_WALL")) or
           (dm.main_dungeon->get_tile(point.row, point.col + 1) == Tileset::get("ROOM_WALL"))) {

            if (dm.main_dungeon->get_tile(point.row - 1, point.col) == Tileset::get("DIRT")) {
                direction = 2;
            }

            else if (dm.main_dungeon->get_tile(point.row + 1, point.col) == Tileset::get("DIRT")) {
                direction = 0;
            }
        } else if ((dm.main_dungeon->get_tile(point.row - 1, point.col) == Tileset::get("ROOM_WALL")) or
            (dm.main_dungeon->get_tile(point.row + 1, point.col) == Tileset::get("ROOM_WALL"))) {

            if (dm.main_dungeon->get_tile(point.row, point.col - 1) == Tileset::get("DIRT")) {
                direction = 1;
            }

            else if (dm.main_dungeon->get_tile(point.row, point.col + 1) == Tileset::get("DIRT")) {
                direction = 3;
            }
        }

        return direction;
    }

    Room build_room(IntPoint tl, IntPoint br, dungeon_meta& dm) {
        set_wall_if_not_path(tl.row, tl.col, dm);
        set_wall_if_not_path(tl.row, br.col, dm);
        set_wall_if_not_path(br.row, tl.col, dm);
        set_wall_if_not_path(br.row, br.col, dm);

        //draw top and bottom rows
        for(int i = tl.col + 1; i <= br.col - 1; i++) {
            set_wall_if_not_path(tl.row, i, dm);
            set_wall_if_not_path(br.row, i, dm);
        }
        //draw left and right ROOM_WALLs
        for(int i = tl.row + 1; i <= br.row - 1; i++) {
            set_wall_if_not_path(i, tl.col, dm);
            set_wall_if_not_path(i, br.col, dm);
        }

        std::unordered_map<std::string, Tile>* tileset = &Tileset::instance()->get_tileset();
        for(int i = tl.row + 1; i <= br.row - 1; i++) {
            for(int j = tl.col + 1; j <= br.col - 1; j++) {
                dm.main_dungeon->set_tile(i, j, (*tileset)["DIRT"]);
            }
        }
        dm.main_dungeon->rooms[dm.num_rooms] = Room(tl, br);
        dm.num_rooms++;

        return Room(tl, br);
    }

    IntPoint rand_wall_block(const Room &current_room) {
        //TODO do these -1's need to be here?
        int height = (current_room.br.row - current_room.tl.row) - 1;
        int width = (current_room.br.col - current_room.tl.col) - 1;
        int path_from_side = rand() % (height + width) + 1;
        IntPoint point;
        if (path_from_side < height) {
            int a = rand() % 2;
            if (a == 0) {
                point.row = current_room.tl.row;
            } else {
                point.row = current_room.br.row;
            }
            point.col = rand() % width + current_room.tl.col + 1;
        } else {
            int a = rand() % 2;
            if (a == 0) {
                point.col = current_room.tl.col;
            } else {
                point.col = current_room.br.col;
            }
            point.row = rand() % height + current_room.tl.row + 1;
        }

        return point;
    }

    IntPoint get_next_point(IntPoint this_point, int direction) {
        IntPoint next_point = this_point;
        switch (direction) {
            case 0:
                next_point.row -= 1;
                break;
            case 1:
                next_point.col += 1;
                break;
            case 2:
                next_point.row += 1;
                break;
            case 3:
                next_point.col -= 1;
                break;
        }

        return next_point;
    }

    void set_wall_if_not_path(int row, int col, dungeon_meta &dm)  {
        if(!(dm.main_dungeon->get_tile(row, col) == Tileset::get("PATH"))) {
            dm.main_dungeon->set_tile(row, col, Tileset::get("ROOM_WALL"));
        }
    }

    void reset(dungeon_meta &dm) {
        dm.num_rooms = 0;
        dm.main_dungeon->clear();
    }

    ChunkLayer* get_dungeon(dungeon_meta &dm){
        return dm.main_dungeon;
    }
}
