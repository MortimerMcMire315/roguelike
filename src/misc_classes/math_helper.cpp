/**
 *  MATH_HELPER.CPP
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

#include "math_helper.h"

double perc_to_rad(int percent)
{
    return (PI * 2) * (double)percent/100;
}


double coords_to_rad(IntPoint coords)
{
    double rads = atan2((double)coords.row, (double)coords.col);
    //convert rads to 0-2PI instead of -PI-PI, because that's stupid
    if(rads < 0)
    {
        rads += (2 * PI);
    }
    return rads;
}

int coords_to_perc(IntPoint coords)
{
    return rad_to_perc(coords_to_rad(coords));
}

int rad_to_perc(double rad)
{
    double perc = (rad/(2 * PI)) * 100;
    return (int)perc;
}
