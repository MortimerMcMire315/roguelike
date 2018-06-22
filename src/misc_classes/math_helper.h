/**
 *  MATH_HELPER.H
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


#ifndef MATH_HELPER_H
#define MATH_HELPER_H
#include "math.h"
#include "int_point.h"

const double PI = 3.1415926;

/**
 * Converts percent into radians.
 * @param percent The percent to convert.
 * @return The result in radians.
 */
double perc_to_rad(int percent);

/**
 * Converts coordinates into radians.
 * @param coords The coordinates to convert.
 * @return The result in radians.
 */
double coords_to_rad(IntPoint coords);

/**
 * Converts coordinates into a percentage (1-100)
 * @parameter coords The coords to convert
 * @return The result as a percent.
 */
int coords_to_perc(IntPoint coords);

/**
 * Converts radians into a percentage.
 * @parameter rad The radians to convert.
 * @return The result as a percent.
 */
int rad_to_perc(double rad);

#endif
