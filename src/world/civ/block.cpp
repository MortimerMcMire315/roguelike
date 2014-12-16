/**
 *  @file BLOCK.CPP
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

#include <block.h>

Block::Block() {

}

Block::Block(const Block& r) {
    tl = r.tl;
    height = r.height;
    width = r.width;
    buildings = r.buildings;
}

Block::Block(int tl_x, int tl_y, int _height, int _width)
{
    tl = IntPoint(tl_y, tl_x);
    height = _height;
    width = _width;
    
    generate_buildings();
}

void Block::generate_buildings()
{
    //for now, statically sized houses.  We'll revisit this later
    BSpaceTree houses = BSpaceTree(height, width, 15, 20);
    
    //just get the base leaves.
    std::vector<BSpaceNode*> house_nodes = houses.get_leaves(); 
    for(int i=0;i<house_nodes.size();i++)
    {
        int rand_x = rand() % 5 + 1;
        int rand_y = rand() % 4 + 1;

        int new_x = rand_x + house_nodes[i]->tl_x;
        int new_y = rand_y + house_nodes[i]->tl_y;
        int new_height = house_nodes[i]->height - (rand() % 4 + rand_y + 1);
        int new_width = house_nodes[i]->width - (rand() % 5 + rand_x + 1);
        buildings.push_back(Building(new_x, new_y, new_height, new_width));
        std::cout<<"X: "<<new_x<<", Y: "<<new_y<<", Height: "<<new_height<<", Width: "<<new_width<<std::endl;
        std::cout<<"NODE: X: "<<house_nodes[i]->tl_x<<", Y: "<<house_nodes[i]->tl_y<<", Height: "<<house_nodes[i]->height<<", Width: "<<house_nodes[i]->width<<std::endl;
    }
}

std::vector<Building>& Block::get_buildings()
{
    return buildings;
}

int Block::get_height()
{
    return height;
}

int Block::get_width()
{
    return width;
}

int Block::get_x()
{
    return tl.col;
}

int Block::get_y()
{
    return tl.row;
}
