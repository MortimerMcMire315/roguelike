/**
 *  SPRING_MATRIX.H
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


#ifndef _SPRING_MATRIX_H
#define _SPRING_MATRIX_H

#include <vector>
#include <stdlib.h>
#include <iostream>

#include "int_point.h"

/**
 * Creates spring points.
 */

class SpringPoint
{
    private:
        /**
         * The X coordinate.
         */
        int x;

        /**
         * The Y coordinate.
         */
        int y;

        /**
         * The size of the point.
         */
        int size;

        /**
         * The point to the right.
         */
        SpringPoint* right;

        /**
         * The point below.
         */
        SpringPoint* bottom;

    public:

        /**
         * The basic constructor.
         */
        SpringPoint(int _x, int _y, int _size);

        /**
         * Sets the point to the right.
         */
        void add_right_point(SpringPoint* point);

        /**
         * Sets the point below.
         */
        void add_bottom_point(SpringPoint* point);

        /**
         * Returns the distance between this point and another point.
         */
        IntPoint get_distance(SpringPoint* point);

        /**
         * Gets the distance between this point and the one to the right.
         */
        IntPoint get_right_distance();

        /**
         * Gets the distance between this point and the one below it.
         */
        IntPoint get_bottom_distance();

        /**
         * Moves the point by an offset.
         */
        void move(int x_amount, int y_amount);

        /**
         * Accessor for the x coordinate.
         */
        int get_x();

        /**
         * Accessor for the y coordinate.
         */
        int get_y();

        /**
         * Accessor for the right node.
         */
        SpringPoint* get_right();

        /**
         * Accessor for the node below.
         */
        SpringPoint* get_bottom();


};


/**
 * The class for a spring matrix.  A spring matrix is a thing that probably
 * has a real name, but I have no idea what that would be.  So I'm calling it
 * a spring matrix.  It is a 2D grid of points, where each point is connected
 * to each of its neighbors.  Each point can then move randomly within the constraints
 * of the "spring."
 */

class SpringMatrix
{
    private:
        /**
         * The list of points in the matrix.  Maybe should be a 2d vector...?
         */
        std::vector<SpringPoint*> points;

        /**
         * The number of points that can fit on a side.
         */
        IntPoint points_per_side;

        /**
         * The size of the points.
         */
        int point_size;

        /**
         * The minimum space between two points.
         */
        int min;

        /**
         * The maximum space between two points.
         */
        int max;

        /**
         * The initial padding around the entire array.
         */
         int padding;

    public:
        /**
         * Basic constructor.
         */
        SpringMatrix(IntPoint num_points, int _point_size, int _min, int _max, int _padding);

        /**
         * Basic destructor.
         */
        ~SpringMatrix();

        /**
         * Creates the matrix with the paramaters above.
         */
        void construct_matrix();

        /**
         * Deforms the matrix a given number of times.
         * Pretty unnecessary for small matrices.
         */
        void deform_matrix(int num_passes);

        /**
         * Accessor for the matrix.
         */
        std::vector<SpringPoint*>& get_matrix();
};

#endif
