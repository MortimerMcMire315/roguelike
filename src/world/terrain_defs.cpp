#include "terrain_defs.h"

namespace tiledef {
    Tile OVERWORLD_DIRT = {250, 1, BROWN, true, true, false};
    Tile DIRT = {250, 1, BROWN, true, false, false};
    Tile ROOM_WALL = {35, 2, GRAY, false, false, true};
    Tile EMPTY = {0, 3, 0, true, false, false};
    //I changed the path to . because I think it adds continuity.  Feel free to change it back.
    Tile PATH = {250, 4, BROWN, true, false, false};
    Tile MAIN_CHAR = {1, 5, DARK_RED, true, true, false}; 
    Tile MAIN_CHAR2 = {1, 5, DARK_GREEN, true, true, false};
    Tile MAIN_CHAR3 = {1, 5, BROWN, true, true, false};
    Tile MAIN_CHAR4 = {1, 5, GRAY, true, true, false};
    Tile TREE = {84, 6, DARK_GREEN, false, true, true};
    Tile BLOCK_WALL = {35, 7, GRAY, false, false, true};
    Tile DOWN_STAIR = {31, 8, GRAY, true, true, false};
    Tile UP_STAIR = {30, 9, GRAY, true, true, false};
}
