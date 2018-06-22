/**
 *  BRESENHAM.CPP
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

#include "bresenham.h"

/*
 * A Bresenham's line algorithm for plotting lines in discrete space.
 *
 * PRE: Will be given a starting point and an ending point.
 * POST: Will return an IntPoint array representing a line of points between the
 * starting and ending point.
 */
std::vector<IntPoint> bresenham_line(IntPoint& first, IntPoint& second) {
    std::vector<IntPoint> points = std::vector<IntPoint>();
    int x0 = first.col;
    int y0 = first.row;
    int x1 = second.col;
    int y1 = second.row;

    bool steep = (abs(y1 - y0) > abs(x1 - x0));

    if(steep) {
        std::swap(x0,y0);
        std::swap(x1, y1);
    }

    int deltax = abs(x1 - x0);
    int deltay = abs(y1 - y0);
    int error = deltax / 2;
    int y = y0;
    int inc;
    if (x0 < x1) {
        inc = 1;
    } else {
        inc = -1;
    }

    int ystep;
    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for(int x = x0; x != x1; x += inc) {
        IntPoint current_point;
        if(steep) {
            //NOTE: This is like saying IntPoint(col, row)
            current_point = IntPoint(x, y);
        } else {
            current_point = IntPoint(y, x);
        }

        points.push_back(current_point);

        error = error - deltay;
        if (error < 0) {
            y = y + ystep;
            error = error + deltax;
        }
    }

    //if(!steep) {
        //cout<<"REVERSED"<<endl;
        //std::reverse(points.begin(), points.end());
    //}

    return points;
}

std::vector<IntPoint> bresenham_circle(IntPoint& start, int radius) {
    std::vector<IntPoint> points = std::vector<IntPoint>();
    int x0 = start.row;
    int y0 = start.col;

    int x = radius;
    int y = 0;
    int radius_error = 1 - x;

    while(x >= y) {
        points.push_back(IntPoint(x + x0, y + y0));
        points.push_back(IntPoint(y + x0, x + y0));
        points.push_back(IntPoint(-x + x0, y + y0));
        points.push_back(IntPoint(-y + x0, x + y0));
        points.push_back(IntPoint(-x + x0, -y + y0));
        points.push_back(IntPoint(-y + x0, -x + y0));
        points.push_back(IntPoint(x + x0, -y + y0));
        points.push_back(IntPoint(y + x0, -x + y0));

        y++;
        if(radius_error < 0) {
            radius_error += (2*y + 1);
        } else {
            x--;
            radius_error += (2*(y-x + 1));
        }
    }

    return points;
}

std::vector<IntPoint> bresenham_arc(IntPoint& start, int radius, IntPoint bounds)
{

    //establish the bounds
    double upper_bounds = perc_to_rad(bounds.row);
    double lower_bounds = perc_to_rad(bounds.col);

    //draw a circle
    std::vector<IntPoint> points = std::vector<IntPoint>();
    std::vector<IntPoint> temp_points = std::vector<IntPoint>();
    int x0 = start.row;
    int y0 = start.col;

    int x = radius;
    int y = 0;
    int radius_error = 1 - x;

    while(x >= y) {


        temp_points.push_back(IntPoint(x + x0, y + y0));
        temp_points.push_back(IntPoint(y + x0, x + y0));
        temp_points.push_back(IntPoint(-x + x0, y + y0));
        temp_points.push_back(IntPoint(-y + x0, x + y0));
        temp_points.push_back(IntPoint(-x + x0, -y + y0));
        temp_points.push_back(IntPoint(-y + x0, -x + y0));
        temp_points.push_back(IntPoint(x + x0, -y + y0));
        temp_points.push_back(IntPoint(y + x0, -x + y0));
        for(int i=0;i<temp_points.size();i++)
        {
            IntPoint point = temp_points[i];
            IntPoint difference = point - start;
            double rads = coords_to_rad(difference);
            if(rads <= upper_bounds && rads >= lower_bounds)
            {
                points.push_back(point);
            }
        }
        temp_points.clear();
        y++;
        if(radius_error < 0) {
            radius_error += (2*y + 1);
        } else {
            x--;
            radius_error += (2*(y-x + 1));
        }
    }

    return points;
}
