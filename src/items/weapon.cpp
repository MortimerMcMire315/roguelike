/**
 *  @file WEAPON.CPP
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

#include "item.h"
#include "character.h"
using namespace equipment;

Weapon::Weapon()
{}

Weapon::Weapon(IntPoint _coords, WeaponType wpn) : Item(wpn.rarity, wpn.weight, wpn.sprite, wpn.name, coords, wpn.description, wpn.category, wpn.use)
{
    damage = wpn.damage;
    range = wpn.range;
    type = wpn.type;
    size = wpn.size;
    can_equip = false;
    can_wield = true;
    can_consume = false;
}


void Weapon::perform_action()
{
    if(can_use)
    {
    }
}

int Weapon::get_type()
{
    return type;
}


int Weapon::get_damage()
{
    return damage;
}
