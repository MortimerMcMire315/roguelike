/**
 *  @file SPAWNER.CPP
 *  @author Michael Yoder
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

#include "spawner.h"
#include "tileset.h"
#include "tile.h"

#include <unordered_map>

using namespace std;
Spawner::Spawner()
{

}

Spawner::Spawner(int _x, int _y, int _depth,  EnemyType _enemy)
{
    x = _x;
    y = _y;
    depth = _depth;
    enemy = _enemy;
    spawn_type = enemy.spawner;
    num_enemy = rand() % (spawn_type.max_enemies - spawn_type.min_enemies) + spawn_type.min_enemies;
    spawn_points = std::vector<Den>();
    construct_den();
    if(spawn_type.spawn_immediately)
    {
        spawn_creep(num_enemy);
    }
}


void Spawner::construct_den()
{
    switch(spawn_type.den_type)
    {
        case spawners::BURROW:
            construct_burrow();
            break;
        case spawners::HUTS:
            construct_huts();
            break;
        default:
            break;
    }
}


void Spawner::construct_burrow()
{
    spawn_points.push_back(Den(0, 0, Tileset::get("BURROW")));
}

void Spawner::construct_huts()
{
    //let's have statically size huts for now.  just shove
    //enemies in there.

    std::unordered_map<std::string, Tile>* tileset = &Tileset::instance()->get_tileset();

    int num_huts = num_enemy/spawn_type.enemies_per_spawn;
    TileMatrix hut = TileMatrix(5, std::vector<Tile>(5, (*tileset)["EMPTY"]));
    IntPoint start = IntPoint(2, 2);
    std::vector<IntPoint> points = bresenham_circle(start, 2);
    for(int i=0;i<points.size();i++)
    {
        hut[points[i].row][points[i].col] = (*tileset)["HUT_WALL"];
    }

    int hut_x;
    int hut_y;
    for(int i=0;i<num_huts;i++)
    {
        int door = rand() % points.size();
        hut[points[door].row][points[door].col] = (*tileset)["EMPTY"];
        do
        {
            hut_x = rand() % (num_huts * 6) - (num_huts*6)/2;
            hut_y = rand() % (num_huts * 6) - (num_huts*6)/2;
        }
        while(overlapping_spawners(hut_x, hut_y, 3));
        spawn_points.push_back(Den(hut_x, hut_y, 5, 5, hut));
    }
}

bool Spawner::overlapping_spawners(int x, int y, int radius)
{
    for(int i=0;i<spawn_points.size();i++)
    {
        int added_radius = radius * 2;
        int x_dif = spawn_points[i].get_x() - x;
        int y_dif = spawn_points[i].get_y() - y;
        int rad_squared = added_radius * added_radius;
        int pyth = (x_dif * x_dif) + (y_dif * y_dif);

        bool overlap = 0 <= pyth && pyth <= rad_squared;
        if(overlap)
        {
            return overlap;
        }
    }
    return false;
}


void Spawner::run()
{
    if(should_spawn())
    {
        spawn_creep(1);
    }
}

bool Spawner::should_spawn()
{
    return (num_enemy > 0) && (rand() % 100 == 0);
}

void Spawner::spawn_creep(int num_creeps)
{
     for(int i=0;i<num_creeps;i++)
     {
        int num = rand() % spawn_points.size();
        int spawn_x = spawn_points[num].get_x() + x;
        int spawn_y = spawn_points[num].get_y() + y;
        Enemy* enem = new Enemy(spawn_x, spawn_y, depth, enemy);
        enemy_queue.push_back(enem);
        num_enemy -= 1;
     }
}

std::vector<Character*>& Spawner::flush()
{
    return enemy_queue;
}

//THIS IS NOT MEMORY SAFE DO NOT DO THIS UNLESS
//THESE POINTERS HAVE BEEN MOVED ELSEWHERE BY CALLING
//FLUSH
void Spawner::clear_queue()
{
    enemy_queue = std::vector<Character*>();
}

void Spawner::deep_clear()
{
    for(int i=0;i<enemy_queue.size();i++)
    {
        delete enemy_queue[i];
    }
    clear_queue();
}

int Spawner::get_x()
{
    return x;
}

int Spawner::get_y()
{
    return y;
}

int Spawner::get_depth()
{
    return depth;
}

int Spawner::get_enemy_type_id()
{
    return enemy.id;
}

std::vector<Den>& Spawner::get_spawn_points()
{
    return spawn_points;
}

Den* Spawner::get_spawn_at(int i)
{
    return &spawn_points[i];
}

bool Spawner::point_in_spawner(int _x, int _y)
{
    for(int i=0;i<spawn_points.size();i++)
    {
        Den* den = &spawn_points[i];
        int den_x = x + den->get_x();
        int den_y = y + den->get_y();
        if((den_x <= _x && _x <= den_x + den->get_width()) &&
           (den_y <= _y && _y <= den_y + den->get_height()))
        {
            return true;
        }
    }
    return false;
}
