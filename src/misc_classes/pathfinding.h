/**
 *  PATHFINDING.H
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

#ifndef _PATHFINDING_H
#define _PATHFINDING_H

#include <vector>
#include <cmath>

#include "int_point.h"
#include "defs.h"

namespace pathfinding
{
    typedef std::vector<std::vector<Tile*> > TilePointerMatrix;
    /**
     * A rather odd way of handling linked lists.
     * This is only really used in the A-star algorithm, and is designed to
     * act as a linked list by accessing on index instead of by memory address.
     * @see Enemy::a_star(IntPoint start, IntPoint goal, TilePointerMatrix& surroundings)
     */
    struct ATile
    {
        /**
         * The index value of the parent of this tile
         */
        int parent;

        /**
         * The coordinates of the tile.
         */
        IntPoint coords;

        /**
         * The total "score" for this tile.
         * Made up of f + g
         */
        int f;

        /**
         * The distance that has passed from the origin to this tile.
         */
        int g;

        /**
         * The result of the Manhattan Heuristic
         * @see Enemy::manhattan(IntPoint current, IntPoint goal)
         */
        int h;

        /**
         * Default constructor.
         */
        ATile()
        {
            f = -1;
            g = -1;
            h = -1;
        }

        /**
         * The constructor
         */
        ATile(int _parent, IntPoint _coords)
        {
            parent = _parent;
            coords = _coords;
            f = 0;
            g = 0;
            h = 0;
        }
    };


    /**
     * Determines the best next move to make to reach a goal.
     * This function loods at the current coordinates, the coordinates of
     * the goal, and surroundings, and decides what the next move on the
     * shortest path is to reach that goal.
     * @param goal THe coordinates of the goal to reach
     * @param surroundings A matrix of the tiles surrounding the enemy.
     * @param cur_coords The current coordinates in the surroundings matrix.
     * @return The coordinates of the best next move.
     * @see a_star(IntPoint start, IntPoint goal, TilePointerMatrix& surroundings)
     */
    IntPoint get_next_step(IntPoint goal, TilePointerMatrix& surroundings, IntPoint cur_coords, int sight);

    /**
     * Gets the next step by simply moving towards the target.  Tries moving left/right
     * if the first step fails.
     */
    IntPoint dumb_path(IntPoint goal, TilePointerMatrix& surroundings, IntPoint cur_coords);

    /**
     * Determines whether the coords are in the list of Tiles.
     * A helper function for the a_star which determines if a given
     * set of coords already exists in a list of ATiles.
     * @param point The point to check against.
     * @param list The list of ATiles which may possess the point.
     * @return The index of the location of the point in the list, or -1 if it is not in the list.
     * @see a_star(IntPoint start, IntPoint goal, TilePointerMatrix& surroundings)
     */
    int is_in(IntPoint point, std::vector<ATile> list);


    /**
     * A pathfinding algorithm.
     * This calculates the best path between two coordinates on a given array of tiles
     * using the a* algorithm.  This is done by keeping track of an "open" and a "closed"
     * list of tiles.  The open list represents tiles that could potentially be looked at,
     * and the closed list is a list of tiles that have already been looked at.
     *
     * The current tile is the tile that is currently being considered, and every tile around
     * it is added to the open list, assuming that those tiles can be moved through and are
     * not on the closed list.  Each tile added to the open list is given a g, h, and f score,
     * and the curren tile becomes the parent.
     * G represents the distance from the start point to the tile, h (standing for heuristic)
     * is a measure from the tile to the goal, and f = h + g.  If a tile is on the open list,
     * then f is recalculated for the current tile.  If it is lower, then the current tile becomes
     * the new parent.
     *
     * Each loop, the current tile is the one on the open list with the shortest f score.  If the
     * goal is on the open list, or the open list is empty, then the algorithm is stopped.
     * See the source for further documentation.
     * @param start The starting point for the algorithm.
     * @param goal The place the enemy is trying to get to.
     * @param surroundings The surroundings within the sight of the enemy.
     * @return A vector containing a list of IntPoints representing the best path, or an empty vector if there is no path.
     */
    std::vector<IntPoint> a_star(IntPoint start, IntPoint goal, TilePointerMatrix &surroundings, int sight);

    /**
     * A heuristic to estimate the distance from a point to the goal.
     * A helper function for the a-star algorithm, it calculates the
     * distance as the x_change + y_change between a point and the goal.
     * @param current The current point.
     * @param goal The goal the enemy is trying to reach.
     * @return The evaluated value.
     * @see a_star(IntPoint start, IntPoint goal, TilePointerMatrix&surroundings)
     */
    int manhattan(IntPoint, IntPoint);

    /**
     * Returns the ATile with the smallest f value.
     * A helper fucntion for the a-star algorithm which finds the smallest
     * f-value.
     * @param A reference to a list of ATiles.
     * @return The index of the smallest f value found in the list.
     */
    int get_smallest_f(std::vector<ATile>& list);

    /**
     * Gets the direction that an enemy should be spooked.
     * Takes in a chunk and coordinates in the direction of the spooker,
     * and calculates the direction to run away from the the spooker.
     * @param abs_coords The absolute coordinates of the spook-e.
     * @param target_abs The absolute coordinates of the spooker.
     * @return The direction to run away in.
     * @see spooked
     */
    IntPoint get_opposite(IntPoint abs_coords, IntPoint target_abs);


    /**
     * A debugging function.
     * Dumps the paths to cout as they are calculated by a-star.  Only
     * enable if a-star is not working propery or if you like seeing the
     * a-star algrorithm work.  Or if you like massive dumps to cout.
     * Alternatively, you can use it to dump a different matrix.
     * @param map The matrix to dump.
     * @param tl The top left corner of the area to dump.
     * @param br The bottom right corner of the area to dump.
     */
    void dump_matrix(TilePointerMatrix& map, IntPoint tl, IntPoint br);
};

#endif
