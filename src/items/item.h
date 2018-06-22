/**
 *  @file ITEM.H
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

#ifndef ITEM_H
#define ITEM_H
#include "defs.h"
#include "int_point.h"

#include <string>

/**
 * String containing the different body parts.
 * \todo Move this to defs.h
 */
static std::string BODY_PARTS[] = {"Head", "Torso", "Legs", "Feet", "Arms", "Hands", "Wielding"};


/**
 * The base class for all item types.
 * This class acts as a base for all items, including equipment,
 * weapons, consumables, and other item types.
 */
class Item
{
    protected:
        /**
         * What the item is called.
         */
        std::string name;

        /**
         * A brief description of the item.
         */
        std::string description;

        /**
         * How much the item weighs.
         */
        int weight;

        /**
         * Where the item currently is.
         * This is only used as a way to graphically represent
         * the item when it is on the ground.  When an item is in
         * a character's inventory, the coordinates are wherever it
         * was picked up, and are only updated when the item is dropped
         * again.
         */
        IntPoint coords;

        /**
         * A graphical representation of the item.
         */
        Tile sprite;

        /**
         * How likely it is that the item will appear.
         */
        int rarity;

        /**
         * The size of the item.
         * Will factor into what can use it and how much
         * can be carried at a time (using a unit system
         * that I'm going to conveniently make up whenever
         * I feel like it).
         */
        int size;

        /**
         * The category that the item belongs to.  Eventually,
         * perhaps, this will be multiple categories.  For now,
         * it will just be the one.
         */
        int category;

    public:
        /**
         * Whether or not the item can be equipped by any character.
         */
        bool can_equip;

        /**
         * Whether the item has a usable component.
         * If this is yes, that means that the item has some active component
         * other than its base use, e.g. a sword that has a spell attached to it.
         */
        bool can_use;

        /**
         * Whether or not an item can be wielded as a weapon by a character.
         */
        bool can_wield;

        /**
         * Whether or not a character can consume an item.
         */
        bool can_consume;

        /**
         * The blank constructor.
         */
        Item();

        /**
         * The basic constructor for an item.
         * @param _coords The coordinates to place the item at.
         */
        Item(IntPoint _coords);

        /**
         * A constructor for an item.
         * @param _rarity Sets the class member rarity.
         * @param _weight Sets the weight of the item.
         * @param _sprite Sets the sprite of the item..
         * @param _name Sets the name of the item.
         * @param _coords Set the coordinates of the item.
         */
        Item(int _rarity, int _weight, Tile _sprite, std::string _name, IntPoint _coords, std::string _description, int _category, bool _use);

        /**
         * Public accessor for the member variable weight.
         */
        int get_weight();

        /**
         * Public accessor for the member variable sprite.
         */
        Tile* get_sprite();

        /**
         * Public accessor for the coordinates.
         */
        IntPoint get_coords();

        /**
         * Public setter for the coordinates.
         * @param _c The new coords to be set.
         */
        void set_coords(IntPoint _c);

        /**
         * Public accessor for the name.
         */
        std::string get_name();

        /**
         * Public accessor for the rarity.
         */
        int get_rarity();

        /**
         * Public accessor for the description.
         */
        std::string get_description();

        /**
         * Public accessor for the size.
         */
        int get_size();

        /**
         * Public accessor for the category.
         */
        int get_category();

        /**
         * Returns whether or not the item belongs
         * to a certain category.
         */
        bool has_category(int cat);

};

/**
 * The class responsible for creating equipiment.
 * Equipment refers to anything wearable on the body.
 * @see EquipType
 */
class Equipment : public Item
{
    protected:
        /**
         * Which body part the equipment will be worn on.
         * This corresponds to the index in the Characters equipment
         * @see Character::equipment
         * @see EquipType::body_part
         */
        int body_part;

        /**
         * Determines which type of armor it is.
         * @see EquipType::type
         * @see WeaponType::type
         */
        int type;

        /**
         * Determines hit bonus gained from the equipment.
         */
        std::vector<int> to_hit;

        /**
         * Determines the damage bonus gained from the equipment.
         */
        std::vector<float> to_dam;

        /**
         * The material the equipment is made out of.
         */
        int material;
    public:
        /**
         * Blank constructor.
         */
        Equipment();


        /**
         * Constructor for the equipment.
         * This is the normally used constructor, relying on the defined EquipTypes and
         * taking a set of coordinates.
         * @param _coords Sets the coordinates of the equipment.
         * @param eqp THe template for the equipment.
         */
        Equipment(IntPoint _coords, EquipType eqp);

        /**
         * For performaing an action other than increasing the armor class.
         * @see Item::perform_action()
         */
        void perform_action();

        /**
         * Public accessor for body_part.
         */
        int get_body_part();

        /**
         * Public accessor for to_hit.
         */
        int get_hit(int type);

        /**
         * Public accessor for to_dam.
         */
        float get_dam(int type);

        /**
         * The material the object is made out of, defined in defs.cpp.
         */
        int get_material();
};

/**
 * Class for creating weapons.
 * Weapons refer to anything that can be wielded to alter a character's attack.
 * @see WeaponType
 */
class Weapon : public Item
{
    protected:
        //weapons are always wielded in slot 6 of the body part diagram

        /**
         * The value the character's attack will become.
         * A character's attack will be replaced by (not added to) the damage
         * done by a weapon.
         */
        int damage;

        /**
         * How many tiles the weapon can reach.
         */
        int range;

        /**
         * The possible types that the weapon can be.
         * The possible types are as follows:
         * 1 = piercing, 2 = slashing,  and 3 = bludgeoning.
         * @see WeaponType::type
         */
        int type;


    public:
        /**
         * Blank constructor.
         */
        Weapon();

        /**
         * Constructor for the weapon.
         * This is the normally used constructor, relying on the defined WeaponTypes and
         * taking a set of coordinates.
         * @param _coords Sets the coordinates of the weapon.
         * @param wpn The template for the weapon.
         */
        Weapon(IntPoint _coords, WeaponType wpn);

        /**
         * For performaing an action other than changing the attack of the character.
         * @see Item::perform_action()
         */
        void perform_action();

        /**
         * Public accessor for the variable type.
         */
        int get_type();

        /**
         * Public accessor for the variable to_dam.
         */
        int get_damage();
};

/**
 * Class for creating consumables.
 * Consumables refer to anything that can be consumed to alter a stat in some way.
 * @see ConsumableType
 */
class Consumable : public Item
{
    protected:
        /**
         * The stat that will be modified by the consumable.
         * This corresponds an index in the current_stat or stat vector in
         * the character class.
         * @see Character::current_stats
         * @see Character::stats
         */
        int stat_modified;

        /**
         * The magnitude that the stat should be modified by.
         */
        int amount_modified;

        /**
         * How the consumable will behave.
         * Corresponds to the enum ConsumableTypes, which defines
         * the behavior of consumable, e.g. restore health vs. fortifying health.
         */
        int type;
    public:
        /**
         * Blank constructor.
         */
        Consumable();

        /**
         * Constructor for the consumable.
         * This is the normally used constructor, relying on the defined ConsumableType and
         * taking a set of coordinates.
         * @param _coords Sets the coordinates of the consumable.
         * @param cons The template for th consumable.
         */
        Consumable(IntPoint _coords, ConsumableType cons);

        /**
         * For performaing an action other than changing a stat of the character.
         * @see Item::perform_action()
         */
        void perform_action();

        /**
         * Public accessor for the variable stat_modified.
         */
        int get_stat();

        /**
         * Public accessor for the variable amount_modified.
         */
        int get_amount();

        /**
         * Public accessor for the variable type.
         */
        int get_type();
};

/**
 * Class for creating misc items.
 * Misc items are items that don't fall into any other categor and
 * generally don't do anything.
 * @see MiscType
 */
class Misc : public Item
{
    public:
        /**
         * Blank constructor.
         */
        Misc();

        /**
         * Constructor for the misc item.
         * This is the normally used constructor, relying on the defined MiscType and
         * taking a set of coordinates.
         * @param _coords Sets the coordinates of the misc item.
         * @param misc The template for the misc item.
         */
        Misc(IntPoint _coords, MiscType misc);

        /**
         * For performaing an action.
         * @see Item::perform_action()
         */
        void perform_action();
};

#endif
