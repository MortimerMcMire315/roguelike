#ifndef _CANVAS_H
#define _CANVAS_H

#define STARTING_WIDTH 100
#define STARTING_HEIGHT 100

#include <SDL/SDL.h>
#include <stdlib.h>
#include <vector>

#include "DungeonBuilder.h"
#include "Dungeon.h"
#include "terrain_defs.h"
#include "Character.h"

using namespace std;
using namespace tiledef;
class Canvas 
{
    typedef std::vector<std::vector<Tile> > TileMatrix;
    private:
        Dungeon dungeon;
        std::vector<std::vector<Tile> > canvas;
    
    public:
        Canvas();
        //TODO do something about these two. Events.cpp?
        //The way the project is now structured, yeah.  Typical games consist of 5 sections:
		//init, input, process, display, cleanup.  As it is, we've started dividing those into files, but we don't have it the whole way yet. Most of those are sitting in Display, and could be given their own files. 
		DungeonBuilder db;
        Main_Character main_char;
        void refresh();
        void resize_canvas(int, int);
        const std::vector<std::vector<Tile> >& get_matrix();
        const Dungeon& get_dungeon();
};

#endif
