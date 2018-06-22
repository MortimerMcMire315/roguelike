/**
 *  @file BLOCK.H
 *  @author Michael and Seth Yoder
 *
 *  @section LICENSE
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

#ifndef _BLOCK_H
#define _BLOCK_H

#include "int_point.h"
#include "building.h"

class Block
{
    public:
        /**
         * The top left corner of the block relative to the settlement.
         */
        IntPoint tl;

        /**
         * The height of the room.
         */
        int height;

        /**
         * The width of the room.
         */
        int width;

        /**
         * A list of buildings inside of the block.
         */
        std::vector<Building> buildings;

        /**
         * Empty constructor.
         */
        Block();

        /**
         * Copy constructor.
         */
        Block(const Block&);

        /**
         * @param _tl the top-left corner of the room relative to the settlment
         * @param _br the bottom-right corner of the room relative to its parent
         * dungeon
         *
         * Creates a new Block.
         */
        Block(int _tl_x, int _tl_y, int _height, int _width);

        /**
         * Generates houses inside of the block.
         */
        void generate_buildings();

        /**
         * Accessor for buildings.
         */
        std::vector<Building>& get_buildings();

        /**
         * Accessor for the height.
         */
        int get_height();

        /**
         * Accessor for the width.
         */
        int get_width();

        /**
         * Accessor for the x coord.
         */
        int get_x();

        /**
         * Accessor for the y coord.
         */
        int get_y();



};

#endif
