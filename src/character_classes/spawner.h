/**
 *  @file SPAWNER.H
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

#ifndef _SPAWNER_H
#define _SPAWNER_H

#include <ctime>
#include <iostream>

#include "enemy.h"
#include "defs.h"
#include "den.h"

/**
 * A class responsible for creating enemies.
 * This class spawns enemies at a given location.
 * Each spawner only spawns one type of enemy.
 */

class Spawner
{
    protected:
        /**
         * X coordinate of the spawner.
         */
        int x;

        /**
         * Y coordinate of the spawner.
         */
        int y;

        /**
         * Depth of the spawner in the chunk.
         */
        int depth;

        /**
         * The number of enemies that can be spawned.
         */
        int num_enemy;

        /**
         * The type of spawner associated with this.  Kind of redundant.
         */
        SpawnType spawn_type;

        /**
         * The type of enemy to spawn.
         */
        EnemyType enemy;

        /**
         * A list of the possible points that something can spawn.
         */
        std::vector<Den> spawn_points;

        /**
         * Queues up the enemies that have been created.
         */
        std::vector<Character*> enemy_queue;
        
        /**
         * Creates the dens for the enemies to live in.
         */
        void construct_den();
       
        /**
         * Makes some huts.
         */
        void construct_huts();

        /**
         * Makes a burrow.
         */
        void construct_burrow();

        /**
         * Checks to see if coordinates are overlapping with any of the dens.
         */
        bool overlapping_spawners(int x, int y, int radius);

    public:
        /*
         * The default constructor.
         */
        Spawner();

        /*
         * The constructor for the spanwer.
         */
        Spawner(int, int, int,  EnemyType);
        
        
        /**
         * Make the spawner do it's thing.
         */
        void run();

        /*
         * Determines whether or not an enemy should spawn.
         * Currently based entireliy on a 1/100 random chance of spawning/
         game tick.
         * \todo Make this more complex based on enemy type and other factors.
         * @return Bool depending on whether or not an enemy should spawn.
         */
        bool should_spawn();
        /**
         * Spawns a new enemy and adds it to the enemy queue.
         * @param chunk_x The x component of the chunk where the enemy should spawn.
         * @param chunk_y The y component of the chunk where the enemey should spawn.
         */
        void spawn_creep(int num_creeps);

        /**
         * Returns the enemies currently in the queue.
         */
        std::vector<Character*>& flush();

        /**
         * A non-memory safe clear of the queue. Nothing is deleted,
         * the list is just cleared.
         */
        void clear_queue();

        /**
         * A memory safe clear of the queue.  Everything is deleted, so
         * they SHOULD NOT BE ELSEWHERE.
         */
        void deep_clear();

        /**
         * Public accessor for the member x.
         */
        int get_x();

        /**
         * Public accessor for the member y.
         */
        int get_y();

        /**
         * Public accessor for the depth.
         */
        int get_depth();

        /**
         * Public accessor for enemy type.
         */
        int get_enemy_type_id();

        /**
         * Accessor for spawn_points.
         */
        std::vector<Den>& get_spawn_points();
        
        /**
         * Accessor for a spawn points pointer.
         */
        Den* get_spawn_at(int i);

        /**
         * Check if a given point is inside the spawner.
         */
        bool point_in_spawner(int _x, int _y);
};

#endif

