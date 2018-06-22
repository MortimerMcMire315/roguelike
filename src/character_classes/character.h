/**
 *  @file CHARACTER.H
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

#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL/SDL.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <math_helper.h>

#include "defs.h"
#include "ASCII_Lib.h"
#include "color_def.h"
#include "int_point.h"
#include "item.h"
#include "utility.h"
#include "bresenham.h"
#include "message.h"

/**
 * A class which is used to construct all characters in game.
 * This is the class that acts as the base for all enemies, NPCs,
 * the main character, and anything else that may be considered an
 * enemy in the game.
 */

class Character
{
    protected:
        /**
         * A protected spectrum ranging from 0-5.
         * Morality acts as the basis for whether or not characters
         * will attack each other, i.e. characters with 0 will attack
         * characters with 5.
         * \todo Make this an enum.
         */
        int moral;

        /**
         * A vector representation of the stats of a character.
         * All of the stats of a character (health, armor, strength, etc.
         * will be held in this vector.
         */
        std::vector<int> stats;

        /**
         * A vector representing the current stats of a character.
         * Whenever one of the player's stats temporarily changes,
         * as in a lowering or raising of health, or lowering or raising
         * of another stat, it will be held in this vector.
         */
        std::vector<int> current_stats;

        /**
         * The character's coordinate in the x direction.
         */
        int x;

        /**
         * The character's coordinates in the y direction.
         */
        int y;

        /**
         * A vector representing the character's current inventory.
         */
        std::vector<Item*> inventory;

        /**
         * A vector representing the character's equipment.
         * This will be "statically" sized, and certain indices
         * represent certain item slots, e.g. head
         */
        std::vector<Item*> equipment;

        /**
         * An integer representing the maximum size of a characters inventory.
         * \todo Implement this so that character's cannot carry more than their
         * inventory.
         */
        int inventory_size;

        /**
         * The 'natural' weapon used to attack when no weapon is equipped.
         */
        Weapon natural_weapon;

        /**
         * How the character will appear in the game.
         */
        Tile sprite;

        /**
         * An item representing the body of the character.
         * An item that is completely separate from the inventory that is droppped
         * onto the map upon the death of the character.  Because it will always be moved
         * to the chunk object before destructing the character, it does not need to be
         * deleted by the character.
         */
        Item* corpse;

        /**
         * The depth the character is in the chunk.
         */
        int depth;

        /**
         * An IntPoint object of the character's coordinates in the chunk_map.
         */
        IntPoint chunk;

        /**
         * What the character is currently focused on.
         * For the main character, this will primarily be to display information about
         * the enemy or NPC that they are focused on.  For non-players characters (both
         * enemies and NPCs, this will act as a continuous representation of their focus.
         */
        Character* target;

        /**
         * The "master" of the character, which corresponds to a character who is in control
         * of this one. I should probably also add a list of "slaves" (yikes) so that the
         * relationship is two ways, and it would also allow for controlling of the different
         * characters.
         */
        Character* master;

        /**
         * The last known health of the "master."
         */
        int master_health;

        /**
         * Whether or not the character is conscious.
         */
        bool conscious;

        /**
         * The current timer of the character.
         */
        long timer;

        /**
         * How quickly the character moves.
         * Represents the threshold for how many milliseconds must pass before
         * the character can act.
         * @see timer
         */
        int speed;

        /**
         * Member variable to hold how far the enemy can see.
         */
        int sight;

        /**
         * Member variable to hold where the enemy is looking.
         * This is 0-100, where 0 (and 100) are directly right.
         */
        int direction;

        /**
         * Member variable to hold the enemies field of view.
         * This is a percentage of the field of view (1-100).
         */
        int view;

        /**
         * The id of the behavior tree that is responsible for controlling this
         * enemy.
         */
        int ai_id;

        /**
         * The number of levels that the character has which have points left to
         * be distributed.
         */
        int level_up;

        /**
         * A string of the name of the enemy.
         * This is a generic string, as in "kobold," or "rabbit."
         */
        std::string name;

        /**
         * Gets the experience required to get to the next level.
         * Goes up by the square of the level.
         */
        int exp_to_level();


        /**
         * The character increases by one level.
         */
        void gain_level();

    public:
        /**
         * The default constructor.
         */
        Character();

        /**
         * The destructor.
         */
        ~Character();

        /**
         * The copy constructor.
         */
        Character(const Character& chara);

        /**
         * The assignment operator.
         */
        Character& operator=(const Character& chara);

        /**
         * The constructor for the character class.
         * This is the constructor which is used to construct the enemy and
         * main character derived classes.
         * @param _stats The stats to give the character.
         * @param _sprite The sprite of the character.
         * @param _corpse The corpse to be crated from the MiscType.
         * @param _morality The morality to give the character.
         * @param _x The x value to give the character.
         * @param _y The y value to give to the character.
         * @param _chunk_x The x coordinate of the chunk to place the character.
         * @param _chunk_y The y coordinate of the chunk to place the character.
         * @param _depth The depth in the chunk to place the charcter.
         */
        Character(std::vector<int> _stats, int _x, int _y, Tile _sprite, MiscType _corpse, int _chunk_x, int _chunk_y, int _depth, int _morality, int _speed, int _ai_id, std::string _name, WeaponType wep);

        /**
         * The constructor for the character class.
         * This is the constructor which is used to construct the enemy and
         * main character derived classes.
         * @param _x The x value to give the character.
         * @param _y The y value to give to the character.
         * @param _chunk_x The x coordinate of the chunk to place the character.
         * @param _chunk_y The y coordinate of the chunk to place the character.
         * @param _depth The depth in the chunk to place the charcter.
         */
        Character(int _x, int _y, int _depth);


        /**
         * Run the actions that happen to the character every frame.  Returns whether
         * or not enough time had passed to complete the action.
         */
        bool act(long ms);

        /**
         * A check to see whether or not the character is alive.
         * @returns True if the character's current health is > 0, otherwise false.
         * @see current_stats, stats
         */
        bool is_alive() const;

        /**
         * Reduces the character's current health based on the
         * weapon used to attack, the part of the body being
         * hit, and the strength of the character hitting them.
         * @see current_stats
         * @see stats
         */
        void take_damage(Weapon* weapon, int part_hit, int strength);

        /**
         * Reduces a given character's health by a certain amount.
         * Damages another character by calling take_damage() on that
         * character.
         * @see take_damage(int damage)
         * @see current_stats
         * @see stats
         */
        void attack(Character* _chara);

        /**
         * Checks to see if a character dodged an attack.
         */
        bool did_dodge(float hit);

        /**
         * Public access function for the character's inventory.
         * @return The character's inventory.
         * @see inventory
         */
        std::vector<Item*>* get_inventory();

         /**
          * Public access function for the character's equipment.
          * @return The character's equipment.
          * @see equipment
          */
        std::vector<Item*>* get_equipment();

        /**
         * Returns the weapon currently equipped.  If no weapon is
         * equipped, it will return the natural weapon.
         */
        Weapon* get_weapon();

        /**
         * Public accessor for the sight.
         * @return The member variable sight.
         */
        int get_sight();

        /**
         * Gets all the coordinates that the enemy can currently see.
         * @return A list of coords the enemy can see.
         */
        std::vector<IntPoint> sight_tiles();

        /**
         * Add an item to the character's inventory.
         * @param new_item The item to place in the character's inventory.
         */
        void add_item(Item* new_item);

        /**
         * Remove an item from the character's inventory.
         * This function is NOT memory safe, and only remove's the pointer to
         * the item, it does not delete the item.  The item should always be asisgned
         * somewhere else (in the case of this function, it should be in the chunk). If
         * the item passed in is not in the inventory, nothing will happen.
         * @param item The item to remove from the inventory.
         */
        void drop_item(Item* item);

        /**
         * Remove an item from the character's inventory by index.
         * Behaves the same as add_item(Item* new_item)
         * @see add_item(Item* new_item)
         */
        void drop_item(int item);

        /**
         * Remove an item from the inventory and destroy the reference to it.
         * This function IS memory safe, and will remove the pointer to the item.
         * This means that there should no longer be ANY references to this object
         * upon calling this function.  If the item is not found in the inventory,
         * nothing will happen.
         * @param item The item to destroy.
         */
        void destroy_item(Item* item);

        /**
         * Sets the corpse to null so that the character doesn't still
         * have a pointer to it.
         */
        void nullify_corpse();

        /**
         * Moves an item from the inventory to the equipment.
         * Checks if it is possible to equip or wield the item.  If so, it checks the
         * equipment slot that item should go into and places it in that slot.  If an
         * item is already equipped, it will remove it from the equipment.  The armor
         * or attack is updated based on the item's stats.
         * @param item THe item which will be equipped.
         * @see equipment
         * @see remove_item(int item)
         * @see Item
         */
        void equip_item(Item* item);

        /**
         * Removes an item from the equipment by index.
         * If there is an item equipped, it will remove it.
         * @param item The index of the item to be removed.
         */
        void remove_item(int item);

        /**
         * Removes all equipment.
         */
        void remove_all();

        /**
         * Public accessor for the x coordinate.
         * @return Member variable x.
         * @see x
         */
        int get_x();

        /** Public accessor for the y coordinate.
         * @return Member variable y.
         * @see y
         */
        int get_y();

        /**
         * Public accessor or the coordinates.
         * @return IntPoint containing y, x.
         */
        IntPoint get_coords();

        /**
         * Public accessor for the chunk coordinates.
         * @return Member variable chunk.
         * @see chunk
         */
        IntPoint get_chunk();

        /**
         * Public accessor for the x component of the chunk.
         * @return Column component of chunk.
         * @see chunk
         */
        int get_chunk_x();


        /**
         * Public accessor for the y component of the chunk.
         * @return Row component of chunk.
         * @see chunk
         */
        int get_chunk_y();

        /**
         * Public accessor for the sprite.
         * @return Member variable sprite.
         * @see sprite
         */
        Tile get_char();

        /**
         * Public accessor for the corpse.
         * @return Member variable corpse.
         * @see corpse
         */
        Item* get_corpse();

        /**
         * Public accessor for the depth.
         * @return Member variable depth.
         * @see depth
         */
        int get_depth();

        /**
         * Public accessor for the target.
         * @return Member variable target.
         * @see target
         */
        Character* get_target();

        /**
         * Public accessor for the max health.
         * Max health corresponds to stats[HEALTH].
         * @return Member variable stats[HEALTH].
         * @see stats
         */
        int get_max_hp();

        /**
         * Public accessor for the current health.
         * Current health corresponds to current_stats[HEALTH].
         * @return Member variable current_stats[HEALTH].
         * @see current_stats
         */
        int get_cur_hp();

        /**
         * Public accessor for the moral member variable.
         * @return Member variable moral.
         * @see moral
         */
        int get_moral();

        /**
         * Public accessor for the armor.
         * Gets the hit resistance value of the armor.
         */
        int get_armor_hit(int body_part, int type);

        /**
         * Public accessor for the armor.
         * Gets the damage reduction of the armor.
         */
        float get_armor_dam(int body_part, int type);

        /**
         * Public setter for the x coordinate.
         * @param _x New x coordinate.
         * @see x
         */
        void set_x(int _x);

        /**
         * Public setter for the y coordinate.
         * @param _y New y coordinate.
         * @see y
         */
        void set_y(int _y);

        /**
         * Public setter for the chunk.
         * @param _chunk New set of chunk coordinates.
         * @see chunk
         */
        void set_chunk(IntPoint _chunk);

        /**
         * Public setter for the x component of chunk.
         * @param _chunk_x New x component of the chunk.
         * @see chunk
         */
        void set_chunk_x(int _chunk_x);

        /**
         * Public setter for the y component of chunk.
         * @param _chunk_y New y component of the chunk.
         * @see chunk
         */
        void set_chunk_y(int _chunk_y);

        /**
         * Public setter for the depth.
         * @param d New depth.
         * @see depth
         */
        void set_depth(int);

        /**
         * Public setter for the target.
         * @param _target New target.
         * @see target
         */
        void set_target(Character* _target);

        /**
         * Consume an item based on the stats of that item.
         * Checks to see if the item is consumable.  If so, alter the appropriate
         * stats based on the item's attributes.
         * @param item The item to consume.
         * @see Consumable
         */
        void consume_item(Item* item);

        /**
         * Public getter for accessing a current stat value.
         * @param stat The index value of the stat to access.
         * @return The current value of that stat from the array stats.
         * @see stats
         */
        int get_stat(int);

        /**
         * Public setter for setting a current stat value.
         * @param stat The index value of the stat to set.
         * @param amount The value the stat should be set to.
         * @see current_stats
         */
        void set_stat(int stat, int amount);

         /**
         * Public getter for accessing a stat value.
         * @param stat The index value of the stat to access.
         * @return The current value of that stat from the array current_stats.
         * @see current_stats
         */
       int get_current_stat(int);

        /**
         * Public setter for setting a stat value.
         * @param stat The index value of the stat to set.
         * @param amount The value the stat should be set to.
         * @see stats
         */
        void set_current_stat(int stat, int amount);


        /**
         * Sets conscious to true.
         */
        void regain_consciousness();

        /**
         * Sets conscious to false.
         */
        void pass_out();

        /**
         * Returns whether or not the character is conscious.
         */
        bool is_conscious();


        /**
         * Reduces the current endurance of the character by a factor.
         * The amount passed in indicates what factor the endurance
         * should be reduced by (e.g. walking reduces the endurance by 1,
         * which will be multiplied by something to reduce endurance).
         */
        void reduce_endurance(int factor);

        /**
         * Gains endurance based on the factor.
         */
        void gain_endurance(int factor);

        /**
         * Checks to see if a point is within the enemy's range of sight.
         * Looks at a point/chunk to see if it it's within the enemies
         * range of vision.
         * @parameter _coords The coordinates of the target.
         * @parameter _chunk The chunk of the target.
         * @return True if the coords/chunk can be seen.
         */
        bool in_sight_range(IntPoint _coords, IntPoint _chunk);

        /**
         * Checks to see if the enemy can see something.
         * Looks at a point/chunk to see if it it's within the enemies
         * slice of view.  Assumes that the enemy can't see everything,
         * as having the upper and lower bounds being the same will make
         * everything pretty wonky.
         * @parameter _coords The coordinates of the target.
         * @parameter _chunk The chunk of the target.
         * @return True if the coords/chunk can be seen.
         */
        bool in_sight(IntPoint _coords, IntPoint _chunk);

        /**
         * Turns the enemy to face towards the coords given.
         * @parameter coords The difference between the target coords and current coords.
         * @todo Implement a max turn amount.
         */
        void turn(IntPoint coords);

        /**
         * Turns the character by an amount.
         */
        void turn(int turn_amount);

        /**
         * Calculates the upper and lower angles (from 1-100) of the field of view.
         * @return An IntPoint containing the upper and lower bounds o the fov as angles.
         */
         IntPoint get_fov();

        /*
         * Public accessor for the ai id.
         */
        int get_ai_id();

        /**
         * Causes experience to increase as a proportion of the level
         * passed into the function.
         */
        void gain_experience(int level);

        /**
         * Accessor for the number of levels a character has gained
         * but not distributed points to.
         */
        int get_new_levels();

        /**
         * Levels a stat up, and decreases the number of new levels a
         * character has.
         */
        void level_stat(int stat);

        /**
         * Checks to see if the character has an item in its inventory
         * or equipment which belongs to the passed in category.
         */
        bool has_item_category(int category);

        /**
         * Public accessor for the name of the character.
         */
        std::string get_name();

        //------------------DERIVED STATS-------------------//
        /**
         * The characters ability to dodge.  This is derived from
         * the stat "Dexterity."
         */
        float dodge_stat();

        /**
         * The character's accuracy, representing the percentage
         * of how often a character hits something.
         */
        int accuracy_stat();


        /**
         * Accessor for the "master."
         */
        Character* get_master();

        /**
         * Setter for the "master."
         */
        void set_master(Character* new_m);

        /**
         * Accessor for the last know masters health.
         */
        int master_hp();

        /**
         * Updates the last known master's health.
         */
        void update_master_health();

        /**
         * Checks to see if the current master's health
         * is different than the last known master's health.
         */
        bool master_health_changed();

        /**
         * Kills the character.
         */
        void kill();

};

struct character_info {
    int x;
    int y;
    int depth;
    int morality;
    int speed;
    int ai_id;
    bool conscious;
    IntPoint chunk;
    std::vector<int> stats;
    Tile sprite;
    MiscType corpse;
    Character* target;
    vector<Item*> equipment;
};

#endif
