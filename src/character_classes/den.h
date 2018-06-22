/**
 *  @file DEN.H
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

#ifndef _DEN_H
#define _DEN_H

#include "defs.h"

typedef std::vector<std::vector<Tile> > TileMatrix;

class Den
{
    private:
        int x;
        int y;
        int height;
        int width;
        TileMatrix ground;
    public:
        Den(int _x, int _y, int _height, int _width, TileMatrix _ground);
        Den(int _x, int _y, Tile sprite);
        int get_x();
        int get_y();
        int get_height();
        int get_width();
        Tile tile_at(int y, int x);
        Tile* tile_pointer_at(int y, int x);
        TileMatrix& get_ground();
};

#endif
