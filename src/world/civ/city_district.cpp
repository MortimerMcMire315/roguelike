/**
 *  @file CITY_DISTRICT.CPP
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

#include "city_district.h"

CityDistrict::CityDistrict(int chunk_x, int chunk_y, int _height, int _width, MapTile _type)
{
    chunk = IntPoint(chunk_y, chunk_x);
    height = _height;
    width = _width;
    type = _type;
    //first thing we need to do it divide it into city blocks
    //so we can have roads running through it.
    BSpaceTree div = BSpaceTree(width, height, 30, 70);
    settlement_from_bst(div);
}

void CityDistrict::settlement_from_bst(BSpaceTree& bst)
{
    blocks_from_bst(bst);
}

void CityDistrict::blocks_from_bst(BSpaceTree& tree)
{
    std::vector<BSpaceNode*> leaves = tree.get_leaves();
    for(int i=0;i<leaves.size();i++)
    {
        int rand_x = rand() % 3 + 1;
        int rand_y = rand() % 3 + 1;
        int new_x = rand_x + leaves[i]->tl_x;
        int new_y = rand_y + leaves[i]->tl_y;
        int new_height = leaves[i]->height - (rand() % 3 + rand_y + 1);
        int new_width = leaves[i]->width - (rand() % 3 + rand_x + 1);
        blocks.push_back(Block(new_x, new_y, new_height, new_width));
        //blocks.push_back(Block(leaves[i]->tl_x, leaves[i]->tl_y, leaves[i]->height, leaves[i]->width));
    }
}

std::vector<Block>& CityDistrict::get_blocks()
{
    return blocks;
}

