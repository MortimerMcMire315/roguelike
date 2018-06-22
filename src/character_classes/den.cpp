/**
 *  @file DEN.CPP
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

#include "den.h"

Den::Den(int _x, int _y, int _height, int _width, TileMatrix _ground)
{
    x = _x;
    y = _y;
    height = _height;
    width = _width;
    ground = _ground;
}

Den::Den(int _x, int _y, Tile sprite)
{
    ground.resize(1);
    ground[0].resize(1);
    ground[0][0] = sprite;
    x = _x;
    y = _y;
    height = 1;
    width = 1;
}

int Den::get_x()
{
    return x;
}

int Den::get_y()
{
    return y;
}

int Den::get_height()
{
    return height;
}

int Den::get_width()
{
    return width;
}

TileMatrix& Den::get_ground()
{
    return ground;
}

Tile Den::tile_at(int y, int x)
{
    return ground[y][x];
}

Tile* Den::tile_pointer_at(int y, int x)
{
    return &ground[y][x];
}
