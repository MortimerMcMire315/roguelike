#include <DungeonBuilder.h>
#include <Character.h>
#include <ncurses.h>
#include <stdio.h>


int main()
{
	DungeonBuilder dungeon(70, 50);
    dungeon.build_pblind_dungeon(1, 1, 1);
   
	return 0;	
}