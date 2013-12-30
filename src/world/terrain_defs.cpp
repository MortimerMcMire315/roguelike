#include <terrain_defs.h>

namespace tiledef {
    Tile OVERWORLD_DIRT = {250, 1, BROWN, true, false, false, false};
    Tile DIRT = {250, 1, BROWN, true, false, false, false};
    Tile DUNGEON_BORDER = {250, 1, DARK_RED, false, true, false, false};
    Tile ROOM_WALL = {219, 2, GRAY, false, false, true, false};
    Tile EMPTY = {0, 3, 0, true, false, false, false};
    Tile PATH = {250, 4, BROWN, true, false, false, false};
    //Tile PATH = DIRT;
    Tile MAIN_CHAR = {1, 5, DARK_RED, true, true, false, false}; 
    Tile MAIN_CHAR2 = {1, 5, DARK_GREEN, true, true, false, false};
    Tile MAIN_CHAR3 = {1, 5, BROWN, true, true, false, false};
    Tile MAIN_CHAR4 = {1, 5, GRAY, true, true, false, false};
    Tile TREE = {84, 6, DARK_GREEN, false, false, true, false};
    //Tile BLOCK_WALL = {177, 7, GRAY, false, false, true, false};
    Tile BLOCK_WALL = {219, 7, GRAY, false, false, true, false};
    Tile DOWN_STAIR = {31, 8, GRAY, true, false, false, false};
    Tile UP_STAIR = {30, 9, GRAY, true, false, false, false};
    Tile KOBOLD = {107, 10, RED, false, true, false, false};
    Tile KOBOLD_SPAWNER = {21, 11, GREEN, false, false, true, false};
    Tile WATER = {247, 12, BLUE, true, false, false, false};
    Tile LIGHT_WATER = {126, 13, BLUE, true, false, false, false};
    Tile SAND1 = {176, 14, TAN, true, false, false, false};
    Tile SAND2 = {178, 15, TAN, true, false, false, false};
}

namespace map_tile {
    MapTile MAP_DEFAULT = {146, GREEN, 1};
    MapTile MAP_WATER = {247, BLUE, 2};
    MapTile MAP_BEACH = {146, TAN, 3};
    MapTile CURSOR = {88, WHITE, 4};
}

