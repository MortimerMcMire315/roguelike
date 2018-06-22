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

#include "spring_matrix.h"

SpringPoint::SpringPoint(int _x, int _y, int _size)
{
    x = _x;
    y = _y;
    size = _size;
    right = NULL;
    bottom = NULL;
}

void SpringPoint::add_right_point(SpringPoint* point)
{
    right = point;
}

void SpringPoint::add_bottom_point(SpringPoint* point)
{
    bottom = point;
}

IntPoint SpringPoint::get_distance(SpringPoint* point)
{
    if(point != NULL)
    {
        return IntPoint((point->get_y() - y) - size, (point->get_x() - x) - size);
    }
    return IntPoint(0, 0);
}


IntPoint SpringPoint::get_right_distance()
{
    return get_distance(right);
}

IntPoint SpringPoint::get_bottom_distance()
{
    return get_distance(bottom);
}

void SpringPoint::move(int x_amount, int y_amount)
{
    x += x_amount;
    y += y_amount;
}

int SpringPoint::get_x()
{
    return x;
}

int SpringPoint::get_y()
{
    return y;
}

SpringPoint* SpringPoint::get_right()
{
    return right;
}

SpringPoint* SpringPoint::get_bottom()
{
    return bottom;
}

//------------------------Spring Matrix-----------------------------//

SpringMatrix::SpringMatrix(IntPoint num_points, int _point_size, int _min, int _max, int _padding)
{
        points_per_side = num_points;
        point_size = _point_size;
        min = _min;
        max = _max;
        padding = _padding;
        points = std::vector<SpringPoint*>();
        construct_matrix();
}

SpringMatrix::~SpringMatrix()
{
    for(int i=0;i<points.size();i++)
    {
        delete points[i];
    }
}

void SpringMatrix::construct_matrix()
{
    int space = (min + max)/2;
    int space_per_point = space + point_size;
    int y_size = (2 * padding) + space_per_point * points_per_side.row;
    int x_size = (2 * padding) + space_per_point * points_per_side.col;
    int point_num = 0;
    for(int i=padding;i<y_size - padding;i+=space_per_point)
    {
        for(int j=padding;j<x_size - padding;j+=space_per_point)
        {
            SpringPoint* current = new SpringPoint(j, i, point_size);
            SpringPoint* left;
            SpringPoint* top;
            if(j>padding)
            {
                left = points[point_num - 1];
                left->add_right_point(current);
            }
            if(point_num - points_per_side.row > 0)
            {
                top = points[point_num - points_per_side.row - 1];
                top->add_bottom_point(current);
            }
            points.push_back(current);
            point_num++;
        }
    }
}

void SpringMatrix::deform_matrix(int num_passes)
{
    if(num_passes <= 0)
    {
        return;
    }
    for(int i=0;i<points.size();i++)
    {
        SpringPoint* point = points[i];
        SpringPoint* right = point->get_right();
        SpringPoint* bottom = point->get_bottom();
        //deform the x
        if(right != NULL)
        {
            int x_dist = rand() % (max - min + 1) + min;
            IntPoint x_current = point->get_right_distance();

            int x_dif = x_current.col - x_dist;
            int cur_change = x_dif/2;
            int right_change = (x_dif - cur_change) * - 1;
            point->move(cur_change, 0);
            right->move(right_change, 0);
        }
        if(bottom != NULL)
        {
            int y_dist = rand() % (max - min + 1) + min;
            IntPoint y_current = point->get_bottom_distance();

            int y_dif = y_current.row - y_dist;
            int cur_change = y_dif/2;
            int bottom_change = (y_dif - cur_change) * - 1;
            point->move(0, cur_change);
            bottom->move(0, bottom_change);
        }
    }
    deform_matrix(num_passes - 1);
}

std::vector<SpringPoint*>& SpringMatrix::get_matrix()
{
    return points;
}
