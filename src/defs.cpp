/**
 *  @file DEFS.CPP
 *  @author Seth Yoder, Michael Yoder
 *
 *  @section LICENSE
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

#include "defs.h"

std::string STAT_NAMES[NUM_STATS] = {"Health", "Attack", "Endurance", "Experience", "Level", "Str", "Dex", "Int"};


int get_part_hit(int chance)
{
    int accum = 0;
    for(int i=0;i<NUM_PARTS;i++)
    {
        accum += HIT_CHANCE[i];
        if(chance <= accum)
        {
            return i;
        }
    }
    return 0;
}


/****************************
 *   TERRAIN DEFS
 ***************************/
//{int char_count, int tile_id, int color, bool can_be_moved_through, bool visible, bool opaque, bool seen, bool can_build_overtop}
namespace tiledef {
    Tile  EMPTY           =  {0,    0,   0,             1,  0,  0,  0,  0};
    Tile  OVERWORLD_DIRT  =  {250,  1,   BROWN,         1,  0,  0,  0,  1};
    Tile  DIRT            =  {250,  2,   BROWN,         1,  0,  0,  0,  1};
    Tile  DUNGEON_BORDER  =  {250,  3,   DARK_RED,      0,  1,  0,  0,  0};
    Tile  ROOM_WALL       =  {219,  4,   GRAY,          0,  0,  1,  0,  0};
    Tile  PATH            =  {250,  5,   BROWN,         1,  0,  0,  0,  1};
    Tile  MAIN_CHAR       =  {1,    6,   DARK_RED,      1,  1,  0,  0,  0};
    Tile  MAIN_CHAR2      =  {1,    7,   DARKER_GREEN,  1,  1,  0,  0,  0};
    Tile  MAIN_CHAR3      =  {1,    8,   BROWN,         1,  1,  0,  0,  0};
    Tile  MAIN_CHAR4      =  {1,    9,   GRAY,          1,  1,  0,  0,  0};
    Tile  TREE            =  {84,   10,  DARK_GREEN,    0,  0,  1,  0,  0};
    Tile  BLOCK_WALL      =  {219,  11,  GRAY,          0,  0,  1,  0,  0};
    Tile  DOWN_STAIR      =  {31,   12,  GRAY,          1,  0,  0,  0,  0};
    Tile  UP_STAIR        =  {30,   13,  GRAY,          1,  0,  0,  0,  0};
    Tile  KOBOLD          =  {107,  14,  DARK_GREEN,    0,  1,  0,  0,  0};
    Tile  KOBOLD_SPAWNER  =  {21,   15,  GREEN,         0,  0,  1,  0,  0};
    Tile  WATER           =  {247,  16,  BLUE,          0,  0,  0,  0,  0};
    Tile  LIGHT_WATER     =  {126,  17,  BLUE,          0,  0,  0,  0,  0};
    Tile  SAND1           =  {176,  18,  TAN,           1,  0,  0,  0,  1};
    Tile  SAND2           =  {178,  19,  TAN,           1,  0,  0,  0,  1};
    Tile  BIG_TREE        =  {116,  20,  DARK_GREEN,    0,  0,  1,  0,  0};
    Tile  GRASS_DIRT      =  {250,  21,  DARKER_GREEN,  1,  0,  0,  0,  1};
    Tile  KOBOLD_CORPSE   =  {107,  22,  GRAY,          1,  0,  0,  0,  0};
    Tile  BOOTS           =  {28,   23,  BROWN,         1,  0,  0,  0,  0};
    Tile  RABBIT          =  {114,  24,  BROWN,         0,  1,  0,  0,  0};
    Tile  RABBIT_CORPSE   =  {114,  25,  GRAY,          1,  0,  0,  0,  0};
    Tile  SWORD           =  {47,   26,  GRAY,          1,  0,  0,  0,  0};
    Tile  POTATO          =  {7,    27,  BROWN,         1,  0,  0,  0,  0};
    Tile  PLANT           =  {6,    28,  GREEN,         1,  0,  0,  0,  0};
    Tile  WOOD_WALL       =  {176,  29,  BROWN,         0,  0,  1,  0,  0};
    Tile  WOOD_FLOOR      =  {47,   30,  BROWN,         1,  0,  0,  0,  0};
    Tile  DOOR            =  {43,   31,  BROWN,         1,  0,  1,  0,  0};
    Tile  BURROW          =  {15,   32,  BROWN,         1,  0,  0,  0,  0};
    Tile  HUT_WALL        =  {35,   33,  BROWN,         0,  0,  1,  0,  0};
    Tile  AXE             =  {213,  34,  GRAY,          1,  0,  0,  0,  0};
    Tile  LOG             =  {220,  35,  BROWN,         1,  0,  0,  0,  0};
    Tile  WOLF            =  {119,  36,  BROWN,         1,  0,  0,  0,  0};
    Tile  HUMAN           =  {104,  37,  TAN,           1,  0,  0,  0,  0};
    Tile  COBBLE          =  {42,   38,  GRAY,          1,  0,  0,  0,  0};

    Tile TILE_INDEX[TILE_TYPE_COUNT] = { //THIS MUST CORRESPOND TO TILE IDS
        EMPTY,          //ID 0
        OVERWORLD_DIRT, //ID 1
        DIRT,           //ID 2
        DUNGEON_BORDER, //etc
        ROOM_WALL,
        PATH,
        MAIN_CHAR,
        MAIN_CHAR2,
        MAIN_CHAR3,
        MAIN_CHAR4,
        TREE,
        BLOCK_WALL,
        DOWN_STAIR,
        UP_STAIR,
        KOBOLD,
        KOBOLD_SPAWNER,
        WATER,
        LIGHT_WATER,
        SAND1,
        SAND2,
        BIG_TREE,
        GRASS_DIRT,
        KOBOLD_CORPSE,
        BOOTS,
        RABBIT,
        RABBIT_CORPSE,
        SWORD,
        POTATO,
        PLANT,
        WOOD_WALL,
        WOOD_FLOOR,
        DOOR,
        BURROW,
        HUT_WALL,
        AXE,
        LOG,
        WOLF,
        HUMAN,
        COBBLE
    };
}


namespace map_tile {
    MapTile MAP_FOREST = {116, DARK_GREEN, 0, true, NORMAL, tiledef::DIRT, true, true, 3, 1};
    MapTile CITY = {9, BROWN, 2, true, NORMAL, tiledef::DIRT, true, true, 3, 1};
    MapTile MAP_WATER = {247, DARK_BLUE, 3, false, HARD, tiledef::WATER, true, true, 2, 3};
    MapTile MAP_BEACH = {178, TAN, 4, false, NORMAL, tiledef::SAND1, false, false, 1};
    MapTile MAP_DEFAULT = {240, DARK_GREEN, 5, true, NORMAL, tiledef::DIRT, false, false, 1};
    MapTile CURSOR = {88, WHITE, 6, false, NORMAL, tiledef::EMPTY, false, false, 1};
    MapTile CITY_MARKET = {9, RED, 7, true, NORMAL, tiledef::DIRT, true, true, 3, 1};
    MapTile CITY_RESIDENTIAL = {9, GREEN, 8, true, NORMAL, tiledef::DIRT, true, true, 3, 1};
    MapTile CITY_NOBILITY = {9, YELLOW, 9, true, NORMAL, tiledef::DIRT, true, true, 3, 1};

    MapTile CITIES[NUM_CITY_TILES] = {
        CITY_NOBILITY,
        CITY_MARKET,
        CITY_RESIDENTIAL
    };

    MapTile WORLD_CONSTRUCTION_INDEX[NUM_WORLD_TILES] = {
        MAP_FOREST,  //ID 0
        CITY,
        MAP_WATER,
    };

    MapTile MAP_TILE_INDEX[NUM_MAP_TILE] = { //THIS MUST CORRESPOND TO MAP TILE IDS
        MAP_FOREST,  //ID 0
        CITY,
        MAP_WATER,
        MAP_BEACH,   //ETC
        CITY_MARKET,
        CITY_RESIDENTIAL,
        CITY_NOBILITY,
        MAP_DEFAULT,
        CURSOR
    };
}


/****************************
 *   ITEM DEFS
 ***************************/
/*****
     0
   54145
     1
    2 2
    3 3
 Body index diagram.
******/


namespace equipment
{
    EquipType boots = {2.0, tiledef::BOOTS, "Boots", "A simple pair of boots.", 0, 3, 0, {0, 0, 0}, {.1, .1, 0}, false, 2, ARMOR, LEATHER};
}

namespace weapons
{
    WeaponType wood_axe = {4.0, tiledef::AXE, "Wood Axe", "A large axe for cutting trees.", 0, 2, 1, 1, false, 1, AXE}; 
    WeaponType dagger = {2.0, tiledef::SWORD,  "Small Dagger", "An unfortunately pathetic dagger.", 0, 3, 2, 1, false, 1, SHORT_BLADE};
    WeaponType fist = {0.0, tiledef::AXE, "Fists", "Used for bludgeoning.", 0, 1, 3, 1, false, 0, HAND};
    WeaponType claws = {0.0, tiledef::AXE, "Claws", "Used for slashing.", 0, 1, 3, 1, false, 0, HAND};

}

namespace consumables
{
    ConsumableType potato = {0.3, tiledef::PLANT, "Potato", "An uncooked potato. It might heal some health.  Or not, I don't know.", false, 0, HEALTH, rand() % 2, RESTORE, 1, INGREDIENT};
    ConsumableType CONSUMABLE_LIST[NUM_CONSUMABLES] = {potato};  
}

namespace misc
{
    MiscType kobold_corpse = {20, tiledef::KOBOLD_CORPSE, "Kobold Corpse", "A decaying corpse of a kobold. Yum!", false, 0, 10, USELESS};
    MiscType rabbit_corpse = {3, tiledef::RABBIT_CORPSE, "Rabbit Corpse", "The corpse of a small woodland creature that met an untimely demise.", false, 0, 2, USELESS};
    MiscType player_corpse = {120, tiledef::MAIN_CHAR, "Your Corpse", "You seem to have died.  Interesting...", false, 0, 30, USELESS};
    MiscType wolf_corpse = {20, tiledef::WOLF, "Wolf Corpse", "The corpse of a brown wolf.", false, 0, 10, USELESS};
    MiscType human_corpse = {120, tiledef::HUMAN, "Human Corpse", "The corpse of a human.", false, 0, 10, USELESS};
    MiscType wood = {5.0, tiledef::LOG, "Logs", "Some logs from a felled tree.", false, 0, 5, USELESS};

}

/****************************
 *   ENEMY DEFS
 ***************************/

namespace spawners
{
    SpawnType kobold = {true, 3, 10, 3, HUTS};
    SpawnType rabbit = {false, 2, 8, 20, BURROW};
}

//Enemy Equipment
EquipType kob_eq[] = {equipment::boots};
std::vector<EquipType> kob_eq_vec(&kob_eq[0], &kob_eq[0] + 1);
WeaponType kob_wep[] = {weapons::dagger};
std::vector<WeaponType> kob_wep_vec(&kob_wep[0], &kob_wep[0] + 1);


//Enemy Stats
//There has to be a better way to do this...
int kob_stats_arr[] = {10, 2, 10, 0, 1, 5, 5, 5};
std::vector<int> kob_stats(&kob_stats_arr[0], &kob_stats_arr[0] + NUM_STATS);

int rab_stats_arr[] = {2, 0, 10, 0, 1, 1, 1, 1};
std::vector<int> rab_stats(&rab_stats_arr[0], &rab_stats_arr[0] + NUM_STATS);

int wolf_stats_arr[] = {10, 1, 10, 0, 1, 3, 3, 3};
std::vector<int> wolf_stats(&wolf_stats_arr[0], &wolf_stats_arr[0] + NUM_STATS);

int hum_stats_arr[] = {10, 2, 10, 0, 1, 5, 5, 5};
std::vector<int> hum_stats(&hum_stats_arr[0], &hum_stats_arr[0] + NUM_STATS);

namespace enemies
{
    EnemyType kobold = {5, kob_stats, 0, 0, 20, 5, 100, "Kobold", tiledef::KOBOLD, misc::kobold_corpse, kob_eq_vec, kob_wep_vec, spawners::kobold, weapons::claws};
    EnemyType rabbit = {3, rab_stats, 1, 1, 15, 25, (rand() % (20 + 50)), "Rabbit",  tiledef::RABBIT, misc::rabbit_corpse, std::vector<EquipType>(), std::vector<WeaponType>(), spawners::rabbit, weapons::claws};
    EnemyType wolf_companion = {1, wolf_stats, 2, 2, 15, 20, 100, "Wolf", tiledef::WOLF, misc::wolf_corpse, std::vector<EquipType>(), std::vector<WeaponType>(), spawners::rabbit, weapons::claws}; 
    EnemyType human = {1, hum_stats, 3, 3, 15, 20, 10, "Human", tiledef::HUMAN, misc::human_corpse, std::vector<EquipType>(), std::vector<WeaponType>(), spawners::rabbit, weapons::fist};  
    EnemyType ENEMY_LIST[4] = {kobold, rabbit};
    int NUM_ENEMIES = 4; 
}


MiscType tree_drops[] = {misc::wood};
std::vector<MiscType> tree_drop_vec(&tree_drops[0], &tree_drops[0] + 1);

namespace plants
{
    PlantType tree = {0, "Tree", construct_big_trees(), AXE, tree_drop_vec};
    PlantType PLANT_LIST[1] = {tree};
}

/**************************CONSTRUCTION FUNCTIONS********************/
std::vector<std::vector<Tile> > construct_big_trees()
{
    std::vector<std::vector<Tile> > tree(2, std::vector<Tile>(2));
    for(int i=0;i<2;i++) {

        for(int j=0;j<2;j++) {
            tree[i][j] = tiledef::BIG_TREE;
        }
    }
    return tree;
}
