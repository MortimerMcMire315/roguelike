/**
 *  @file STRING_TREE.H
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

#ifndef STRINGTREE_H
#define STRINGTEE_H

#include <string>
#include <stdlib.h>
#include <vector>

#include "character.h"
#include "game.h"

/**
 * A generic interface that stores a string
 * in a tree structure.  Is used for dialogue, and documentation
 * will operate under this assumption.
 * At some point, this could (and probably will be) abstrated out
 * to only have one string at each level--each node will represent
 * possible responses by either party.
 */
class StringTreeNode
{
    public:
        /**
         * Constructs a message board.  Private so that it can't
         * be called outside of the messageboard class.
         */
        StringTreeNode(std::string _statement, std::vector<StringTreeNode*> _nodes, void (*_callback) = NULL);

        /**
         * Returns the statement.
         */
        std::string get_statement();

        /**
         * Gets a particular node object.
         */
        StringTreeNode* get_node(int index);

        /**
         * Call for callback (ha), first checking
         * to make sure it exists.
         */
        void do_callback(Character* main, Character* second, Game* game);


    private:
        /**
         * The initial statement made by a character.
         */
        std::string statement;

        /**
         * The possible dialogue options that this
         * response leads the player too.
         */
        std::vector<StringTreeNode*> nodes;

        /**
         * Callback function.
         * When you hit a leaf node, something needs to happen.
         * This is what it will be. Sorry.
         */
        void (*callback)(Character* main, Character* second, Game* game);
};

#endif
