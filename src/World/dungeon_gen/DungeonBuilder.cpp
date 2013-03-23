#include <DungeonBuilder.h>

/* PRE:  Will be given the desired width and height of the
 * dungeon floor.
 * POST: Will populate the :char dungeon[][]: array with dummy
 * values.
 */
DungeonBuilder::DungeonBuilder(int _width, int _height, int seed)
{
	width = _width;
	height = _height;

	if(width > MAX_WIDTH)
	{
		width = MAX_WIDTH;
	}
	if(height > MAX_HEIGHT)
	{
		height = MAX_HEIGHT;
	}

	cout<<"width:"<<_width<<"  height:"<<_height<<endl;

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			dungeon[i][j] = ' ';
		}
	}
    srand(seed);
}

/*
 * POST: Will print the dungeon floor to stdout.
 */
ostream& operator<<(ostream &out, const DungeonBuilder &D)
{
    D.print();
    return out;
}

void DungeonBuilder::print() const
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			cout<<dungeon[i][j]<<" ";
		}
		cout<<endl;
	}
}

/* PRE: Will be given :int given:, a number under 100.
 *
 * POST: Will perform a random number check between 1 and 100.
 * Will return true if result <= given; will return false if 
 * result > given.
 */
bool DungeonBuilder::rolled_over(int given) const
{
	int generated = rand() % 100;
	if(generated <= given)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* PRE: Will be given :IntPoint point:
 *
 * POST: Will determine whether or not the point is an empty
 * space.
 */
bool DungeonBuilder::is_empty_space(IntPoint point) const
{
    return ((dungeon[point.row][point.col] == ' ') or (dungeon[point.row][point.col] == 'X'));
}

/* PRE: Will be given :IntPoint point:
 *
 * POST: Will determine whether or not the given point is beyond the 
 * bounds of the dungeon.
 */
bool DungeonBuilder::point_is_beyond_bounds(IntPoint point) const
{
    if((point.row < 0) or (point.row >= height))
    {
        return true;
    }

    else if((point.col < 0) or (point.col >= width))
    {
        return true;
    }

    return false;
}

/* PRE: Will be given :IntPoint point: that lies on the wall of a room.
 * POST: Will determine which wall of the room the point lies on based
 *       on the surrounding tiles.
 */
int DungeonBuilder::determine_which_wall(IntPoint point) const
{
    int direction;

    if((dungeon[point.row][point.col - 1] == '=') or 
       (dungeon[point.row][point.col + 1] == '='))
    {
        if (dungeon[point.row - 1][point.col] == '.')
        {
            direction = 2;
        }
        
        else if (dungeon[point.row + 1][point.col] == '.')
        {
            direction = 0;
        }
    }

    else if ((dungeon[point.row - 1][point.col] == '|') or
        (dungeon[point.row + 1][point.col] == '|'))
    {
        if (dungeon[point.row][point.col - 1] == '.')
        {
            direction = 1;
        }
        
        else if (dungeon[point.row][point.col + 1] == '.')
        {
            direction = 3;
        }
    }
    
    cout<<direction<<endl;
    return direction;
}


/* PRE:
 * POST: Will find a good starting point for a procedurally-blind dungeon
 */
IntPoint DungeonBuilder::find_viable_starting_point(int std_width, int std_height) const
{
    int good_row = rand() % (int)(height/2) + (int)(height/4) - (int)(std_height / 2);
    int good_col = rand() % (int)(width/2) + (int)(width/4) - (int)(std_width / 2);
    return IntPoint(good_row, good_col);
}

/* PRE: Will be given :IntPoint tl:, which represents the top-left corner,
 * :IntPoint br:, which represents the bottom-right corner, and
 * :int squareness:, which denotes how square the rooms will be.
 * 
 * POST: Will draw a room on the dungeon array with the given parameters.
 */
Room DungeonBuilder::build_room(IntPoint tl, IntPoint br, int squareness)
{
    //draw four corners
    dungeon[tl.row][tl.col] = '+';
    dungeon[tl.row][br.col] = '+';
    dungeon[br.row][tl.col] = '+';
    dungeon[br.row][br.col] = '+';
    //draw top and bottom rows
    for(int i = tl.col + 1; i <= br.col - 1; i++)
    {
        dungeon[tl.row][i] = '=';
        dungeon[br.row][i] = '=';
    }
    //draw left and right walls
    for(int i = tl.row + 1; i <= br.row - 1; i++)
    {
        dungeon[i][tl.col] = '|';
        dungeon[i][br.col] = '|';
    }
    
    for(int i = tl.row + 1; i <= br.row - 1; i++)
    {
        for(int j = tl.col + 1; j <= br.col - 1; j++)
        {
            dungeon[i][j] = '.';
        }
    }

    return Room(tl, br);
}

/* PRE:
 * POST: Will build a good starting room in the dungeon space
 */
void DungeonBuilder::build_start_room(int std_room_width, int std_room_height,
                                int room_width_deviation, int room_height_deviation)
{
    int room_width = rand() % room_width_deviation + 
                             (std_room_width - (int)(room_width_deviation/2));
    int room_height = rand() % room_height_deviation +
                             (std_room_height - (int)(room_height_deviation/2));
    IntPoint starting_point = find_viable_starting_point(std_room_width, 
                                                        std_room_height);
    IntPoint br = IntPoint(starting_point.row + (room_height + 1), 
                           starting_point.col + (room_width + 1));
    rooms[num_rooms] = build_room(starting_point, br, 2);
    num_rooms += 1;
}

/* PRE: Will be given a Room object.
 * POST: Will return the original number of wall spaces in the room 
 * (corners are not included).
 */
int DungeonBuilder::get_wall_count(const Room &R) const
{
    return (R.br.row - R.tl.row - 1) * 2 + (R.br.col - R.tl.col - 1) * 2;
}

/*
 * PRE: Will be given a Room object.
 * POST: Will return a random wall block that lies on the circumference of the given room.
 */
IntPoint DungeonBuilder::rand_wall_block(const Room &current_room)
{ 
    int height = (current_room.br.row - current_room.tl.row) - 1;
    int width = (current_room.br.col - current_room.tl.col) - 1;
    int path_from_side = rand() % (height + width) + 1;
    IntPoint point;
    if (path_from_side < height)
    {
        int a = rand() % 2;
        if (a == 0)
        {
            point.row = current_room.tl.row;
        }
        else
        {
            point.row = current_room.br.row;
        }
        point.col = rand() % width + current_room.tl.col + 1; 
    }
    else
    {
        int a = rand() % 2;
        if (a == 0)
        {
            point.col = current_room.tl.col;
        }
        else
        {
            point.col = current_room.br.col;
        }
        point.row = rand() % height + current_room.tl.row + 1;
    }

    return point;
}

/* PRE: Will be given :IntPoint this_point:, which is the current point, and 
 *      :int direction:, which refers to the proposed direction.
 * POST: Will return the next point given the proposed direction.
 */
IntPoint DungeonBuilder::get_next_point(IntPoint this_point, int direction) const
{
    IntPoint next_point = this_point;
    switch (direction)
    {
        case 0:
            next_point.row -= 1;
            break;
        case 1:
            next_point.col += 1;
            break;
        case 2:
            next_point.row += 1;
            break;
        case 3:
            next_point.col -= 1;
            break;
    }

    return next_point;
}   

/* PRE: Will be given :IntPoint start:, which refers to the wall block of the room at
 * which the path starts, and :int direction:, which refers to the starting direction
 * of the path according to this scheme:
 * 0 = up
 * 1 = right
 * 2 = down
 * 3 = left
 *
 * POST: Will attempt to build a path from the given Room and block. If this succeds,
 * will return the IntPoint of the end of the path. If it fails, it will return an 
 * IntPoint with row=-1 and col=-1.
 */
IntPoint DungeonBuilder::build_path(IntPoint start, int direction)
{
    int path_length = rand() % (MAX_PATH_LENGTH - MIN_PATH_LENGTH) + MIN_PATH_LENGTH;
    IntPoint current_point = start;
    int current_direction = direction;
    bool bad_direction;
    int tries;
    IntPoint potential_point;
    for(int i = 0; i < path_length; i++)
    {
        dungeon[current_point.row][current_point.col] = 'X';
        //For at least 2 or 3 blocks, just go straight. otherwise,
        //we may change direction.
        if (i > 3)
        {
            //Do we change direction?
            int dirchange = rand() % 100;
            if (dirchange < 15)
            {
                //Which direction do we change to?
                switch (dirchange % 2)
                {
                    case 0:
                        current_direction -= 1;
                        current_direction = (current_direction % 4);
                        break;
                    case 1:
                        current_direction += 1;
                        current_direction = (current_direction % 4);
                        break;
                }
            }
        }

        do
        {
            bad_direction = false;
            potential_point = get_next_point(current_point, current_direction);
            if ((point_is_beyond_bounds(potential_point)) or (!is_empty_space(potential_point)))
            {
                bad_direction = true;
                current_direction += 1;
            }
        } while(bad_direction);
        
        current_point = potential_point;
        
    }
    return IntPoint(-1, -1);
}

/* PRE: Will be given :int target: to specify a general target
 * number of openings in the dungeon floor, :int deviation: to
 * specify the maximum desired deviation from this target, and
 * :int squareness:, a number from 0 to 100, which determines 
 * how square the rooms are.
 *
 * POST: Will build a procedurally-blind dungeon in the dungeon
 * floor (in which a room is built near the center, and rooms and
 * hallways crawl off of that room.
 */
int DungeonBuilder::build_pblind_dungeon(int target, 
                                         int deviation, int squareness)
{   
    int std_room_width = 10;
    int std_room_height = 10;
    int room_width_deviation = 12;
    int room_height_deviation = 8;
    build_start_room(std_room_width, std_room_height, room_width_deviation, 
                     room_height_deviation);    
    //int target_rooms = rand() % deviation + 
    //                        (_target - (int)(deviation / 2));
    int current_room_num= 0;
    recursive_pblind_dungeon(target, deviation, squareness, std_room_width,
                             room_width_deviation, room_height_deviation,
                             current_room_num);
    cout<<*this;
	return 0;
}

/* PRE: Will be given :int target:, :int deviation:, :int squareness:,
 * :int std_room_width:, :int room_width_deviation:, 
 * :int room_height_deviation: from the parent function :int build_pblind_dungeon():,
 * and :int current_room_num:, which is an index of the "rooms" array.
 *
 * POST: Will build the dungeon by finding 0 to 2 viable wall blocks in the given
 * room, building paths outward from those wall blocks, and (often) building rooms
 * at the end of those paths. Every time a new room is built, the function is called
 * again with that room's index passed as :int current_room_num:. 
 *
 */
void DungeonBuilder::recursive_pblind_dungeon(int target, int deviation,
                                             int squareness,int std_room_width,
                                             int room_width_deviation,
                                             int room_height_deviation,
                                             int current_room_num)
{
    //declaring Room as pointer to point to different array indices.
    Room * current_room = &rooms[current_room_num];
    IntPoint point = rand_wall_block(*current_room);

    build_path(point, determine_which_wall(point));
    dungeon[point.row][point.col] = 'X';
    int num_paths = rand() % 3;

}