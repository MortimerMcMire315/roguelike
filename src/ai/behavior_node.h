/**
 *  BEHAVIOR_NODE.H
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

#ifndef _BEHAVIOR_NODE_H
#define _BEHAVIOR_NODE_H

#include <vector>
#include <assert.h>

#include "game.h"
#include "behavior_actor.h"

class BActor;

enum NODE_STATES
{
    FAILURE,
    SUCCESS,
    RUNNING,
    DEAD
};

/**
 * Defines the different nodes for a behavior tree.
 */
class BNode
{
    protected:
        /**
         * The children nodes of the object.
         */
        std::vector<BNode*> nodes;
    public:

        void add_child(BNode* node, int index);

        /**
         * Public accessor for the children nodes.
         */
        std::vector<BNode*> get_children();

        /**
         * Ticks the node forward once.
         * TODO MAKE THIS VIRTUAL AGAIN
         */
        virtual int tick(BActor actor, Game* game)=0;

        /**
         * Because wth c++.
         */
        virtual BNode* clone()=0;
};

//-------------------------COMPOSITE NODES------------------//

/**
 * A composite node responsible for running nodes in a sequence.
 * It will call the child nodes until one of them returns false.
 */
class SequenceNode : public BNode
{
    public:
        SequenceNode(std::vector<BNode*> _nodes);
        int tick(BActor actor, Game* game);
        SequenceNode* clone();
};

/**
 * A composite nodes responsible for running nodes in order until
 * one returns true.
 */
class PriorityNode : public BNode
{
    public:
        PriorityNode(std::vector<BNode*> nodes);
        int tick(BActor actor, Game* game);
        PriorityNode* clone();
};

/**
 * A class to emulate if/else statements, which is dangerous territory
 * as it could become overrused.  The problem now, however, is that if
 * you want a condition to execute one of two behaviors depending on
 * success or failure, you have to put a priority with the two behaviors
 * you want, and then put the condition at the beginning of the first node,
 * so that if it fails you move to the second.  However, if the node
 * were to fail for another reason (like, for instance, a character
 * being unable to move), then you would move onto the second one anyway.
 * This is a problem, so I made a branching conditional.  It will execute
 * the first node if the condition returns success, otherwise it will
 * execute the second.
 */
class BranchingCondition : public BNode
{
    public:
        BranchingCondition(BNode* condition, BNode* success, BNode* failure);
        int tick(BActor actor, Game* game);
        BranchingCondition* clone();
};

//---------------------------DECORATOR NODES---------------------//
/**
 * Inverts whatever is returned from the child node.
 */
class InverterNode : public BNode
{
    public:
        InverterNode(BNode* node);
        int tick(BActor actor, Game* game);
        InverterNode* clone();
};


//-------------------------------ACTION NODES---------------------//

class MoveTowards : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        MoveTowards* clone();
};


class MoveTowardsM : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        MoveTowardsM* clone();
};

class MoveAway : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        MoveAway* clone();
};

class Attack : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        Attack* clone();
};

class Wander : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        Wander* clone();
};

class FreakOut : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        FreakOut* clone();
};

class Die : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        Die* clone();
};

class TurnToward : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        TurnToward* clone();
};

class TurnTowardM : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        TurnTowardM* clone();
};

class TurnAway : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        TurnAway* clone();
};



//------------------------------CONDITION NODES---------------------//

class LowHealth : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        LowHealth* clone();
};

class EnemyInRange : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        EnemyInRange* clone();
};

class GetMTarget : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        GetMTarget* clone();
};

class NextTo : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        NextTo* clone();
};

class NextToM : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        NextToM* clone();
};

class HasHealth : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        HasHealth* clone();
};

class MHealthChange : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        MHealthChange* clone();
};

class InWorld : public BNode
{
    public:
        int tick(BActor actors, Game* game);
        InWorld* clone();
};

class ValidTarget : public BNode
{
    public:
        int tick(BActor actor, Game* game);
        ValidTarget* clone();
};

#endif
