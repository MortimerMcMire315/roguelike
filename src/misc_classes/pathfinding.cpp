/**
 *  PATHFINDING.H
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

#include "pathfinding.h"

//TODO clean this up and optimize it at some point.
IntPoint pathfinding::get_next_step(IntPoint goal, TilePointerMatrix& surroundings, IntPoint cur_coords, int sight)
{

    return dumb_path(goal, surroundings, cur_coords);
    /**
    std::vector<IntPoint> path = a_star(cur_coords, goal, surroundings, sight);
    if(path.size()>0)
    {
        IntPoint buffer_step = path[path.size() - 1];
        IntPoint next_step = IntPoint(buffer_step.row - cur_coords.row, buffer_step.col - cur_coords.col);
        return next_step;
    }
    else
    {
        return IntPoint(0, 0);
    }
    */
}


IntPoint pathfinding::dumb_path(IntPoint goal, TilePointerMatrix& surroundings, IntPoint cur_coords)
{
    int y = 0 + (goal.row > cur_coords.row) - (goal.row < cur_coords.row);
    int x = 0 + (goal.col > cur_coords.col) - (goal.col < cur_coords.col);
    IntPoint next = IntPoint(y, x);
    IntPoint first_fail = IntPoint(0, 0);
    IntPoint second_fail = IntPoint(0, 0);
    first_fail.row = first_fail.row - (next.row == 0);
    first_fail.col = first_fail.col - (next.col == 0) + (next.col == -1) - (next.col == 1);
    second_fail.row = second_fail.row + (next.row == 0) - (next.row == 1) + (next.row == -1);
    second_fail.col = second_fail.col + (next.col == 0);
    if(surroundings[next.row + cur_coords.row][next.col + cur_coords.col]->can_be_moved_through)
    {
        return next;
    }
    else if(surroundings[first_fail.row + cur_coords.row][first_fail.col + cur_coords.col]->can_be_moved_through)
    {
        return first_fail;
    }
    else if(surroundings[second_fail.row + cur_coords.row][second_fail.col + cur_coords.col]->can_be_moved_through)
    {
        return second_fail;
    }
    else
    {
        return IntPoint(0, 0);
    }
}

std::vector<IntPoint> pathfinding::a_star(IntPoint start, IntPoint goal, TilePointerMatrix& surroundings, int sight)
{
    std::vector<ATile> open;
    std::vector<ATile> closed;

    /* This is used so there is a consistant place for tiles
     * on the open list to point to.  Each tile's parent is
     * an int which corresponds to an index value for any tile
     * which has at some point been the "current tile"
     */
     std::vector<ATile> current_list;

    //The first tile is added to the open list
    open.push_back(ATile(-1, start));
    int cur_index;
    //while the goal hasn't been found
    while(is_in(goal, open) == -1)
    {
        if(open.size()<=0)
        {
            //if the open list is empty, stop
            break;
        }
        //set the current to the smallest f value
        int current_i = get_smallest_f(open);
        //set the "parent" index to the index of the current on the current_lsit
        cur_index = current_list.size();
        current_list.push_back(open[current_i]);

        //move the current from the open list to the closed list
        open.erase(open.begin() + current_i);
        closed.push_back(current_list[cur_index]);

        //loop through the surrounding tiles
        for(int i=current_list[cur_index].coords.row-1;i<=current_list[cur_index].coords.row+1;i++)
        {
            for(int j=current_list[cur_index].coords.col-1;j<=current_list[cur_index].coords.col+1;j++)
            {
                //make sure we're not about to operate on the current tile
                if(i!=current_list[cur_index].coords.row || j!=current_list[cur_index].coords.col)
                {
                    //check if this point is on the open list
                    int open_index = is_in(IntPoint(i, j), open);

                    int y_move = (i-start.row) >= 0 ? (i-start.row) : ((i-start.row) * -1);
                    int x_move = (j-start.col) >= 0 ? (j-start.col) : ((j-start.col) * -1);
                    bool in_range = y_move <= sight && x_move <= sight;
                    bool is_good = i>=0 && j>=0 && i < surroundings.size() && j < surroundings[i].size();
                    //check if this point can be moved through, isn't on the open list, and isn't on the closed list and isn't out of range
                    if(in_range && is_good && surroundings[i][j]->can_be_moved_through && open_index == -1 && is_in(IntPoint(i, j), closed) == -1)
                    {
                        ATile temp = ATile(cur_index, IntPoint(i, j));
                        temp.g = current_list[cur_index].g + (14 * ((i - current_list[cur_index].coords.row != 0) &&
                            (j - current_list[cur_index].coords.col != 0))) + (10 * ((i -
                            current_list[cur_index].coords.row == 0) || (j - current_list[cur_index].coords.col == 0)));

                        temp.h = manhattan(IntPoint(i, j), goal);
                        temp.f = temp.h + temp.g;
                        open.push_back(temp);

                    }
                    //check if it can be moved through, is on the open list, and isn't on the closed list
                    else if(surroundings[i][j]->can_be_moved_through && open_index != -1 && is_in(IntPoint(i, j), closed) == -1)
                    {
                        //recalculate g to give a new f
                        int new_g = current_list[cur_index].g + (14 * ((i - current_list[cur_index].coords.row != 0) &&
                            (j - current_list[cur_index].coords.col != 0))) + (10 * ((i -
                            current_list[cur_index].coords.row == 0) || (j - current_list[cur_index].coords.col == 0)));

                        //if the new g is lower, replace the old parent
                        if(new_g<open[open_index].g)
                        {
                            open[open_index].g = new_g;
                            open[open_index].parent = cur_index;
                            open[open_index].f = open[open_index].g + open[open_index].h;
                        }
                    }
                }
            }
        }
    }

    //get the path by starting at the goal
    //and iterating through parents until
    //we're back at the start.  The one before
    //start is the best move.
    std::vector<IntPoint> path;
    int index = is_in(goal, open);
    if(index != -1)
    {
        ATile current = open[index];
        while(current.parent != -1)
        {
            path.push_back(current.coords);
            current = current_list[current.parent];

        }
    }
    return path;
}


int pathfinding::is_in(IntPoint point, std::vector<ATile> list)
{
    for(int i=0;i<list.size();i++)
    {
        if(list[i].coords.row == point.row && list[i].coords.col == point.col)
        {
            return i;
        }
    }
    return -1;
}


int pathfinding::manhattan(IntPoint current, IntPoint goal)
{
    int dy = (current.row - goal.row) * 10;
    int dx = (current.col - goal.col) * 10;
    dx = (dx > 0) ? dx : -dx;
    dy = (dy > 0) ? dy : -dy;
    return dx + dy;
}

int pathfinding::get_smallest_f(std::vector<ATile>& list)
{
    int smallest = list[0].f;
    int index = 0;
    ATile smallest_a = list[0];
    for(int i=1;i<list.size();i++)
    {
        if(list[i].f<smallest)
        {
            index = i;
            smallest = list[i].f;
        }
    }
    return index;
}

//Dis some real funky maths.  I remember deliberating over it for a
//long time, and I now have no idea how it works.
IntPoint pathfinding::get_opposite(IntPoint abs_coords, IntPoint target_abs)
{
    float rise = abs_coords.row-target_abs.row;
    float run = abs_coords.col- target_abs.col;
    //umm...this should definitely give an exception sometimes
    float slope = rise/run;
    float unsigned_slope = slope * (-1 * (slope < 0));
    int x_change = 0;
    int y_change = 0;

    //i just realized that I have no idea what this line does..
    //I think it was supposed to avoid a divide by 0 error, but clearly
    //does not.
    if(run == 0)
    {
        unsigned_slope = 2;
    }
    if(unsigned_slope < 1)
    {
        x_change = 0 - (run < 0) + (run > 0);
    }
    else if(unsigned_slope > 1)
    {
        y_change = 0 - (rise < 0) + (rise > 0);
    }
    else if(unsigned_slope == 1)
    {
        x_change = 0 - (run < 0) + (run > 0);
        y_change = 0 - (rise < 0) + (rise > 0);
    }
    return IntPoint(y_change, x_change);
}



void pathfinding::dump_matrix(TilePointerMatrix& map, IntPoint tl, IntPoint br)
{
    int tile;
    for(int i=tl.row; i<br.row;i++)
    {
        for(int j=tl.col; j<br.col;j++)
        {
            tile = map[i][j]->tile_id;
            switch(tile){
                case 1:
                    cout<<".";
                    break;
                case 2:
                    cout<<".";
                    break;
                case 4:
                    cout<<"#";
                    break;
                case 5:
                    cout<<"P";
                    break;
                case 6:
                    cout<<"M";
                    break;
                case 10:
                    cout<<"T";
                    break;
                case 11:
                    cout<<"#";
                    break;
                case 12:
                    cout<<"d";
                    break;
                case 13:
                    cout<<"u";
                    break;
                case 15:
                    cout<<"S";
                    break;
                case 16:
                    cout<<"~";
                    break;
                case 20:
                    cout<<"t";
                    break;
                case 21:
                    cout<<".";
                    break;
                default:
                    cout<<"0";
                    break;
            }
        }
        cout<<endl;
    }
    cout<<"-----------------------------------------------------"<<endl;
}
