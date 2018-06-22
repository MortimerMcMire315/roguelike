/**
 *  BEHAVIOR_TREE.H
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

#ifndef _BEHAVIOR_TREE_H
#define _BEHAVIOR_TREE_H

#include <vector>

#include "behavior_node.h"
#include "behavior_actor.h"

/**
 * Oh boy, let's get started on behavior trees.
 * The way that behavior trees work is as a collection of nodes to make
 * up a specific behavior.  For each actor in the tree, the tree is
 * traversed, and the behavior is decided based on the traversal of nodes.
 */
class BehaviorTree
{
    private:
        /**
         * The root node of the tree.
         */
        BNode* root;

        /**
         * The game that this tree should use.
         */
        Game* game;

        /**
         * The id of the tree.
         */
        int id;

        /**
         * The actors in the tree.
         */
        std::vector<BActor> actors;

        /**
         * Deletes the nodes in the tree.
         */
        void delete_all(BNode* node);

    public:
        /**
         * The default constructor.
         */
        BehaviorTree(BNode* node, Game* _game, int _id);

        /**
         * The copy constructor.
         */
        BehaviorTree(const BehaviorTree& tree);

        /**
         * The assignment operator.
         */
        BehaviorTree& operator=(const BehaviorTree& tree);

        /**
         * Deep copies the tree.
         */
        BNode* copy_tree(BNode* node);

        /**
         * The destructor.
         */
        ~BehaviorTree();

        /**
         * Adds an actor to the tree.
         */
        void add_actor(BActor actor);

        /**
         * Runs all of the actors in the tree.
         */
        void run_actors(long delta_ms);

        /**
         * Removes the actor with a particular character.
         */
        void remove_actor(Character* chara);

        /**
         * Accessor for the id.
         */
        int get_id();
};

#endif
