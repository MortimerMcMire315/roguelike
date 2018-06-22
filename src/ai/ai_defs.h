/**
 *  @file AI_DEFS.H
 *  @author Michael Yoder, Seth Yoder
 *
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

#ifndef _AI_DEFS_H
#define _AI_DEFS_H

#include <vector>

#include "behavior_tree.h"
#include "behavior_node.h"
#include "behavior_actor.h"
#include "game.h"

namespace ai
{

    BNode* DeathNode();
    BehaviorTree GENERIC_AGGRESSIVE(Game* game);
    BehaviorTree GENERIC_PASSIVE(Game* game);
    BehaviorTree FOLLOW(Game* game);
    BehaviorTree NPC(Game* game);
    //extern BehaviorTree GENERIC_PASSIVE;
}

#endif
