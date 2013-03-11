#ifndef DUNGEONBUILDER_H
#define DUNGEONBUILDER_H

#define MAX_WIDTH 500 
#define MAX_HEIGHT 500
#define MAX_ROOMS 15
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
        void build_start_room(int, int, int, int);
		bool rolled_over(int) const;
		bool is_empty_space(IntPoint) const;
        bool point_is_beyond_bounds(IntPoint) const;
		void print() const;
        Room build_room(IntPoint, IntPoint, int);
        IntPoint find_viable_starting_point(int, int) const;

	public:
		int build_pblind_dungeon(int, int, int);
		DungeonBuilder(int, int, int seed=time(NULL));
};

#endif
