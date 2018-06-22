/**
 *  BRESENHAM.H
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

#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <vector>

#include "math_helper.h"
#include "int_point.h"

std::vector<IntPoint> bresenham_line(IntPoint&, IntPoint&);
std::vector<IntPoint> bresenham_circle(IntPoint&, int);
std::vector<IntPoint> bresenham_arc(IntPoint& start, int radius, IntPoint bounds);
