/**
 *  @file FRAME.CPP
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

#include "frame.h"

Frame::Frame(std::vector<Actor> _actors)
{
    actors = _actors;
}

Frame::Frame()
{
    actors = std::vector<Actor>();
}

std::vector<Actor> Frame::get_actors()
{
    return actors;
}

void Frame::add_actor(Actor actor)
{
    actors.push_back(actor);
}

Actor Frame::actor(int a)
{
    return actors[a];
}
