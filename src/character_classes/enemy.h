/**
 *  @file ENEMY.H
 *  @Author Michael Yoder
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

#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <vector>

#include "character.h"
#include "defs.h"
#include "constants.h"
#include "utility.h"
#include "constants.h"
#include "math_helper.h"
#include "bresenham.h"

using namespace enemies;


/**
 * The enemy class. The primary antagonists of the game.
 * A class which will randomly spawn and attempt to kill or
 * assist the main character.  Right now it's a fairly simple class,
 * but it should become increasingly complex as we go.
 */
class Enemy : public Character
{
    protected:
        /**
         * The id of the enemy.
         * Each enemy type has an id, starting at 0, e.g. kobolds id=0.
         * This is to allow accessing things by index based on the id of the
         * enemy.
         */
        int id;


        /**
         * Generates the equipment from a list of EquipType.
         * Loops through a list of possible equipment that an enemy can have,
         * and the returned list goes into the inventory.
         * \todo Make the enemy equip the items.
         * @param equipment_list A list of possible equipment that the enemy can have.
         * @return A list of equipment that the enemy does have.
         */
        std::vector<Equipment*> generate_equipment(std::vector<EquipType> equipment_list);

        /**
         * Generates the weapon from a list of WeaponType.
         * Loops through a list of possible weapons that an enemy can have,
         * and the returned weapon goes into the inventory.
         * \todo Make the enemy equip the weapon.
         * \todo Posbbily make this multiple weapons and have it pick the best one.
         * @param weapon_list A list of possible weapons that the enemy can have.
         * @return The weapon that the enemy has.
         */
        Weapon* generate_weapon(std::vector<WeaponType>);


    public:
        /**
         * The default constructor.
         */
        Enemy();

        /**
         * Constructor without the chunk info.
         */
        Enemy(int _x, int _y, int _depth, EnemyType enemy);

        /**
         * The destructoid!
         */
        ~Enemy();

        /**
         * Public accessor for the id.
         * @return The member id.
         * @see id
         */
        int get_id();

};

#endif
