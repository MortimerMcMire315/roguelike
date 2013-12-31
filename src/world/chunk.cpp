/**
 *  CHUNK.CPP
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

#include <chunk.h>
using namespace std;

Chunk::Chunk() {
    initialized = false;
}

Chunk::Chunk(int _width, int _height, MapTile tile_type) {
    initialized = true;
    width = _width;
    height= _height;

    if(tile_type == map_tile::MAP_DEFAULT) {
        build_land_chunk();
    } else if (tile_type == map_tile::MAP_WATER) {
        build_water_chunk();
    } else if (tile_type == map_tile::MAP_BEACH) {
        build_beach_chunk();
    }
}

void Chunk::build_land_chunk() {
    depth = rand() % 6 + 1;
    dungeon_floors = vector<Dungeon>(depth, Dungeon(width, height));
    Dungeon* temp_d;
    ProcedurallyBlindDB db(width, height);
    //CorruptiblePBlindDB db(width, height);

    bool has_layer_below;

    for (int i=0; i < depth; i++) {
        has_layer_below = (i < depth - 1);
        db.build_dungeon(5, 5);
        temp_d = db.get_dungeon();

        dungeon_floors[i] = *temp_d;

        //makes the staircases in a dungeon
        dungeon_floors[i].make_stairs(has_layer_below);
        dungeon_floors[i].make_spawner(i);
    }
    //generate the overworld
    has_layer_below = (depth > 0);
    overworld = Overworld(width, height, has_layer_below, map_tile::MAP_DEFAULT);
}

void Chunk::build_water_chunk() {
    depth = 0;
    bool has_layer_below = false;
    overworld = Overworld(width, height, has_layer_below, map_tile::MAP_WATER);
}

void Chunk::build_beach_chunk() {
    depth = 0;
    bool has_layer_below = false;
    overworld = Overworld(width, height, has_layer_below, map_tile::MAP_BEACH);
}

const std::vector<std::vector<Tile> >& Chunk::get_floor(int depth) {
    if (depth == -1){
        return overworld.get_ground();
    } else {
        return dungeon_floors[depth].get_dungeon();
    }
}

Tile* Chunk::get_tile(int depth, int row, int col) {
    if (depth == -1){
        return overworld.get_tile(row, col);
    } else {
        return dungeon_floors[depth].get_tile_pointer(row, col);
    }
}

void Chunk::set_tile(int depth, int row, int col, Tile* tile){
    if (depth == -1) {
        overworld.set_tile(row, col, tile);
    } else {
        dungeon_floors[depth].set_tile(row, col, *tile);
    }
}

IntPoint Chunk::get_up_stair(int depth) const{
    assert(depth>=0);
    return dungeon_floors[depth].up_stair;
}

IntPoint Chunk::get_down_stair(int depth) const{
    if(depth==-1) {
        return overworld.down_stair;
    } else {
        return dungeon_floors[depth].down_stair;
    }
}

int Chunk::get_depth() const {
    return depth;
}

bool Chunk::is_initialized() const {
    return initialized;
}

bool Chunk::out_of_bounds(int _depth, int row, int col) const {
    return (_depth >= depth ||
            row >= height ||
            col >= width);
}

/*
 * For now, assume that there is one per floor
 * Later, I'll change this to a vector
 * But that will also have ramifications in how
 * the game will handle things.
 */

Spawner Chunk::get_spawner(int _depth) {
    if(_depth>=0) {
        return dungeon_floors[_depth].get_spawner();
    } else {
        return overworld.get_spawner();
    }
}

void Chunk::dungeon_dump(int _depth) {
    dungeon_floors[_depth].dungeon_dump();
}

/**
 * Serialized file looks like this:
 * HEADER:
 * 3 bytes: Width, Height, and Depth.
 * BODY:
 * 2 bytes per tile: tile_id; seen
 *
 * So the end file size is 3 + (2*w*h*d) bytes.
 */
void Chunk::serialize(int world_row, int world_col) {
    int chunk_depth = depth;
    stringstream ss;
    ss<<"chunk"<<world_row<<"_"<<world_col;
    string file_name = std::string(CHUNK_DIR) + std::string("/") + ss.str();

    ofstream chunk_data_file(file_name.c_str(),
            std::ofstream::out | std::ofstream::binary);

    int num_header_bytes = 3;
    int bytes_per_tile = 2;

    int file_size = 3 + (bytes_per_tile*width*height*(chunk_depth+1));

    char file[file_size];

    file[0] = width;
    file[1] = height;
    file[2] = chunk_depth;
    int current_byte = 3;

    int tile_id, seen;
    Tile current_tile;

    cout<<"height: "<<height<<endl;
    cout<<"width: "<<width<<endl;
    cout<<"chunk_depth: "<<chunk_depth<<endl;
    cout<<"serializing with file size: "<<file_size<<endl;
    for(int i = 0; i < (chunk_depth + 1); i++) {
        for(int j = 0; j < height; j++) {
            for(int k = 0; k < width; k++) {
                current_tile = *get_tile(i - 1, j, k);
                tile_id = current_tile.tile_id;
                seen = current_tile.seen;
                file[current_byte] = tile_id;
                file[current_byte+1] = seen;

                current_byte += bytes_per_tile;
            }
        }
    }

    for(int i = 0; i < file_size; i++) {
        chunk_data_file<<file[i];
    }

    chunk_data_file.close();
}
