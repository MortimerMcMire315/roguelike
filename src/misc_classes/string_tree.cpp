/**
 *  @file STRING_TREE.CPP
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

#include "string_tree.h"

StringTreeNode::StringTreeNode()
{
    //do nothing
}

StringTreeNode::StringTreeNode(std::string _statement, std::vector<StringTreeNode*> _nodes, void (*_callback)) {
    statement = _statement;
    nodes = _nodes;
    callback = _callback;
}

std::string StringTreeNode::get_statement() {
    return statement;
}

StringTreeNode* StringTreeNode::get_node(int index) {
    if(index < nodes.size() && index >= 0) {
        return nodes[index];
    } else {
        return NULL;
    }
}

void do_callback(Character* main, Character* second, Game* game) {
    if(callback != NULL) {
        callback(main, second, game);
    }
}
