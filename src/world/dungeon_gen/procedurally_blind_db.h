/**
 *  @file PROCEDURALLY_BLIND_DB.H
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

#ifndef _PROCEDURALLY_BLIND_DUNGEONBUILDER_H
#define _PROCEDURALLY_BLIND_DUNGEONBUILDER_H

#include "dungeonbuilder.h"

namespace db=dungeon_builder;

/**
 * Generates dungeons in a "procedurally-blind" manner.
 *
 * The method for this dungeon generation technique is:
 *
 * 1. Create a viable "first" dungeon room.
 * 2. Tunnel out from that room in a number of paths.
 * 3. At the end of the last path that was created, try to create another room.
 * 4. Repeat this until one of two conditions is met:
 *      a. The room limit is reached
 *      b. There is no viable room space at the end of the path.
 * 5. If the resulting dungeon does not have very many rooms, scrap it and start
 * over. This currently happens <25% of the time.
 */
namespace pblind_db {
    /**
     * Builds an initial room in the dungeon space.
     * @param dm - instance of a dungeon to work on
     * @see find_viable_starting_point
     */
    void build_start_room(db::dungeon_meta& dm);

    /**
     * Will find and return a viable room space, and if there is no such space,
     * will return a room with coordinates (-1,-1),(-1,-1).
     *
     *  Pseudocode for this function:
     *
     *  Declare min room width and min room height; declare and define test_room
     *  based on this width and height; declare upper_bound, lower_bound,
     *  left_bound, right_bound = 1
     *
     *  if test_room collides with something solid: return something nullish
     *
     *  while (room width < max) and (room height < max) AND (upper_bound +
     *  lower_bound + left_bound + right_bound > 0):
     *
     *      move all of test_room's points out; (subroutine probably) subtract
     *      upper_bound from row value of both upper points; subtract left_bound
     *      from col value of both left side points; add lower_bound to row
     *      value of both lower points; add right_bound to col value of both
     *      right side points;
     *
     *      "scan" across those newly created edges;
     *
     *      if, during the scan, we ran into a solid block: which side did it
     *      occur on?  Set the *_bound to 0 for that side; move the points on
     *      that side one step toward the room center;
     *
     * @param the_point - the point from which the function will start
     * attempting to find a room space.
     * @param dm - instance of a dungeon to work on
     *
     * @return a room representing the discovered room space, or
     * ((-1,-1),(-1,-1)) if there is no such room.
     */
    Room find_viable_room_space(IntPoint the_point, db::dungeon_meta& dm);

    /**
     * Will find a starting point for the dungeon.
     * @param dm - instance of a dungeon to work on
     *
     * @return the point that was found.
     */
    IntPoint find_viable_starting_point(const db::dungeon_meta& dm);

    /**
     * Builds a path starting at the given point.
     *
     * Will attempt to build a path starting from the given point, and
     * return the point where the path ends; if the path cannot be built,
     * returns IntPoint(-1, -1).
     *
     * @param start - the starting point for this path.
     * @param direction - The starting direction of the path, where 0 = up,
     * @param dm - instance of a dungeon to work on
     * 1 = right, 2 = down, and 3 = left.
     * @return the end of the path, or (-1,-1) if the path could not be
     * built.
     */
    IntPoint build_path(IntPoint start, int direction, db::dungeon_meta& dm);

    /**
     * A recursive helper function to build the dungeon.
     *
     * Finds a viable wall block in the given room, building paths
     * outward from those wall blocks and often building rooms at the ends
     * of those paths. Every time a new room is built, the function is
     * called again.
     *
     * @param target - The target number of rooms.
     * @param dm - instance of a dungeon to work on
     */
    void build_dungeon_recursive(int target, db::dungeon_meta& dm);

    /**
     * The primary entry point for dungeon building.
     *
     * @param target - The target number of rooms to build.
     * @param cl - Instance of a ChunkLayer to use.
     */
    void build_dungeon(int width, int height, int target, ChunkLayer& cl);
};

#endif
