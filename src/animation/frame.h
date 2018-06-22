/**
 *  @file FRAME.H 
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

#ifndef __FRAME_H
#define __FRAME_H

#include <vector>
#include "actor.h"

/**
 * The class for a single frame in an animation.
 */

class Frame
{
    private:
        /**
         * The list of actors in this frame.
         */
        std::vector<Actor> actors;

    public:
        /**
         * Constructor which takes in a list of actors.
         */
        Frame(std::vector<Actor> _actors);
        
        /**
         * Blank constructor.
         */
        Frame();
        
        /**
         * Returns a list of actors.
         */
        std::vector<Actor> get_actors();
        
        /**
         * Adds an actor.
         */
        void add_actor(Actor actor);
        
        /**
         * Returns a particular actor.
         */
        Actor actor(int a);
};

#endif
