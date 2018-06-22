/**
 *  @file ACTOR.CPP
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

#include "actor.h"

Actor::Actor(int _x, int _y, char _sprite, Uint32 col)
{
    x = _x;
    y = _y;
    sprite = _sprite;
    color = col;
}

int Actor::get_x()
{
    return x;
}

int Actor::get_y()
{
    return y;
}

char Actor::get_char()
{
    return sprite;
}

Uint32 Actor::get_color()
{
    return color;
}
