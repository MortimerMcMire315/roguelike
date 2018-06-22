/**
 *  @file CHUNK.CPP
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

#include <iostream>

#include "procedurally_blind_db.h"
#include "corruptible_pblind_db.h"
#include "overworld_gen.h"
#include "spawner.h"
#include "plant.h"
#include "tileset.h"
#include "chunk.h"

Chunk::Chunk() {
    cm.height = CHUNK_HEIGHT;
    cm.width = CHUNK_WIDTH;
}

Chunk::Chunk(MapTile tile_type, int world_row, int world_col, string _save_folder, MapTileMatrix& map) {
    cm.height = CHUNK_HEIGHT;
    cm.width = CHUNK_WIDTH;
    cm.chunk_type_id = tile_type.id;
    init(tile_type, world_row, world_col, _save_folder, map);
}

/*
bool Chunk::find_serialized_chunk() {
    fs::path chunk_dir(save_folder);

    stringstream ss;
    ss << save_folder << "/chunk" << cm.world_row << "_" <<cm.world_col;
    string filename = ss.str();

    if(fs::exists(chunk_dir) && fs::is_directory(chunk_dir)) {
        if(fs::exists(filename)) {
            deserialize(filename);
            return true;
        }
    } else {
        cout<<"Chunk directory is missing. Aborting."<<endl;
        exit(EXIT_FAILURE);
    }

    return false;
}
*/

void Chunk::init(MapTile tile_type, int world_row, int world_col, string _save_folder, MapTileMatrix& world_map) {
    save_folder = _save_folder;
    chunk_type = tile_type;
    cm.world_row = world_row;
    cm.world_col = world_col;
    cm.chunk_type_id = tile_type.id;

    //bool found_chunk = find_serialized_chunk();
    //if(!found_chunk) {
        if(tile_type == map_tile::MAP_DEFAULT) {
            build_land_chunk();
        } else if (tile_type == map_tile::MAP_WATER) {
            build_water_chunk();
        } else if (tile_type == map_tile::MAP_BEACH) {
            build_beach_chunk();
        } else if (tile_type == map_tile::MAP_FOREST) {
            build_forest_chunk();
        } else if (tile_type == map_tile::CITY) {
            build_city_chunk();
        } else if (tile_type == map_tile::CITY_NOBILITY) {
            build_city_chunk();
        } else if (tile_type == map_tile::CITY_MARKET) {
            build_city_chunk();
        } else if (tile_type == map_tile::CITY_RESIDENTIAL) {
            build_city_chunk();
        }
        blend_chunk(world_map, -1, 0);
        blend_chunk(world_map, 1, 0);
        blend_chunk(world_map, 0, 1);
        blend_chunk(world_map, 0, -1);
    //}
}

bool Chunk::build_chunk_with_dungeons() {
    cm.depth = rand() % 6 + 1;

    layers = std::vector<ChunkLayer>(cm.depth, ChunkLayer(cm.width, cm.height));
    //CorruptiblePBlindDB db(cm.width, cm.height);

    layers[0].has_layer_below = (cm.depth > 1);

    bool has_layer_below;
    //Don't generate the top layer; it will be an overworld.
    for (int i=1; i < cm.depth; i++) {
        has_layer_below = (i < cm.depth - 1);
        pblind_db::build_dungeon(cm.width, cm.height, 5, layers[i]);

        //makes the staircases in a dungeon
        layers[i].make_stairs(has_layer_below);
        layers[i].has_layer_below = has_layer_below;
    }
}

void Chunk::build_land_chunk() {
    build_chunk_with_dungeons();
    overworld_gen::build_land_overworld(layers[0]);
}

void Chunk::build_forest_chunk() {
    build_chunk_with_dungeons();
    overworld_gen::build_forest_overworld(layers[0]);

    //TODO: probably rename this
    build_some_dank_trees();
}


void Chunk::build_water_chunk() {
    cm.depth = 1;
    layers = std::vector<ChunkLayer>(1, ChunkLayer(cm.width, cm.height));
    overworld_gen::build_water_overworld(layers[0]);
}

void Chunk::build_beach_chunk() {
    cm.depth = 1;
    layers = std::vector<ChunkLayer>(1, ChunkLayer(cm.width, cm.height));
    overworld_gen::build_beach_overworld(layers[0]);
}

void Chunk::build_city_chunk() {
    //TODO: Make this so that it can take in the dungeon
    //type to build.  In cities, I want sewers! Because
    //medieval cities definitely had running water ;)
    build_chunk_with_dungeons();
    int height = layers[0].height;
    int width = layers[0].width;
    overworld_gen::build_city_overworld(layers[0]);
    CityDistrict settlement = CityDistrict(0, 0, height, width, map_tile::CITY);
    std::vector<Block> blocks = settlement.get_blocks();

    for(int i=0;i<blocks.size();i++)
    {
        std::vector<Building> builds = blocks[i].get_buildings();
        for(int j=0;j<builds.size();j++)
        {

            add_building(builds[j], 0);
            Enemy* chara = new Enemy(builds[j].get_x() + 1, builds[j].get_y() + 1, 0, enemies::human);
            layers[0].add_character(chara);
            builds[j].add_owner(chara);

            int cob_x = rand() % 2 + 1;
            int cob_y = rand() % 2 + 1;

            std::unordered_map<std::string, Tile>* tileset = &Tileset::instance()->get_tileset();
            for(int row=builds[j].get_y() - cob_y;row<builds[j].get_height() + builds[j].get_y() + cob_y;row++)
            {
                for(int col=builds[j].get_x() - cob_x;col<builds[j].get_width() + builds[j].get_x() + cob_x;col++)
                {
                    layers[0].set_tile(row, col, (*tileset)["DIRT"]);//map_tile::MAP_TILE_INDEX[cm.chunk_type_id].base_tile);
                }
            }
        }

    }
}


void Chunk::build_some_dank_trees()
{
    //erm...let's have a tree density
    //(that's trees/tile sq)
    int min = 1;
    int max = 10;
    int dist_between_trees = (min+max)/2;
    int padding = 0;
    int tree_size = 2;

    int x_trees = (cm.width - padding * 2)/(dist_between_trees + tree_size) + 1;
    int y_trees = (cm.height - padding * 2)/(dist_between_trees + tree_size) + 1;

    IntPoint trees_per_side = IntPoint(y_trees, x_trees);
    SpringMatrix mat = SpringMatrix(trees_per_side, tree_size, min, max, padding);

    mat.deform_matrix(1);
    std::vector<SpringPoint*> points = mat.get_matrix();
    for(int i=0;i<points.size();i++)
    {
        int x = points[i]->get_x();
        int y = points[i]->get_y();
        if(can_build(0, x, y))
        {
            layers[0].add_plant(Plant(x, y, cm.world_col, cm.world_row, plants::tree));
        }
    }
}

/**
 * MAKE THIS GENERALIZED FOR ALL CHUNKS/LAYERS.
 */
bool Chunk::can_build(int depth, int x, int y)
{
    if(!layers[depth].in_layer(x, y))
    {
        return false;
    }
    bool can_build = layers[depth].get_tile(y, x).can_build_overtop;

    //lols checkout this line count tradeoff: the above line versus the rest of
    //this function. Keeping this here so i can brag about it -SAY 12/21/2014

    //Actually...this won't work.  The spawners aren't in the ground anymore.
    //Plus, the spawners aren't a single tile, they're a matrix, so we don't want it
    //building in the middle.  Also plus, this doesn't do what I was going to add next,
    //which was check whether or not the x and y are even in the tile matrix.
    //-MJY 12/21/2014 (slightly later)

    //Is it in a spawner?
    bool in_spawner = false;
    std::vector<Spawner>* spawners = get_spawners(depth);
    for(int i=0;i<spawners->size();i++)
    {
        if(spawners->at(i).point_in_spawner(x, y))
        {
            in_spawner = true;
        }
    }

    return  can_build && !in_spawner;
}

IntPoint Chunk::get_world_loc() const{
    return IntPoint(cm.world_row, cm.world_col);
}

std::vector<IntPoint> Chunk::get_up_stairs(int depth) const{
    assert(depth>0);
    assert(cm.depth > depth);
    return layers[depth].up_stairs;
}

std::vector<IntPoint> Chunk::get_down_stairs(int depth) const{
    assert(cm.depth > depth);
    return layers[depth].down_stairs;
}

std::vector<Item*>* Chunk::get_items(int depth) {
    return layers[depth].get_items();
}

void Chunk::remove_item(Item* item, int depth) {
    vector<Item*>* items = get_items(depth);
    for(int i = 0; i<items->size();i++) {
        if(items->at(i) == item) {
            items->erase(items->begin() + i);
        }
    }
}

void Chunk::add_item(Item* item,  int depth) {
    assert(depth < cm.depth);
    layers[depth].add_item(item);
}

const std::vector<std::vector<Tile> >& Chunk::get_floor(int depth) {
    return layers[depth].get_ground();
}

Tile* Chunk::get_tile(int depth, int row, int col) {
    assert(!out_of_bounds(depth, row, col));
    return layers[depth].get_tile_pointer(row, col);
}

void Chunk::set_tile(int depth, int row, int col, Tile tile){
    layers[depth].set_tile(row, col, tile);
}

int Chunk::get_depth() const {
    return cm.depth;
}

bool Chunk::out_of_bounds(int _depth, int row, int col) const {
    return (_depth >= cm.depth ||
            row >= cm.height ||
            col >= cm.width ||
            _depth < 0 ||
            row < 0 ||
            col < 0);
}

/*
 * For now, assume that there is one per floor
 * Later, I'll change this to a vector
 * But that will also have ramifications in how
 * the game will handle things.
 */
std::vector<Spawner>* Chunk::get_spawners(int _depth) {
    ChunkLayer* current = &layers[_depth];
    return current->get_spawners();
}

std::vector<Plant>* Chunk::get_plants(int _depth) {
    ChunkLayer* current = &layers[_depth];
    return current->get_plants();
}

std::vector<std::vector<int> >* Chunk::get_heightmap() {
    return &heightmap;
}

void Chunk::dungeon_dump(int _depth) {
    layers[_depth].layer_dump();
}

MapTile Chunk::get_type() {
    return chunk_type;
}

void Chunk::kill_plant(Plant *plant, int depth)
{
    layers[depth].kill_plant(plant);
}

//===========SERIALIZATION/DESERIALIZATION=========


/*
int Chunk::pack_int_into_char_array(int num, char* file, int index) {
    unsigned int n = (unsigned int) num;
    file[index] = (unsigned char)   ((n & 0xff000000) >> 24);
    file[index+1] = (unsigned char) ((n & 0x00ff0000) >> 16);
    file[index+2] = (unsigned char) ((n & 0x0000ff00) >> 8);
    file[index+3] = (unsigned char)  (n & 0x000000ff);

    return index + 4;
}

int Chunk::char_array_to_int(char* file, int& the_index) {
    int index = the_index;

    int num = (((unsigned char) file[index]) << 24) |
              (((unsigned char) file[index+1]) << 16) |
              (((unsigned char) file[index+2]) << 8) |
              ((unsigned char) file[index+3]);
    assert(num >= 0);

    the_index += 4;
    return num;
}

int Chunk::calculate_file_size(int bytes_per_tile) {
    int bytes_per_layer = 0;
    for(int i = 0; i < cm.depth; i++) {
        bytes_per_layer += 3; //num_spawners, num_*_stairs
        bytes_per_layer += 4; //num_plants
        for(int j = 0; j < layers[i].spawners.size(); j++) {
            bytes_per_layer += 3;
        }
        for(int j = 0; j < layers[i].down_stairs.size(); j++) {
            bytes_per_layer += 2;
        }
        for(int j = 0; j < layers[i].up_stairs.size(); j++) {
            bytes_per_layer += 2;
        }
        for(int j = 0; j < layers[i].get_plants()->size(); j++) {
            bytes_per_layer += 4;
            Plant* current_plant = &(*(layers[i].get_plants()))[j];

            for(int k = 0; k < current_plant->get_sprites()->size(); k++) {
                for(int l = 0; l < (*(current_plant->get_sprites()))[k].size(); l++) {
                    bytes_per_layer += 2;
                }
            }
        }
    }

    bytes_per_layer += (BYTES_PER_TILE * cm.width * cm.height * cm.depth);

    return (CHUNK_META_BYTES + bytes_per_layer);
}

int Chunk::serialize_metadata(char* file) {
    file[0] = cm.width;
    file[1] = cm.height;
    file[2] = cm.depth;
    file[3] = cm.chunk_type_id;
    file[4] = cm.world_row;
    file[5] = cm.world_col;

    return 6;
}

int Chunk::serialize_plants(char file[], int layer, int current_byte) {
    int cb = current_byte;
    Plant* this_plant;
    std::vector<std::vector<Tile> >* plant_tiles;
    std::vector<Plant>* plants = layers[layer].get_plants();

    for(int i = 0; i < plants->size(); i++) {
        this_plant = &(*plants)[i]; //lol wat
        plant_tiles = this_plant->get_sprites();
        file[cb + 0] = this_plant->get_y();
        file[cb + 1] = this_plant->get_x();
        file[cb + 2] = plant_tiles->size();
        file[cb + 3] = plant_tiles[0].size();
        cb += 4;

        for(int j = 0; j < plant_tiles->size(); j++) {
            for(int k = 0; k < ((*plant_tiles)[j]).size(); k++) {
                file[cb + 0] = ((*plant_tiles)[j])[k].tile_id;
                file[cb + 1] = ((*plant_tiles)[j])[k].seen;
                cb += 2;
            }
        }
    }

    return cb;
}

int Chunk::serialize_layer_metadata(char file[], int cb) {
    int current_byte=cb;

    for(int i = 0; i < cm.depth; i++) {
        //IMPORTANT: These lines store how many spawners, down_stairs, and up_stairs
        //           to expect when deserializing.
        file[current_byte + 0] = layers[i].spawners.size();
        file[current_byte + 1] = layers[i].down_stairs.size();
        file[current_byte + 2] = layers[i].up_stairs.size();
        current_byte += 3;

        current_byte = pack_int_into_char_array(layers[i].get_plants()->size(), file, current_byte);

        current_byte = serialize_plants(file, i, current_byte);
        //cout<<"SERIALIZING: Chunk "<<cm.world_row<<", "<<cm.world_col<<", "<<i<<" : "<<current_byte<<endl;

        for(int j = 0; j < layers[i].spawners.size(); j++) {
            file[current_byte + 0] = layers[i].spawners[j].get_y();
            file[current_byte + 1] = layers[i].spawners[j].get_x();
            file[current_byte + 2] = layers[i].spawners[j].get_enemy_type_id();
            current_byte += 3;
        }

        for(int j = 0; j < layers[i].down_stairs.size(); j++) {
            file[current_byte + 0] = layers[i].down_stairs[j].row;
            file[current_byte + 1] = layers[i].down_stairs[j].col;
            current_byte += 2;
        }

        for(int j = 0; j < layers[i].up_stairs.size(); j++) {
            file[current_byte + 0] = layers[i].up_stairs[j].row;
            file[current_byte + 1] = layers[i].up_stairs[j].col;
            current_byte += 2;
        }
    }

    return current_byte;
}

int Chunk::serialize_layers(char file[], int cb) {
    int current_byte=cb;

    char tile_id, seen;
    Tile current_tile;

    for(int i = 0; i < cm.depth; i++) {
        for(int j = 0; j < cm.height; j++) {
            for(int k = 0; k < cm.width; k++) {
                current_tile = *get_tile(i, j, k);
                tile_id = (char) current_tile.tile_id;
                seen = (char) current_tile.seen;

                file[current_byte] = tile_id;
                file[current_byte+1] = seen;

                current_byte += BYTES_PER_TILE;
            }
        }
    }

    return current_byte;
}


void Chunk::save_file(char file[], string filename, int file_size) {
    string full_path = save_folder + std::string("/") + filename;
    std::ofstream chunk_data_file;

    //TODO Valgrind complains about the following line.
    chunk_data_file.open(full_path.c_str(), std::ofstream::out | std::ofstream::binary);

    for(int i = 0; i < file_size; i++) {
        //TODO Valgrind complains about the following line.
        chunk_data_file<<file[i];
    }

    chunk_data_file.close();
}


void Chunk::serialize() {
    stringstream ss;
    ss<<"chunk"<<cm.world_row<<"_"<<cm.world_col;

    int file_size = calculate_file_size(BYTES_PER_TILE);

    //Set up the file array to write to
    char file[file_size];
    int current_byte = serialize_metadata(file);
    current_byte = serialize_layer_metadata(file, current_byte);
    current_byte = serialize_layers(file, current_byte);
    assert(current_byte == file_size);

    save_file(file, ss.str(), file_size);
}

void Chunk::deserialize_metadata(char file_data[]) {
    cm.width = file_data[0];
    cm.height = file_data[1];
    cm.depth = file_data[2];
    cm.chunk_type_id = file_data[3];
    cm.world_row = file_data[4];
    cm.world_col = file_data[5];

     //The map tile ID was stored, now we just reverse it to get the chunk type.
    chunk_type = map_tile::MAP_TILE_INDEX[file_data[3]];
    //TODO return 6
}

int Chunk::deserialize_plants(char file[], int layer, int current_byte, int num_plants) {
    int cb = current_byte;
    int tiles_rows, tiles_cols;
    Plant this_plant;
    std::vector<std::vector<Tile> > plant_tiles;

    for(int i = 0; i < num_plants; i++) {
        this_plant.set_y(file[cb + 0]);
        this_plant.set_x(file[cb + 1]);
        tiles_rows = file[cb + 2];
        tiles_cols = file[cb + 3];
        plant_tiles = std::vector<std::vector<Tile> >(tiles_rows, std::vector<Tile>(tiles_cols));

        cb += 4;

        for(int j = 0; j < tiles_rows; j++) {
            for(int k = 0; k < tiles_cols; k++) {
                //TODO plants won't load in correctly. We'll have to find a way
                //to also hash tiles by id. For now, they'll be rabbit corpses.
                plant_tiles[j][k] = Tileset::get("RABBIT_CORPSE");
                plant_tiles[j][k].seen = file[cb + 1];
                cb += 2;
            }
        }

        this_plant.set_sprites(plant_tiles);

        layers[layer].add_plant(this_plant);
    }

    return cb;
}

int Chunk::deserialize_layer_metadata(char file_data[], int cb) {
    char num_spawners, spawner_row, spawner_col, num_down_stairs, num_up_stairs;
    EnemyType enemy_type;
    int current_byte = cb;
    int enemy_list_index;
    int num_plants;

    for(int i = 0; i < cm.depth; i++) {
        num_spawners = file_data[current_byte + 0];
        num_down_stairs = file_data[current_byte + 1];
        num_up_stairs = file_data[current_byte + 2];
        current_byte += 3;

        //current_byte gets updated because it is passed by reference.
        num_plants = char_array_to_int(file_data, current_byte);
        //cout<<"NUM_PLANTS: "<<num_plants<<endl;

        current_byte = deserialize_plants(file_data, i, current_byte, num_plants);
        //cout<<"DESERIALIZING: Chunk "<<cm.world_row<<", "<<cm.world_col<<", "<<i<<" : "<<current_byte<<endl;


        for(int j = 0; j < num_spawners; j++) {
            spawner_row = file_data[current_byte + 0];
            spawner_col = file_data[current_byte + 1];
            enemy_list_index = file_data[current_byte + 2];
            enemy_type = enemies::ENEMY_LIST[enemy_list_index];
            layers[i].make_spawner(i, IntPoint(spawner_row, spawner_col), enemy_type);
            current_byte += 3;
        }

        for(int j = 0; j < num_down_stairs; j++) {
            layers[i].make_stairs_at_coords(file_data[current_byte], file_data[current_byte + 1], Tileset::get("DOWN_STAIR"));
            current_byte += 2;
        }

        for(int j = 0; j < num_up_stairs; j++) {
            layers[i].make_stairs_at_coords(file_data[current_byte], file_data[current_byte + 1], Tileset::get("UP_STAIR"));
            current_byte += 2;
        }

        layers[i].has_layer_below = (i < (cm.depth - 1));
    }

    return current_byte;
}

int Chunk::deserialize_layers(char file_data[], int cb) {
    int current_byte = cb;

    Tile current_tile;
    unsigned int tile_id;
    bool seen;
    //Undo the serialization.
    for(int i = 0; i < cm.depth; i++) {
        for(int j = 0; j < cm.height; j++) {
            for(int k = 0; k < cm.width; k++) {
                tile_id = (file_data[current_byte]);
                seen = file_data[current_byte + 1];
                current_tile=tiledef::TILE_INDEX[tile_id];
                current_tile.seen = seen;

                set_tile(i, j, k, current_tile);
                current_byte += BYTES_PER_TILE;
            }
        }
    }

    return current_byte;
}

void Chunk::deserialize(string file_name) {
    //Open the data file.
    std::ifstream chunk_data_file(file_name.c_str(),
            std::ifstream::in | std::ifstream::binary);
    //Stat the file to get the file size easily.
    int file_size = fs::file_size(file_name);

    //Initialize an array for the file data.
    char * file_data = new char[file_size];

    //Read the entire file into the file_data array.
    chunk_data_file.read(file_data, file_size);

    deserialize_metadata(file_data);

    int current_byte=CHUNK_META_BYTES;
    layers = vector<ChunkLayer>(cm.depth, ChunkLayer(cm.width, cm.height));

    current_byte = deserialize_layer_metadata(file_data, current_byte);

    current_byte = deserialize_layers(file_data, current_byte);
    assert(current_byte == file_size);

    //Cleaning up.
    delete [] file_data;
    chunk_data_file.close();
}
*/

void Chunk::blend_chunk(MapTileMatrix& map, int row_change, int col_change)
{
    int new_row = cm.world_row + row_change;
    int new_col = cm.world_col + col_change;
    MapTile other;
    if(new_row >= 0 && new_row < map.size() &&  new_col >= 0 && new_col < map[new_row].size())
    {
        other = map[new_row][new_col];
    }
    else
    {
        return;
    }

    if(other.base_tile != chunk_type.base_tile)
    {
        //Keep in mind that if you have two map tiles, one with NORMAL and one
        //with HARD, both of these if-statements are technically valid. So
        //there's a bit of redundancy and HARD takes precedence... so NORMAL
        //blending will only ever happen if both chunks have NORMAL. You
        //probably knew this, but it's hard to tell from the way the code is
        //laid out. Otherwise this is freaking sweet. -SAY

        //That's actually exactly what I wanted to happen.  I forget if I made a note of
        //this in the commit log, but there's an order of precendence--if one chunk has a
        //"hard" blend type (e.g. water), then I want a hard line between them, regardless
        // of the other chunks blend type.  The redundancy is due to scalability, so you just
        //pick where you want the new blend type to take precedence and then stick it in the
        //"else if" tree.
        //-MJY

        if(other.blend_type == map_tile::HARD || chunk_type.blend_type == map_tile::HARD)
        {
            blend_hard(row_change, col_change, other);
        }
        else if(other.blend_type == map_tile::NORMAL || chunk_type.blend_type == map_tile::NORMAL)
        {
            blend_normal(row_change, col_change, other);
        }
    }
}

void Chunk::blend_hard(int row, int col, MapTile other)
{
    //Do nothing for this for now
}

void Chunk::blend_normal(int row, int col, MapTile other)
{

    //Check where we should start blending from
    //The only time we don't want to start at a 0 point is
    //if the change is 1.
    int start_x = 0 + ((col == 1) * (CHUNK_WIDTH - 1));
    int start_y = 0 + ((row == 1) * (CHUNK_HEIGHT - 1));

    //Find the range we want to run.  If the chunk change for that
    //coord is not zero, then that's the direction we want to blend.
    //Otherwise, we want it to run the breadth of the chunk.
    int x_range = ((CHUNK_WIDTH * .125) * (col != 0)) + (CHUNK_WIDTH * (col == 0));
    int y_range = ((CHUNK_HEIGHT * .25) * (row != 0)) + (CHUNK_HEIGHT * (row == 0));

    //Finds the direction we need to iterate in.
    int x_iter = 0 + (col <= 0) - (col > 0);
    int y_iter = 0 + (row <= 0) - (row > 0);

    //Find the end coordinate for our iteration
    int x_end = start_x + (x_range * x_iter);
    int y_end = start_y + (y_range * y_iter);


    for(int i = start_y;i != y_end;i += y_iter)
    {
        for(int j = start_x; j != x_end; j += x_iter)
        {
            //Determien the chance for any tile to become the other's base tile.
            int y_chance = 4 + ((CHUNK_HEIGHT/2) - std::abs((CHUNK_HEIGHT/2) - i));
            int x_chance = 4 + ((CHUNK_WIDTH/2) - std::abs((CHUNK_WIDTH/2) - j));
            int chance = (y_chance/2 * (row != 0)) + (x_chance/2 * (col  != 0));
            if(rand() % chance == 0)
            {
                layers[0].set_tile(i, j, other.base_tile);
            }
        }
    }
}

Plant* Chunk::get_plant(IntPoint coords, int depth)
{
    return layers[depth].get_plant(coords);
}

void Chunk::add_building(Building building, int depth)
{
    layers[depth].add_building(building);
}

std::vector<Building>* Chunk::get_buildings(int depth)
{
    return layers[depth].get_buildings();
}

std::vector<Character*> Chunk::get_character_queue(int depth)
{
    return layers[depth].get_character_queue();
}

void Chunk::clear_character_queue(int depth)
{
   layers[depth].clear_character_queue();
}

void Chunk::run_spawners(int depth)
{
    layers[depth].run_spawners();
}
