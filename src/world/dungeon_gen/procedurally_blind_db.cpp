/**
 *  @file PROCEDURALLY_BLIND_DB.CPP
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

#include <procedurally_blind_db.h>

using namespace tiledef;

ProcedurallyBlindDB::ProcedurallyBlindDB() {

}

/* PRE:  Will be given the desired width and height of the
 * dungeon floor.
 * POST: Will populate the :char dungeon[][]: array with dummy
 * values.
 */
ProcedurallyBlindDB::ProcedurallyBlindDB(int _width, int _height) {
    width = _width;
    height = _height;
    num_rooms = 0;


    main_dungeon = ChunkLayer(width, height);
}

void ProcedurallyBlindDB::build_start_room() {
    int room_width = rand() % ROOM_WIDTH_DEV +
                             (STD_ROOM_WIDTH - (int)(ROOM_WIDTH_DEV/2));
    int room_height = rand() % ROOM_WIDTH_DEV +
                             (STD_ROOM_HEIGHT - (int)(ROOM_HEIGHT_DEV/2));
    IntPoint starting_point = find_viable_starting_point();
    IntPoint br = IntPoint(starting_point.row + (room_height + 1),
                           starting_point.col + (room_width + 1));
    build_room(starting_point, br);
}

Room ProcedurallyBlindDB::find_viable_room_space(IntPoint the_point) const {
    int min_room_width = STD_ROOM_WIDTH - (ROOM_WIDTH_DEV / 2);
    int min_room_height = STD_ROOM_HEIGHT - (ROOM_HEIGHT_DEV / 2);

    //create a room centered around the end of the path. floor() and ceil() derived from math.h.
    Room test_room = Room(IntPoint((the_point.row - (int) ceil(min_room_height / 2.0)),
                           (the_point.col - (int) ceil(min_room_width / 2.0) + 1)),
                  IntPoint((the_point.row + (int) floor(min_room_height / 2.0) + 1),
                           (the_point.col + (int) floor(min_room_width / 2.0))));

    //Check if the room is out-of-bounds; if so, return a null room
    if((point_is_beyond_bounds(test_room.tl)) || (point_is_beyond_bounds(test_room.br))) {
        return Room(IntPoint(-1, -1), IntPoint(-1, -1));
    }

    //Check the room's walls to see if they clip with anything else.
    if (edges_collide_with_something(test_room) != "0000") {
        return Room(IntPoint(-1, -1), IntPoint(-1, -1));
    }

    int upper_bound = 1;
    int lower_bound = 1;
    int left_bound = 1;
    int right_bound = 1;

    //Here, we should have a test_room that doesn't collide with things.
    int max_room_width = STD_ROOM_WIDTH + (ROOM_WIDTH_DEV / 2);
    int max_room_height = STD_ROOM_HEIGHT + (ROOM_HEIGHT_DEV / 2);

    //until each rectangle edge is unable to grow any more:
    while ((test_room.br.col - test_room.tl.col < max_room_width) &&
           (test_room.br.row - test_room.tl.row < max_room_height) &&
           (upper_bound + lower_bound + left_bound + right_bound > 0)) {
        //move the edges out
        test_room.tl.row -= upper_bound;
        test_room.tl.col -= left_bound;
        test_room.br.row += lower_bound;
        test_room.br.col += right_bound;

        //if we ran into a solid block, set *_bound to 0 for that edge,
        //then move that edge one step toward the room center...
        string collision_bin_str = edges_collide_with_something(test_room);
        if(collision_bin_str[0] == '1') {
            upper_bound = 0;
            test_room.tl.row += 1;
        }
        if(collision_bin_str[1] == '1') {
            right_bound = 0;
            test_room.br.col -= 1;
        }
        if(collision_bin_str[2] == '1') {
            lower_bound = 0;
            test_room.br.row -= 1;
        }
        if(collision_bin_str[3] == '1') {
            left_bound = 0;
            test_room.tl.col += 1;
        }
    }
    //Here, test_room should be a viable area in which to stick a real room.

    /*
     *  problem: shouldn't just use the largest room possible, that would be
     *  dumb. Instead, use that space to create a random room. But how to ensure
     *  that the path intersects this room?
     *
     *      solution to that issue:
     *
     *          -start with a left column that is between the left rectangle
     *          wall and (the lesser value of: (the right wall column -
     *          MIN_ROOM_WIDTH), the path column)
     *
     *          -get a right column somewhere between (the larger value of:
     *          (left room column + MIN_ROOM_WIDTH), the path column) and the
     *          right wall
     *
     *          -get a top row somewhere between the top wall and (the lesser
     *          value of: (the bottom wall row - MIN_ROOM_HEIGHT),the path row)
     *
     *          -get a bottom row somewhere between (the larger value of: (top
     *          room row + MIN_ROOM_HEIGHT), the path row) and the bottom wall
     *
     */

    //I am using the same variables, but for an entirely different purpose...
    //call the cops.
    int left_column_right_bound = min((test_room.br.col - min_room_width), the_point.col);
    left_column_right_bound = max(left_column_right_bound, test_room.tl.col + 1);
    left_bound = rand() % (abs(left_column_right_bound - test_room.tl.col)) + test_room.tl.col;

    int right_column_left_bound = max((test_room.tl.col + min_room_width), the_point.col);
    right_column_left_bound = min(right_column_left_bound, test_room.br.col - 1);
    right_bound = rand() % (abs(test_room.br.col - right_column_left_bound)) + test_room.br.col;

    int top_row_lower_bound = min((test_room.br.row - min_room_height), the_point.row);
    top_row_lower_bound = max(top_row_lower_bound, test_room.tl.row + 1);
    upper_bound = rand() % (abs(top_row_lower_bound - test_room.tl.row)) + test_room.tl.row;

    int bottom_row_upper_bound = max((test_room.tl.row + min_room_height), the_point.row);
    bottom_row_upper_bound = min(bottom_row_upper_bound, test_room.br.row - 1);
    lower_bound = rand() % (abs(test_room.br.row - bottom_row_upper_bound)) + test_room.br.row;

    IntPoint tl = IntPoint(upper_bound, left_bound);
    IntPoint br = IntPoint(lower_bound, right_bound);

    if((point_is_beyond_bounds(tl)) || (point_is_beyond_bounds(br))) {
        return Room(IntPoint(-1, -1), IntPoint(-1, 1));
    }

    return Room(tl, br);
}

IntPoint ProcedurallyBlindDB::find_viable_starting_point() const {
    int std_height = STD_ROOM_HEIGHT;
    int std_width = STD_ROOM_WIDTH;

    int good_row = rand() % (int)(height/2) + (int)(height/4) - (int)(std_height / 2);
    int good_col = rand() % (int)(width/2) + (int)(width/4) - (int)(std_width / 2);
    return IntPoint(good_row, good_col);
}

/* PRE: Will be given :IntPoint start:, which refers to the wall block of the
 * room at which the path starts, and :int direction:, which refers to the
 * starting direction of the path according to this scheme:
 * 0 = up
 * 1 = right
 * 2 = down
 * 3 = left
 *
 * POST: Will attempt to build a path from the given Room and block. If this
 * succeds, will return the IntPoint of the end of the path. If it fails, it
 * will return an IntPoint with row=-1 and col=-1.
 */
IntPoint ProcedurallyBlindDB::build_path(IntPoint start, int direction)
{
    int path_length = rand() % (MAX_PATH_LENGTH - MIN_PATH_LENGTH) + MIN_PATH_LENGTH;
    IntPoint current_point = start;
    int current_direction = direction;
    bool bad_direction;
    IntPoint potential_point;
    for(int i = 0; i < path_length; i++) {
        main_dungeon.set_tile(current_point, PATH);
        //For at least 2 or 3 blocks, just go straight. otherwise,
        //we may change direction.
        if (i > 3) {
            //Do we change direction?
            int dirchange = rand() % 100;
            if (dirchange < 15) {
                //Which direction do we change to?
                switch (dirchange % 2) {
                    case 0:
                        current_direction -= 1;
                        current_direction = (current_direction % 4);
                        break;
                    case 1:
                        current_direction += 1;
                        current_direction = (current_direction % 4);
                        break;
                }
            }
        }

        int tries = 0;
        do {
            bad_direction = false;
            potential_point = get_next_point(current_point, current_direction);
            if (point_is_beyond_bounds(potential_point) ||
                    !is_empty_space(potential_point)) {
                bad_direction = true;
                current_direction += 1;
            }
            tries++;
        } while((bad_direction) && tries < 4);

        current_point = potential_point;
    }
    return current_point;
}

/* PRE: Will be given :int target: to specify a general target number of
 * openings in the dungeon floor, :int deviation: to specify the maximum desired
 * deviation from this target, and :int squareness:, a number from 0 to 100,
 * which determines how square the rooms are.
 *
 * POST: Builds a procedurally-blind dungeon in the dungeon floor (in which a
 * room is built near the center, and rooms and hallways crawl off of that room.
 */
void ProcedurallyBlindDB::build_dungeon(int target) {
    reset();
    bool dungeon_is_awesome;
    build_start_room();
    int tries = 0;
    do {
        tries++;
        dungeon_is_awesome = true;
        build_dungeon_recursive(target);
        if (num_rooms < (target - 3)) {
            dungeon_is_awesome = false;
            main_dungeon = ChunkLayer(width, height);
            num_rooms = 0;
            main_dungeon.num_rooms = 0;
            build_start_room();
        }
    } while(!dungeon_is_awesome);
    main_dungeon.num_rooms = num_rooms;
}

void ProcedurallyBlindDB::build_dungeon_recursive(int target)
{
    if (target == 0) {
        return;
    }

    Room current_room;
    Room new_room;
    int tries = 0;
    bool acceptable;
    int max_tries = 3;
    do {
        tries++;
        acceptable = true;
        assert(num_rooms > 0);
        if(num_rooms == 0) {
            acceptable = false;
            break;
        }
        current_room = main_dungeon.rooms[num_rooms - 1];
        IntPoint point = rand_wall_block(current_room);

        IntPoint path_end = build_path(point, determine_which_wall(point));
        new_room = find_viable_room_space(path_end);

        //This ensures that the room is valid. If this value is -1, it is
        //because find_viable_room_space couldn't make a valid room.
        if(new_room.tl.row == -1) {
            acceptable = false;
        }

    } while((acceptable == false) && (tries < max_tries));

    if(!acceptable) {
        return;
    } else {
        build_room(new_room.tl, new_room.br);
        build_dungeon_recursive(target - 1);
    }
}

