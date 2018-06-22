/**
 *  BEHAVIOR_ACTOR.H
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

#ifndef _BEHAVIOR_ACTOR_H
#define _BEHAVIOR_ACTOR_H

#include "character.h"
#include "behavior_node.h"

class BNode;

/**
 * The actor class for behavior trees.  Holds information about a character
 * and that character's place in the tree.
 */
class BActor
{
    private:
        /**
         * The character associated with this actor.
         */
        Character* chara;

       /**
        * Someday, perhaps, when I thread things, this will do something.
        * For the nonce, it is a placeholder.
        */
       BNode* running_node;

    public:

        /**
         * The empty constructor.
         */
        BActor();

        /**
         * The default constructor.
         */
        BActor(Character* _chara);


       /**
        * Accessor for the actor.
        */
       Character* get_character();

       /**
        * Checks whether or not enough time has elapsed for the character
        * to act.
        */
       bool should_tick(long delta_ms);
};

#endif
