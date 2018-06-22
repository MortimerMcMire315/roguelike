/**
 *  @file ROOM.H
 *  @author Seth A. Yoder
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

#ifndef ROOM_H
#define ROOM_H

#include "int_point.h"

/**
 * A representation of a room in a dungeon. Used most often to place objects in
 * reasonable places.
 */

using namespace std;

class Room
{
    /**
     * @param R the room to output
     * Overloaded output stream operator to easily print a room.
     */
    friend ostream& operator<<(ostream&, const Room &R);
    private:
        /**
         * Prints a description of the points in this room.
         */
        void print() const;

    public:
        /**
         * The top left corner of the room relative to its parent dungeon.
         */
        IntPoint tl;

        /**
         * The bottom left corner of the room relative to its parent dungeon.
         */
        IntPoint br;

        /**
         * The height of the room.
         */
        int height;

        /**
         * The width of the room.
         */
        int width;


        /**
         * Empty constructor.
         */
        Room();

        /**
         * Copy constructor.
         */
        Room(const Room&);

        /**
         * @param _tl the top-left corner of the room relative to its parent
         * dungeon
         * @param _br the bottom-right corner of the room relative to its parent
         * dungeon
         *
         * Creates a new room.
         */
        Room(IntPoint _tl, IntPoint _br);
        Room(int _tl_x, int _tl_y, int _height, int _width);

};

#endif
