/**
 *  @file ANIMATION_DEFS.CPP
 *  @author Michael Yoder
 *
 *  @section LICENSE
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

#include "animation_defs.h"

Animation construct_explosion(int x, int y, int chunk_x, int chunk_y, int size, Uint32 color)
{
    Animation anim = Animation(70, size, x, y, chunk_x, chunk_y);
    //iterate through the frames.
    for(int i=0;i<size;i++)
    {

        //make a circle with the same radius as the frame number (frame 3 has a radius of 3)
        IntPoint start = IntPoint(0, 0);
        std::vector<IntPoint> points = bresenham_circle(start, i);
        for(int j=0;j<points.size();j++)
        {
            //for each point in the circle, make an actor and add it to the frame
            Actor a = Actor(points[j].col, points[j].row, '*', color);
            anim.add_actor_to_frame(a, i);
        }
    }

    return anim;
}

