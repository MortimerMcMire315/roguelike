/**
 *  @file BUILDING.CPP
 *  @author Michael and Seth Yoder
 *
 *  @section LICENSE
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

#include "building.h"
#include "tileset.h"

Building::Building(){
};


/*
 * For now, the buildings will just be wood.  Mostly because I'm making
 * no progress sitting here thinking about how to do this, and so I should
 * just sit down and actually do it.  I can always add more later.
 */
Building::Building(int x, int y, int _height, int _width)
{
    tl = IntPoint(y, x);
    height = _height;
    width = _width;
    floor_plan.resize(height);
    floor = Tileset::get("WOOD_FLOOR");
    wall = Tileset::get("WOOD_WALL");
    owner = NULL;
    for(int i=0;i<floor_plan.size();i++)
    {
        floor_plan[i].resize(width);
    }
    house_from_bst();
}

void Building::house_from_bst()
{
    //convert it into rooms
    BSpaceTree house = BSpaceTree(width-1, height-1, 4, 8);
    rooms_from_tree(house);
    //turn it into a floor_plan
    rooms_to_floor();
    //add some doors
    bool a = add_doors(house);
}

void Building::rooms_from_tree(BSpaceTree& tree)
{
    std::vector<BSpaceNode*> nodes = tree.get_leaves();
    for(int i=0; i<nodes.size();i++)
    {
        rooms.push_back(Room(nodes[i]->tl_x, nodes[i]->tl_y, nodes[i]->height, nodes[i]->width));
    }
}

void Building::rooms_to_floor()
{
    for(int i=0;i<rooms.size();i++)
    {
        Room room = rooms[i];
        for(int j=room.tl.row;j<=room.height + room.tl.row;j++)
        {
            for(int k=room.tl.col;k<=room.width + room.tl.col;k++)
            {
                if(j == room.tl.row || j == room.height+room.tl.row || k == room.tl.col || k == room.width+room.tl.col)
                {

                    floor_plan[j][k] = wall;
                }
                else
                {
                    if(floor_plan[j][k] != wall)
                    {
                        floor_plan[j][k] = floor;
                    }
                }
            }
        }
    }
}

bool Building::add_doors(BSpaceTree& bst)
{
    connect_nodes(bst.get_root());
    add_random_door(bst.get_root());
    add_random_door(bst.get_root());
    return true;
}

void Building::connect_nodes(BSpaceNode* node)
{
    if(node->left != NULL && node->right != NULL)
    {
        //connect the two
        int x_coord;
        int y_coord;
        if(node->left->tl_y + node->left->height == node->right->tl_y)
        {
            do
            {
                x_coord = rand() % (node->left->width-2) + node->left->tl_x + 1;
                y_coord = node->right->tl_y;
            } while(surrounding_walls(y_coord, x_coord) != 2);
        }
        else if(node->left->tl_x + node->left->width == node->right->tl_x)
        {
            do
            {
                y_coord = rand() % (node->left->height-2) + (node->left->tl_y + 1);
                x_coord = node->right->tl_x;
            } while(surrounding_walls(y_coord, x_coord) != 2);
        }
        floor_plan[y_coord][x_coord] = Tileset::get("DOOR");
        connect_nodes(node->left);
        connect_nodes(node->right);
    }
}


void Building::add_random_door(BSpaceNode* node)
{
    bool rand_x = rand() % 2;
    int x;
    int y;
    do
    {
        if(rand_x)
        {
            x = rand() % (width - 2) + 1;
            y = rand() % 2;
            if(y != 0)
            {
                y = height - 1;
            }
        }
        else
        {
            y = rand() % (height - 2) + 1;
            x = rand() % 2;
            if(x != 0)
            {
                x = width - 1;
            }
        }
    } while(surrounding_walls(y, x) != 2);
    floor_plan[y][x] = Tileset::get("DOOR");
}


int Building::surrounding_walls(int y, int x)
{
    int accum = 0;
    if(y+1 < height && floor_plan[y+1][x] == wall)
    {
        accum++;
    }
    if(y-1 >= 0 && floor_plan[y-1][x] == wall)
    {
        accum++;
    }
    if(x+1 < width && floor_plan[y][x+1] == wall)
    {
        accum++;
    }
    if(x-1 >= 0 && floor_plan[y][x-1] == wall)
    {
        accum++;
    }
    return accum;
}

Tile Building::get_floor()
{
    return floor;
}

Tile Building::get_wall()
{
    return wall;
}

TileMatrix& Building::get_floor_plan()
{
    return floor_plan;
}

int Building::get_x()
{
    return tl.col;
}

int Building::get_y()
{
    return tl.row;
}

int Building::get_width()
{
    return width;
}

int Building::get_height()
{
    return height;
}

Tile Building::tile_at(int y, int x)
{
    return floor_plan[y][x];
}

Tile* Building::tile_pointer_at(int y, int x)
{
    return &floor_plan[y][x];
}

void Building::add_owner(Character* chara)
{
    owner = chara;
}

Character* Building::get_owner()
{
    return owner;
}
