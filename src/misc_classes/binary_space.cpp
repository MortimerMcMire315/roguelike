/**
 *  BINARY_SPACE.CPP
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

#include "binary_space.h"


/****************** NODES ***************************/
BSpaceNode::BSpaceNode(int _tl_x, int _tl_y, int _width, int _height)
{
    tl_x = _tl_x;
    tl_y = _tl_y;
    height = _height;
    width = _width;
    left = NULL;
    right = NULL;
}

void BSpaceNode::resize(int new_x, int new_y, int new_width, int new_height)
{
    tl_x = new_x;
    tl_y = new_y;
    width = new_width;
    height = new_height;
}

/****************** TREES **************************/


BSpaceTree::BSpaceTree(int _height, int _width, int _min_size, int _max_size)
{
    height = _height;
    width = _width;
    min_size = _min_size;
    max_size = _max_size;
    root = new BSpaceNode(0, 0, height, width);
    generate_tree(root);
}

BSpaceTree::~BSpaceTree()
{
    delete_all(root);
}

void BSpaceTree::delete_all(BSpaceNode* node)
{
    if(node != NULL)
    {
        if(node->left != NULL)
        {
            delete_all(node->left);
        }
        if(node->right != NULL)
        {
            delete_all(node->right);
        }
        delete node;
    }
}

void BSpaceTree::generate_tree(BSpaceNode* node)
{
    //if splitting the node was successful, do the same to the newly
    //generated child nodes, if they're too big or with a 75% chance.
    if(split_node(node) != false)
    {
        if(node->left->width > max_size || node->left->height > max_size || rand() % 100 > 25)
        {
            generate_tree(node->left);
        }
        if(node->right->width > max_size || node->right->height > max_size || rand() % 100 > 25)
        {
            generate_tree(node->right);
        }
    }
}


bool BSpaceTree::split_node(BSpaceNode* node)
{
    //if the nodes exist, return
    if(node->left != NULL || node->right != NULL)
    {
        return false;
    }

    //check which direction we're splitting.
    //if the height and width are similar, pick one randomly.
    bool splitH = rand() % 2;
    if((float)node->height/(float)node->width < .5 || (node->height<max_size && node->width>max_size))
    {
        splitH = false;
    }
    else if((float)node->width/(float)node->height < .5 || (node->height>max_size && node->width<max_size))
    {
        splitH = true;
    }
    //check to see if we have enough space to split
    int max = (splitH ? node->height : node->width) - min_size;
    if(max <= min_size)
    {
        return false;
    }

    //get a space to split on
    int split = rand() % (max - min_size) + min_size;

    //split the node
    if(splitH)
    {
        node->left = new BSpaceNode(node->tl_x, node->tl_y, node->width, split);
        node->right = new BSpaceNode(node->tl_x, node->tl_y + split, node->width, node->height - split);
    }
    else
    {
        node->left = new BSpaceNode(node->tl_x, node->tl_y, split, node->height);
        node->right = new BSpaceNode(node->tl_x + split, node->tl_y, node->width - split, node->height);
    }
    return true;
}

std::vector<BSpaceNode*>& BSpaceTree::get_leaves()
{
    rec_get_leaves(leaves, root);
    return leaves;
}

void BSpaceTree::rec_get_leaves(std::vector<BSpaceNode*>& vec, BSpaceNode* node)
{
    if(node->left == NULL && node->right == NULL)
    {
        vec.push_back(node);
    }
    else
    {
        rec_get_leaves(vec, node->left);
        rec_get_leaves(vec, node->right);
    }
}


void BSpaceTree::dump_all()
{
    std::cout<<(print_tree(root, 0))<<std::endl;
}

std::string BSpaceTree::print_tree(BSpaceNode* node, int accum)
{
    std::stringstream ss;
    for(int i=0;i<accum;i++)
    {
        if(i == accum - 1)
        {
            ss<<"  |-->";
        }
        else
        {
            ss<<"  |   ";
        }
    }
    ss<<"X: "<<node->tl_x<<", Y: "<<node->tl_y<<", Height: "<<node->height<<", Width: "<<node->width<<"\n";
    if(node->left == NULL || node->right == NULL)
    {
        return ss.str();
    }
    else
    {
        return ss.str() +  print_tree(node->left, accum + 1) + print_tree(node->right, accum + 1);
    }
}




BSpaceNode* BSpaceTree::get_root()
{
    return root;
}
