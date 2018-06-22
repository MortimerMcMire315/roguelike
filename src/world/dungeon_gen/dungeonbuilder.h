/**
 *  @file DUNGEONBUILDER.H
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

#ifndef DUNGEONBUILDER_H
#define DUNGEONBUILDER_H


#include "bitset"
#include "ASCII_Lib.h"

//forward declarations
class IntPoint;
class ChunkLayer;
class Room;


using namespace std;

/**
 * A namespace from which all dungeon-building classes inherit.
 *
 * This provides core functionality that those dungeon builders share.
 *
 * @see ProcedurallyBlindDB
 * @see CorruptiblePBlindDB
 */
namespace dungeon_builder {

    /**
     * Data describing a dungeon. This wraps around a ChunkLayer and provides
     * extra information.
     */
    struct dungeon_meta {
        /**
         * @see ChunkLayer
         */
        ChunkLayer* main_dungeon;
        /**
         * The width of the dungeon to be created.
         */
        int width;
        /**
         * The height of the dungeon to be created.
         */
        int height;
        /**
         * The number of rooms in this dungeon.
         * @see Room
         */
        int num_rooms;
    };

    /**
     * "Typical" rooms are about this large.
     */
    static const int STD_ROOM_WIDTH=8;
    static const int STD_ROOM_HEIGHT=6;

    static const int MAX_PATH_LENGTH=75;
    static const int MIN_PATH_LENGTH=16;

    /**
     * Rooms can only deviate by this width and height.
     * Keep these two numbers even.
     */
    static const int ROOM_WIDTH_DEV=2;
    static const int ROOM_HEIGHT_DEV=2;

    /**
     * Has an n% chance of returning 'true', where n is the given number.
     *
     * @param given - the desired probability that this check will return
     * true.
     * @return True if a random roll between 1 and 100 landed below the
     * given number.
     */
    bool rolled_over(int given);

    /**
     * Check if the given point on the dungeon model can be written over.
     * @param point - the point to check.
     * @param dm - instance of a dungeon to work on
     * @return True if the given point is empty.
     */
    bool is_empty_space(IntPoint point, const dungeon_meta& dm);

    /**
     * Determines whether or not the given point is beyond the bounds of the
     * dungeon.
     * @param point - the point to check
     * @param dm - instance of a dungeon to work on
     * @return True if the point is out of bounds.
     */
    bool point_is_beyond_bounds(IntPoint point, const dungeon_meta& dm);

    /**
     * Determines whether the edges of a given room collide with another
     * room.
     * @param r - the room to check
     * @param dm - instance of a dungeon to work on
     * @return A binary string that denotes which edges of the room found
     * collisions; the first bit is the top edge, then right, then bottom,
     * then left.
     *
     * \todo maybe use a bitset here.
     */
    std::bitset<4> edges_collide_with_something(Room &r, const dungeon_meta& dm);

    /**
     * Determines which wall of a room a given point lies on.
     * @param point - a point which has previously been determined to lie on
     * the edge of a room wall.
     * @param dm - instance of a dungeon to work on
     *
     * @return An integer denoting the wall that the given room lies on,
     * where 0 = top, 1 = right, 2 = bottom, 3 = left.
     */
    int determine_which_wall(IntPoint point, const dungeon_meta& dm);

    /**
     * Builds a room in the dungeon with the given points.
     * @param tl - the top-left corner of the room.
     * @param br - the bottom-right corner of the room.
     * @param dm - instance of a dungeon to work on
     * @return The room that was built.
     * @see room
     */
    Room build_room(IntPoint tl, IntPoint br, dungeon_meta& dm);

    /**
     * Finds a random tile on the circumference of the given room.
     * @param current_room - the room to use.
     * @return A random IntPoint on the circumference of this room.
     */
    IntPoint rand_wall_block(const Room&);

    /**
     * Finds the adjacent point in the given direction
     * @param this_point - the current point.
     * @param direction - the direction to move, where 0 = up, 1 = right, 2 =
     * down, and 3 = left.
     * @return The point that was found.
     */
    IntPoint get_next_point(IntPoint, int);

    /**
     * Resets the num_rooms and main_dungeon variables, effectively clearing
     * the dungeon. The dungeon must be initialized before calling this
     * method.
     * @param dm - instance of a dungeon to work on
     */

    void reset(dungeon_meta &dm);
    /**
     * Sets the given point to a ROOM_WALL tile if it is not a PATH tile.
     * @param row
     * @param col
     * @param dm - instance of a dungeon to work on
     */
    void set_wall_if_not_path(int row, int col, dungeon_meta &dm);

    ChunkLayer* get_dungeon(dungeon_meta &dm);
};

#endif
