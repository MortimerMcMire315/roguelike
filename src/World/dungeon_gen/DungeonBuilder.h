#ifndef DUNGEONBUILDER_H
#define DUNGEONBUILDER_H

#define MAX_WIDTH 500 
#define MAX_HEIGHT 500
#define MAX_ROOMS 15
#define MAX_PATH_LENGTH 100
#define MIN_PATH_LENGTH 16 
#include <iostream>
#include <string>
#include <stdlib.h>
#include <int_point.h>
#include <Room.h>
using namespace std;

class DungeonBuilder
{
    friend ostream& operator<<(ostream&, const DungeonBuilder&);
	private:
        //variables
		int width;
		int height;
        int num_rooms;
		char dungeon[MAX_WIDTH][MAX_HEIGHT];
        Room rooms[MAX_ROOMS];

        //methods
		void print() const;
		bool rolled_over(int) const;
		bool is_empty_space(IntPoint) const;
        bool point_is_beyond_bounds(IntPoint) const;
        int determine_which_wall(IntPoint) const;
        IntPoint find_viable_starting_point(int, int) const;
        Room build_room(IntPoint, IntPoint, int);
        void build_start_room(int, int, int, int);
        int get_wall_count(const Room&) const;
        IntPoint rand_wall_block(const Room&);
        IntPoint get_next_point(IntPoint, int) const;
        IntPoint build_path(IntPoint, int);
        void recursive_pblind_dungeon(int, int, int, int, int, int, int);

	public:
		DungeonBuilder(int, int, int seed=time(NULL));
		int build_pblind_dungeon(int, int, int);
};

#endif