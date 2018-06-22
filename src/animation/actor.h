/**
 *  @file ACTOR.H
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

#ifndef __ACTOR_H
#define __ACTOR_H

#include <SDL/SDL.h>

/**
 * A basic actor in an animation scene!
 */
class Actor
{
    private:
        /**
         * The x coordinate of the actor (relative to the coords of the
         * animation).
         */
        int x;

        /**
         * The y coordinate of the actor (relative to the coords of the
         * animation).
         */
        int y;

        /**
         * The sprite of the animation...wait...this is a char! Hurray
         * for implicit conversions.  Note: change this to an int.
         */
        char sprite;

        /**
         * The color of the actor.  Whoa, that sounds a little racey.
         */
        Uint32 color;
    public:

        /**
         * Constructor for the actor.
         */
        Actor(int _x, int _y, char _sprite, Uint32 col);

        /**
         * Basic accessors.
         */
        int get_x();
        int get_y();
        char get_char();
        Uint32 get_color();
};

#endif
