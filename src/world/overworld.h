#ifndef _OVERWORLD_H
#define _OVERWORLD_H

#include <terrain_defs.h>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <spawner.h>

class Overworld
{
    typedef std::vector<std::vector<Tile> > TileMatrix;

    private:
        TileMatrix ground;
        Spawner spawner;
    public:
        bool has_layer_below;
        std::vector<int> down_stair;
        int width;
        int height;
        Tile* get_tile(int, int) ;
        void set_tile(int, int, Tile*);
        Overworld(int, int, bool);
        Overworld();
        std::vector<std::vector<Tile> >& get_ground();
        Spawner* get_spawner();

};

#endif
