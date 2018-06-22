/**
 *  @file CHUNK.H
 *  @author Michael and Seth Yoder
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

#ifndef _CHUNK_H
#define _CHUNK_H

#define CHUNK_DIR DATADIR "/save"

#include <vector>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <assert.h>
#include <ctime>

#include "constants.h"
#include "chunk_layer.h"
#include "defs.h"
#include "spring_matrix.h"
#include "building.h"
#include "city_district.h"
#include "block.h"

class Plant;
class Spawner;
class IntPoint;

namespace fs=boost::filesystem;

static const int BYTES_PER_TILE = 2;
static const int CHUNKLAYER_META_BYTES = 6; //spawner row/col, upstairs row/col, downstairs row/col
static const int CHUNK_META_BYTES = 6; //height, width, depth, chunk_type_id, world_row, world_col

typedef std::vector<std::vector<Tile> > TileMatrix;

struct chunk_meta {
    /**
     * This chunk's height in tiles (the number of rows)
     */
    int height;
    /**
     * This chunk's width in tiles (the number of columns).
     */
    int width;

    /**
     * An integer representing chunk depth. Chunk depth is equal to the
     * number of dungeons beneath the overworld. This means that ocean and
     * sand tiles have a depth of 0. If a chunk consists of a forest tile
     * with 4 dungeons below it, the chunk has a depth of 4.
     */
    int depth;

    /**
     * The ID of this chunk's map type
     */
    int chunk_type_id;

    /**
     * The index of the chunk's row on the world map.
     */
    int world_row;

    /**
     * The index of the chunk's column on the world map.
     */
    int world_col;
};

/**
 * The Chunk class is the largest data model in the game.
 *
 * Each Chunk contains a vector of dungeons and an overworld. Chunks are in
 * charge of their own loading method: When a new chunk is constructed or its
 * init() method is called (the first implies the second), it will look in the
 * chunk data folder to see if the chunk at its coordinates has previously been
 * serialized. If not, the chunk controls its own creation, and if so, the chunk
 * calls its deserialize() method, loading it with the appropriate data.
 */
class Chunk{
    typedef std::vector<std::vector<MapTile> > MapTileMatrix;
    private:

        /**
         * Full path to the directory that this chunk will be saved in.
         */
        string save_folder;

        /**
         * \todo
         */
        chunk_meta cm;

        /**
         * The type of this chunk, found in defs.h.
         * @see MapTile
         */
        MapTile chunk_type;

        /**
         * A vector containing the layers of this chunk. Generally this is the
         * main data chokepoint in the Chunk class.
         */
        std::vector<ChunkLayer> layers;

        /**
         * The entry point for deserialization. Will attempt to find an
         * offloaded chunk with the given coordinates. If an offloaded chunk is
         * found in the filesystem, calls the deserialize() function and returns
         * true. Otherwise, returns false.
         *
         * @param row - The world row of the chunk to find.
         * @param col - The world column of the chunk to find.
         * @return whether or not a serialized chunk was found.
         * @see deserialize
         */
        //bool find_serialized_chunk();

        /**
         * The heightmap of this chunk.
         */
        std::vector<std::vector<int> > heightmap;


    public:
        /**
         * Empty constructor. \todo check if this can be empty.
         */
        Chunk();

        /**
         * Typical use case constructor. Will handle file deserialization by
         * itself: When provided with a location, the function will decide
         * whether or not it should deserialize an offloaded chunk.
         *
         * @param tile_type - the type of the map tile on which this chunk lies.
         * @param _world_col - The row of this chunk's location on the world
         * map.
         * @param _world_col - The column of this chunk's location on the world
         * map.
         *
         * @see MapTile
         */
        Chunk(MapTile tile_type, int _world_row, int _world_col, string _save_folder, MapTileMatrix& map);

        /**
         * Resets all important information in the chunk, deserializing as
         * needed.
         *
         * @param tile_type - The type of the map tile on which this chunk lies.
         * @param _world_row - The row of this chunk's location on the world
         * map.
         * @param _world_col - The column of this chunk's location on the world
         * map.
         * @see deserialize
         * @see MapTile
         */
        void init(MapTile tile_type, int _world_row, int _world_col, string save_folder, MapTileMatrix& world_map);

        /**
         * \todo this documentation
         */
        bool build_chunk_with_dungeons();

        /**
         * Builds a chunk with dirt, trees, and dungeons.
         * Behaves as the default overworld.
         */
        void build_land_chunk();

        /**
         * Builds a chunk with grass, trees, groves, and dungeons.
         */
        void build_forest_chunk();

        /**
         * Builds a chunk with only a watery overworld.
         */
        void build_water_chunk();

        /**
         * Builds a chunk with only a sandy overworld.
         */
        void build_beach_chunk();

        /**
         * Builds a chunk with a city in it.  As of right now,
         * there's no continuity between adjacent chunks which
         * are city chunks, which is too bad, and I need to
         * think of a way to apply a single generated city
         * across multiple chunks.
         */
        void build_city_chunk();

        /**
         * Adds some dank trees to the current chunk.
         */
        void build_some_dank_trees();

        /**
         * Check to see if something can be built at a point in the chunk.
         */
        bool can_build(int depth, int x, int y);

        /**
         * Returns an IntPoint representing the chunk's location on the world
         * map.
         * \todo Just use an IntPoint to store world location in general.
         * @return the chunk's location on the world map
         */
        IntPoint get_world_loc() const;

        /**
         * Returns the location of the up stair in the chunk as an IntPoint.
         *
         * @param depth - the depth of this chunk.
         */
        std::vector<IntPoint> get_up_stairs(int depth) const;

        /**
         * Returns the location of the down stair in the chunk as an IntPoint.
         *
         * @param depth - the depth of this chunk.
         */
        std::vector<IntPoint> get_down_stairs(int depth) const;

        /**
         * Returns a vector of all the items on the ground in this chunk.
         * @param depth - the depth of this chunk.
         * @return a pointer to a vector of items in the chunk.
         * \todo should this return a reference instead of a pointer?
         */
        std::vector<Item*>* get_items(int depth);

        /**
         * Will remove the given item from the chunk.
         * @param item - a pointer to the item to remove
         * @param depth - the depth at which to remove the item.
         */
        void remove_item(Item* item, int depth);

        /**
         * Will add the given item to the chunk's item list.
         * @param item - a pointer to the item to add
         * @param depth - the depth at which to add the item.
         */
        void add_item(Item* item, int depth);

        /**
         * @param depth - the depth to get (-1 is the overworld)
         * @return a reference to the TileMatrix of the given floor
         */
        const std::vector<std::vector<Tile> >& get_floor(int depth);

        /**
         * @param depth
         * @param row
         * @param col
         * @return a pointer to the tile at the given location.
         */
        Tile* get_tile(int depth, int row, int col) ;

        /**
         * Sets a tile at the given position to the given tile type. If the
         * depth is -1, the tile will be set at the overworld.
         *
         * @param depth - the depth of this chunk.
         * @param row - the row of the tile to set.
         * @param col - the column of the tile to set.
         * @param tile - the tile type.
         */
        void set_tile(int depth, int row, int col, Tile tile);

        /**
         * Determine how many layers are beneath the overworld in this chunk.
         * @return the depth.
         */
        int get_depth() const;

        /**
         * Given a point, will determine whether the point is out of bounds.
         * @param depth
         * @param row
         * @param col
         * @return true if the given location is out of bounds on the chunk.
         */
        bool out_of_bounds(int depth, int row, int col) const;

        /**
         * Returns the spawner object at the given depth.
         * @param depth
         * @return the spawner object
         */
        std::vector<Spawner>* get_spawners(int depth);

        /**
         * Returns the plants at a given depth.
         */
        std::vector<Plant>* get_plants(int depth);


        /**
         * Removes a plant from the chunk.
         */
        void kill_plant(Plant *plant, int depth);

        /**
         * Get the heightmap for this chunk.
         */
        std::vector<std::vector<int> >* get_heightmap();

        /**
         * Prints a graphical representation of the given layer to stdout.
         * @param depth - the depth of the layer to print.
         */
        void dungeon_dump(int depth);

        /**
         * @return the type of chunk.
         */
        MapTile get_type();

        /**
         * Will return a file name for this chunk based on its
         * unique world location.
         *
         * @return a stringstream containing the file's name.
         */
        stringstream make_filename();

        /**
         * Given an int, will assume it is no more than 4 bytes long and pack it
         * into a char array at a given index.
         *
         * @param num the number to pack
         * @param file the char array to pack into
         * @param index the index to start at
         *
         * @return the new current index at the end of the sequence of 4 bytes.
         */
        int pack_int_into_char_array(int num, char* file, int index);

        /**
         * Given a char array and an index, will unpack 4 bytes into an int.
         *
         * @param file the char array
         * @param the_index index at which the sequence of 4 bytes begins
         *
         * @return the unpacked integer.
         */
        int char_array_to_int(char* file, int& the_index);

        /**
         * Determines the size of the file that will be created for this chunk.
         * @param bytes_per_tile the number of bytes expected per tile.
         * @return integer file size in bytes.
         */
        int calculate_file_size(int bytes_per_tile);

        /**
         * Serializes the file's header bytes from the chunk_meta struct.
         * @param file the file byte array
         * @return x the next index of the file array to write to.
         */
        //int serialize_metadata(char* file);

        /**
         * Serialize all the plants in a layer.
         * @param file the file byte array
         * @param layer the layer to work on
         * @current_byte the current byte in the file array.
         */
        //int serialize_plants(char* file, int layer, int current_byte);

        /**
         * Save metadata for each layer of this chunk.
         * @param file the file byte array to write to.
         * @param cb the index at which to begin writing.
         * @return the index after the last piece of layer metadata.
         */
        //int serialize_layer_metadata(char* file, int cb);

        /**
         * Save the chunk layers.
         * @param file the file byte array to write to.
         * @param cb the index at which to begin writing.
         * @return the index after the last piece of layer metadata.
         */
        //int serialize_layers(char* file, int cb);

        /**
         * Write to the save file in the CHUNK_DIR directory.
         * @param file the file data to write.
         * @param filename the name of this chunk file.
         */
        //void save_file(char* file, string filename, int file_size);

        /**
         * Saves all of the important information for this chunk in a file. The
         * file size is first carefully calculated, then a char array is created
         * with that size and populated with the dungeon data. The file is saved
         * in data/chunk.
         *
         * Some bitwise arithmetic is used to pack ints and bools together in a
         * single byte. As we get over 128 tile IDs, we will have to change this
         * functionality.
         */
        //void serialize();

        /**
         * Set Chunk attributes to those found in the file data array.
         * @param file the data file
         */
        //void deserialize_metadata(char* file);

        /**
         * For each layer, set layer attributes to those found in the
         * file data array.
         * @param file the data file
         * @param cb the byte at which to begin reading
         * @return index of the byte after the last metadata byte
         */
        //int deserialize_layer_metadata(char* file, int cb);

        /**
         * Deserialize all the plants in a layer.
         *
         * @param file the data file array
         * @param layer the layer to work on
         * @param cb the current byte in the file array
         * @param num_plants the number of plants in this layer.
         */
        //int deserialize_plants(char* file, int layer, int cb, int num_plants);

        /**
         * Populate this chunk's layer array with data from the file array.
         * @param file the data file
         * @param cb the byte at which to begin reading
         * @return index of the byte after the last metadata byte
         */
        //int deserialize_layers(char* file, int cb);

        /**
         * Loads this chunk's information from a file. Essentially does a simple
         * reverse of the serialization process. The "header" bytes, which
         * always must exist in the file, are processed one-by-one and loaded
         * into the class member variables to restore the data. Then, data which
         * may or may not exist (such as stair/spawner locations) is loaded
         * based on header information. Next, the bytes of the dungeon and
         * overworld are loaded in a triple-nested for-loop.
         *
         * @param file_name - The name of the file from which the chunk will be
         * loaded.
         * @param world_row - This chunk's row on the world map.
         * @param world_col - This chunk's column on the world map.
         */
        //void deserialize(string file_name);


        /**
         * Repsonsible for blending chunks of different types.  Takes
         * in the world map, the change in chunk x/y of the chunk to
         * blended, and then passes it to the appropriate function
         * based on type of chunk.
         */
        void blend_chunk(MapTileMatrix& map, int row_change, int col_change);

        /**
         * Blends chunks that should have a "hard line" between them,
         * e.g. water and anything else.
         */
        void blend_hard(int row, int col, MapTile other);

        /**
         * Blends chunks "normally," so that each tile in a range has a
         * 50% chance of becoming the other chunk's base tile.
         */
        void blend_normal(int row, int col, MapTile other);

        /**
         * Finds a plant at the given coords.  Returns NULL if none is found.
         */
        Plant* get_plant(IntPoint coords, int depth);


        /**
         * Adds a building to the chunk.
         */
        void add_building(Building building, int depth);

        /**
         * Gets the buildings from the chunk.
         */
        std::vector<Building>* get_buildings(int depth);

        /**
         * Returns the characters which have been created in a parcticular chunk.
         */
        std::vector<Character*> get_character_queue(int depth);

        /**
         * Clears the character queue in a particular layer.
         */
        void clear_character_queue(int depth);

        /**
         * Runs the spawners in a particular layer.
         */
        void run_spawners(int depth);


};

#endif
