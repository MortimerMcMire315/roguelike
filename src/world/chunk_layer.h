/**
 *  @file CHUNK_LAYER.H
 *  @author Michael & Seth Yoder
 *
 *  @section LICENSE
 *
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

#ifndef CHUNK_LAYER
#define CHUNK_LAYER

#include <unordered_map>
#include <vector>

#include "defs.h"
#include "spawner.h"
#include "room.h"
#include "plant.h"
#include "building.h"

typedef std::vector<std::vector<Tile> > TileMatrix;
class ChunkLayer {

    private:
        //is this really the place for this?  The answer is no. It's not.
        static const int MAX_ROOMS=15;

        /**
         * Store a pointer to the global tileset.
         */
        std::unordered_map<std::string, Tile>* tileset;

        /**
         * The central data model for the class: A two-dimensional matrix
         * storing the layer's tiles.
         */
        TileMatrix ground;

        /**
         * A vector storing pointers to each item on the chunk.
         */
        std::vector<Item*> items;

        /**
         * The plants present in the given layer.  Shouldn't need to
         * be a pointer.
         */
        std::vector<Plant> plants;

        /**
         * The buildings present in the layer.
         */
        std::vector<Building> buildings;

        /**
         * A list of characters which have just been generated.
         */
        std::vector<Character*> characters;

        /**
         * Copies over all values from the given layer to this layer.
         * @param l - The layer from which to swap ownership.
         */
        void swap(const ChunkLayer& l);

        /**
         * @param row
         * @param col
         * Run the given point in the dungeon through a series of assertions
         * to ensure that it is valid.
         */
        void tile_assertions(int row, int col) const;


    public:
        ChunkLayer();
        ChunkLayer(int _width, int _height);
        ChunkLayer(int _width, int _height, bool has_layer_below);
        ChunkLayer(const ChunkLayer& l);

        /**
         * Resets the chunk layer to its default settings.
         */
        void clear();

        /**
         * A vector of IntPoints representing the location of down stairs on
         * this chunk.
         */
        std::vector<IntPoint> down_stairs;

        /**
         * The coordinates of the stair leading to the layer above this one, if
         * such a layer exists.
         */
        std::vector<IntPoint> up_stairs;

        /**
         * The width of this layer.
         */
        int width;

        /**
         * The height of this layer.
         */
        int height;

        /**
         * @param d a reference to another dungeon
         * @return a reference to this dungeon
         *
         */
        ChunkLayer& operator= (const ChunkLayer& d);

        /**
         * True if there is a layer below this one.
         */
        bool has_layer_below;

        /**
         * The number of rooms in this layer.
         * \todo figure out what to do with this.
         */
        int num_rooms;

        /**
         * A vector containing data about the rooms in this dungeon.
         */
        std::vector<Room> rooms;

        /**
         * Any monster spawners that are stored on this layer.
         */
        std::vector<Spawner> spawners;

        /**
         * Returns the tile at the given (row, col) location.
         *
         * @param row
         * @param col
         * @return a pointer to the tile at that location
         */
        Tile* get_tile_pointer(int row, int col);
        Tile* get_tile_pointer(IntPoint point);

        Tile get_tile(int row, int col) const;
        Tile get_tile(IntPoint point) const;

        /**
         * Sets the tile at the given location.
         *
         * @param row
         * @param col
         * @param tile_type
         */
        void set_tile(int row, int col, Tile tile_type);

        void set_tile(IntPoint point, Tile tile_type);

        /**
         * @return a reference to the matrix of tiles on this layer.
         */
        std::vector<std::vector<Tile> >& get_ground();

        /**
         * @return a pointer to the list of item pointers on this chunk.
         * \todo why a pointer and not a reference? There could be a legit reason. -Seth
         */
        std::vector<Item*>* get_items();

        /**
         * @return a vector of the equipment on this chunk.
         * \todo return a reference so we don't have to copy
         */
        std::vector<EquipType>& get_equipment();

        /**
         * @return a vector of the weapons on this chunk.
         * \todo return a reference so we don't have to copy
         */
        std::vector<WeaponType>& get_weapons();

        /**
         * @return the spawners on this chunk.
         */
        std::vector<Spawner>* get_spawners();

        /**
         * Returns the list of plants, as a reference.
         * This is to maintain integrity with the view
         * on the tiles getting set properly.
         */
        std::vector<Plant>* get_plants();

        /**
         * Gets a plant at the location. Returns NULL if there
         * is a plant.
         */
        Plant* get_plant(IntPoint coords);

        /**
         * Removes a plant from the layer.
         */
        void kill_plant(Plant *plant);

        /**
         * Adds an item to the item list for this chunk.
         * @param item - a pointer to the item to add.
         */
        void add_item(Item* item);

        /**
         * @param has_layer_below a bool denoting whether or not there is a
         * layer below this dungeon.
         *
         * Create an "up stair" in a random room and a "down stair" in a
         * random room (if there is a chunk below).
         */
        void make_stairs(bool has_layer_below);

        /**
         * @param row obvious
         * @param col obvious
         * @param tile is it an up_stair or a down_stair?
         *
         * Make stairs at given coordinates.
         */
        void make_stairs_at_coords(int row, int col, Tile stair_type);
        /**
         * @param depth the depth at which to build a spawner
         *
         * Builds a monster spawner in a random room at the given depth.
         * \todo should go in dungeon gen
         */
        void make_spawner(int depth);

        void make_spawner(int depth, IntPoint point);

        void make_spawner(int depth, IntPoint point, EnemyType enemy);

        /**
         * Adds a plant to the current layer.
         */
        void add_plant(Plant plant);

        /**
         * Checks to see if a point is in the layer.
         */
        bool in_layer(int x, int y);

        /**
         * Adds a building to the layer.
         */
        void add_building(Building building);

        /**
         * Returns the buildings in the layer.
         */
        std::vector<Building>* get_buildings();


        /**
         * Returns the characters created by this chunk, or by
         * spawners.
         */
        std::vector<Character*> get_character_queue();

        /**
         * Clears the characters from this chunk and from the
         * spawners. NOTE: Not memory safe.
         */
        void clear_character_queue();

        /**
         * Adds a character to the queue.
         */
        void add_character(Character* character);

        /**
         * Runs all of the spawners (has them spawn things)
         * in the layer.
         */
        void run_spawners();

        /**
         * Prints an ASCII representation of the layer to stdout.
         */
        void layer_dump();

};

#endif
