/**
 *  GAME.H
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

#ifndef _GAME_H
#define _GAME_H

#include <vector>
#include <unordered_map>
#include <string>

#include "chunk_matrix.h"
#include "constants.h"
#include "enemy.h"
#include "world_map.h"
#include "utility.h"
#include "item.h"
#include "animation.h"
#include "animation_defs.h"
#include "defs.h"
#include "chunk.h"
#include "character.h"
#include "bresenham.h"
#include "pathfinding.h"
#include "message.h"
#include "tileset.h"

//Forward declarations
struct Tile;
struct Item;

class Chunk;
class Character;
class Spawner;
class Den;
class Plant;

class Game
{
    typedef std::vector<std::vector<Tile> > TileMatrix;
    typedef std::vector<std::vector<Tile*> > TilePointerMatrix;
    typedef std::vector<std::vector<MapTile> > MapTileMatrix;

    private:


//----------------------------------GAME DATA-----------------------------------//
//src/controller/game.cpp

        /**
         * Whether or not the game has been initialized.
         */
        bool initialized;

        /**
         * The name of this saved game. Maybe eventually it will be based on a
         * randomly-generated world name, but for now it is generated based on
         * the date and time.
         */
        string name;

        /**
         * If the game is currently paused.
         * This isn't a game state because things still need to
         * be happening in other game states while the game is paused.
         */
        bool paused;

        /**
         * The map of the world.
         * @see MapTile
         */
        WorldMap world_map;

        /**
         * The list of points which correspond to visible tiles.
         */
        std::vector<std::vector<IntPoint> > bresenham_lines;

//--------------------------------------BUFFER DATA/PRIVATE METHODS----------------------------------------//
//src/controller/buffer_controller.cpp
//
        /**
         * The tile that will be used to fill buffer spaces.
         */
        Tile buffer_tile_placeholder;

        /**
         * A 2D vector of pointers to tile objects that represents all of the tiles
         * in the chunks surrounding the player.  This allows for easy writing to the
         * screen.
         */
        TilePointerMatrix buffer;

        /**
         * Converts coordinates in the form of (chunk, coords) to coordinates relative to
         * the top left of the buffer.
         */
        IntPoint get_buffer_coords(IntPoint chunk, IntPoint coords);

        /**
         * Returns whether or not the chunk is currently in the buffer.
         */
        bool chunk_in_buffer(int row, int col);

        /**
         * Checks whether a set of coordinates (relative to the buffer)
         * is inside the buffer.
         */
        bool coords_in_buffer(int row, int col);


//------------------------------CHARACTER DATA/PRIVATE METHODS------------------//
//src/controller/character_controller.cpp
        /**
         * The list of characters in the game.
         */
        std::vector<Character*> character_list;

        /**
         * A two dimensional matrix to hold characters.
         */
        std::vector<std::vector<Character*> > character_index;


//-------------------------------CANVAS DATA/Private Methods--------------//
//src/controller/canvas_controller.cpp

        /**
         * A 2D vector of pointers of tiles that will get written to the
         * screen on the next pass.  Point to tiles in chunks.
         */
        TilePointerMatrix canvas;


        /**
         * Assertions to ensure that the current point is in the screen.
         * @param row The row of the point.
         * @param col The column of the point.
         */
        void point_assertions(int row, int col);



        /**
         * Checks if the point at (point.row, point.col) is out of the screen.
         */
        bool out_of_bounds(IntPoint point);

        /**
         * Checks if the point at (row, col) is out of the screen.
         */
        bool out_of_bounds(int row, int col);



//-----------------------CHUNK MAP DATA/PRIVATE METHODS-------------------//
//src/controller/chunkmap_controller.cpp

        /**
         * A two dimensional vector containing the chunks.
         * This is the core of how the game doesn't run out of
         * memory.  It's broken up into chunks, which are then
         * serialized/deserialized as the player moves about.
         * @see Chunk
         */
        ChunkMatrix chunk_map;

        /**
         * Gets the item at the specific coordinates.
         * @param coords The coordinates in the chunk.
         * @param chunk The chunk of the item.
         * @return The item at the coordinates. NULL if none is found.
         */
        Item* item_at_coords(IntPoint, IntPoint, int);

//-------------------------------SPAWNER DATA----------------------------//
//src/controller/spawn_controller.cpp

        /**
         * Any character in this list will be added to the appropriate
         * behavior tree on the next pass of the gui.
         */
        std::vector<Character*> character_queue;


//------------------------------ANIMATION DATA---------------------------//
//src/controller/animation_controller.cpp
        /**
         * Whenever a new animation needs to be performed, it
         * should be added to this queue.
         */
        std::vector<Animation> anim_queue;



    public:

//------------------------------GAME DATA/FUNCTIONS--------------------------//
//src/controller/game.cpp

        /**
         * The default constructor.
         */
        Game();

        /**
         * The destructor, removing all of the enemies in the game.
         */
        ~Game();

        /**
         * Creates an initialized game from a set of maptiles and the chunk
         * that was selected by the user.
         */
        void init(const WorldMap&, IntPoint);

        /**
         * Gets the name of the current world (for saving, eventually).
         */
        string get_name();

        /**
         * Sets the name of the current world.
         */
        void set_name(string n);

        /**
         * Checks to see if the world has been initialized (the init method
         * has been called.
         */
        bool is_initialized();

        /**
         * Steps the world forward by one tick, with the number of milliseconds
         * which have elapsed since the previous frame.  Performs any
         * passive actions which need to happen.
         */
        void act(long delta_ms);

        /**
         * Returns true if the game is paused.
         */
        bool is_paused();

        /**
         * Sets "paused" to true.
         */
        void pause();

        /**
         * Sets "paused" to false.
         */
        void unpause();

        /**
         * Flips the boolean value of puased.
         */
        void toggle_pause();

        /**
         * Determines whether the visibility should be on or off.
         * For debuggin purposes.
         */
        bool visibility_on;

        /**
         * Updates the visible portions of the game (dumps parts of the
         * buffer into the canvas), and redraws visibility lines.
         * Gets called when the main character moves.
         */
        void refresh();

        /**
         * Sets tiles that are a part of the "bresenham_lines" to visible.
         */
        void draw_visibility_lines();

        /**
         * Sets the tiles that are in the bresenham_lines to non-visible
         * (visible = false).
         */
        void undo_visibility();

        /**
         * Rewrites the points that should be made visible, based on
         * the location of the character.
         */
        void recalculate_visibility_lines(int radius);

//-----------------------------------BUFFER PUBLIC METHODS----------------//
//src/controller/buffer_controller.cpp

        /**
         * Writes the tiles from the chunks surrounding the passed in
         * chunk to the buffer.  Also rewrites the enemies positions
         * in the character_index.
         */
        void update_buffer(IntPoint chunk);

        /**
         * Adds a tile to the buffer at the given coordinates, after
         * checking if the tile is in the buffer.
         */
        void add_tile_to_buffer(IntPoint chunk, IntPoint coords, Tile* tile);

        /**
         * Checks if a point in the form of (chunk, coords) is in the buffer.
         */
        bool point_in_buffer(IntPoint chunk, IntPoint coords);


        /**
         * The function that writes objects from the chunk (items,
         * plants, etc.) into the buffer to be rendered.
         */
        void show_chunk_objects();


        /**
         * Writes a list of items into the buffer at a given chunk
         * location.
         */
        void items_to_buffer(std::vector<Item*>* items, IntPoint chunk);

        /**
         * Writes a list of spawners into the buffer at a given chunk
         * location.
         */
        void spawners_to_buffer(std::vector<Spawner>* spawners, IntPoint chunk);

        /**
         * Writes the tiles in a den into the buffer.
         */
        void den_to_buffer(Den* den, IntPoint chunk, IntPoint coords);

        /**
         * Writes plants to the buffer.
         */
        void plants_to_buffer(std::vector<Plant>* plants, IntPoint chunk);


        /**
         * Writes the buildings in the current chunk to the buffer.
         */
        void buildings_to_buffer(std::vector<Building>* building, IntPoint chunk);

//-------------------------------CHARACTER PUBLIC METHODS----------------//
        Character main_char;

        /**
         * Updates the character index with the current chunk.
         */
        void update_character_index();

        /**
         * Sets everything in the character index to NULL.
         */
        void clear_character_index();

        /**
         * Adds a character to the character_index at the appropriate location.
         */
        void character_to_index(Character* chara);

        /**
         * Sets the characters location in the character_index to NULL.
         */
        void remove_index_char(Character* chara);

        /**
         * Iterates through every character in the game.  If the character
         * is targetting enem, then the target becomes NULL.
         */
        void remove_targets(Character* enem);

        /**
         * Returns a reference to the 1D vector containing all characters.
         */
        std::vector<Character*>& get_characters();


        /**
         * Returns true if there is an enemy in range of the character.
         * If so, sets the character's target to the enemy found.
         */
        bool find_target(Character* chara);

        /**
         * Gets the best target for a normal character.  Takes in the
         * character to find the target for and a list of possible
         * characters.
         */
        Character* normal_target(Character* chara, std::vector<Character*> characters);

        /**
         * Gets the best target for a passive character.  Takes in the
         * character to find the target for and a list of possible
         * characters.
         */
        Character* passive_target(Character* chara, std::vector<Character*> characters);

        /**
         * Returns a list of all of the characters in range of the
         * character passed into the function.
         */
        std::vector<Character*> characters_in_range(Character* chara);


        /**
         * Uses pathfinding to get to the point.  Note: this could be
         * a nightmare, as characters will currently only attempt to move
         * to points they can see, but the pathfinding may take them on
         * a very bizarre path involving the entire buffer.
         */
        int move_to_point(Character* chara, IntPoint coords, IntPoint chunk);

        /**
         * Causes the enemy to try to run away from its current target.
         */
        int run_away(Character* chara, IntPoint coords, IntPoint chunk);

        /**
         * Causes a character to wander listlessly.
         */
        void wander(Character* chara);

        /**
         * Checks if one character is next to another.
         */
        bool next_to_char(Character* chara, Character* target);

        /**
         * Kills a character.  DOES NOT DELETE THE POINTER.
         */
        void kill(Character* chara);

        /**
         * Removes an enemy from the game. DOES NOT DELETE THE POINTER.
         */
        void remove_enemy(Character* chara);

        /**
         * Checks if a character is in the buffer.
         */
        bool character_in_range(Character* chara);

        /**
         * Changes the depth of the character, if it is standing on the
         * appropriate stair tile.
         */
        void change_depth(int direction, Character* chara);

        /**
         * Causes the character to attempt to move.  If the main character
         * is adjacent to another character, it will attack it.
         */
        bool move_char(int col_change, int row_change, Character* chara);

        /**
         * Causes one character to attack another.
         */
        bool attack_char(Character* chara, Character* target);

        /**
         * Turns the character towards another character.
         */
        void turn_character(Character* chara, Character* target);

        /**
         * Turns the character away from another character.
         */
        void turn_away(Character* chara, Character* target);

        /**
         * Checks to see if a target is still within the character's line
         * of sight.
         */
        bool valid_target(Character* chara, Character* target);

        /**
         * If the character is standing above an item, it will pick it up.
         */
        void get_item(Character*);

        /**
         * Removes an item from the characters inventory and places it
         * in the chunk the character is standing in.
         */
        void drop_item(Item*, Character*);

        void harvest_plant(Plant* plant, Character* chara);

        /**
         * Checks if there is a character at a given location.  Returns
         * NULL if there is not.
         */
        Character* character_at_loc(IntPoint, IntPoint);

//-----------------------------CANVAS PUBLIC METHODS--------------------------//
//src/controller/canvas_controller.cpp
//
        /**
         * Checks to see if a set of coordinates (starting from the top
         * left of the canvas) are within the canvas.
         */
        bool is_vis(IntPoint coords);

        /**
         * Public accessor for the tile at canvas[row][col].
         */
        Tile* get_tile(int row, int col);

        /**
         * Public accessor for the tile at canvas[point.row][point.col].
         */
        Tile* get_tile(IntPoint point);

        /**
         * Sets the point in the canvas at row, col to Tile
         * @param row The row of the canvas to set the tile.
         * @param col The column of the canvas to set the tile.
         * @param tile The tile to set row, col to.
         */
        void set_tile(int row, int col, Tile* tile);

        /**
         * Sets the point in the canvas at IntPoint to tile.
         * @param point The point of the canvas to set the tile.
         * @param tile The tile to set the IntPoint of the canvas to.
         */
        void set_tile(IntPoint point, Tile* tile);

        /**
         * Converts coords in the form of (chunk, coords) to coords starting
         * with the top left of the canvas.
         */
        IntPoint get_canvas_coords(IntPoint chunk, IntPoint coords);

        /**
         * Gets a list of the characters that are currently visible.
         */
        std::vector<Character*> get_vis_characters();

        /**
         * Returns a const reference to the canvas (read-only).
         */
        const std::vector<std::vector<Tile*> >& get_canvas();

//-----------------------------CHUNK_MAP PUBLIC METHODS------------------//
//src/controller/chunkmap_controller.cpp

        /**
         * Updates the chunk map given a new chunk coordinates.
         */
        void update_chunk_map(IntPoint shift_dir);

        /**
         * Returns the chunk that the main character is currently in.
         */
        Chunk* get_current_chunk();

        /**
         * Returns a plant pointer at a given location (chunk, coordinates, and depth).
         */
        Plant* get_plant(IntPoint chunk, IntPoint coords, int depth);

//-------------------------------ANIMATION PUBLIC METHODS----------------//
//src/controller/animation_controller.cpp

        /**
         * Returns a reference to the vector of animation list.
         */
        std::vector<Animation>& get_animations();

        /**
         * Passes each animation the number of milliseconds which have
         * passed since the last time it ran, which then determines if
         * it should move to the next frame or not.
         */
        void tick_animations(long delta_ms);

        /**
         * Creates an explosion at the given location.
         */
        void create_explosion(int x, int y, int chunk_x, int chunk_y);


//-----------------------------SPAWNER PUBLIC METHODS--------------------//
//src/controller/spawn_controller.cpp

        /**
         * Ticks each spawner forward, flushing the queue of enemies it
         * may have accumulated.
         */
        void run_spawners();

        /**
         * Clears the queue of characters.
         */
        void clear_character_queue();

        /**
         * Returns the list of character points in the character_queue.
         */
        std::vector<Character*> flush_characters();

        /**
         * Takes care of everything needed to add a character to the game.
         */
        void add_character(Character* character, IntPoint chunk);

//-------------------------------DEBUG PUBLIC METHODS--------------------//
//src/controller/debug_controller.cpp
        /**
         * Spawns a new enemy.
         */
        void spawn_enemy(int x, int y, int chunk_x, int chunk_y, int number, int);

        /**
         * Debug function to teleport the character to a new place.
         */
        void teleport(int chunk_x, int chunk_y, int x, int y);

};

#endif
