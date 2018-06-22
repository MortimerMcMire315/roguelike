/**
 *  @file EQUIPMENT.CPP
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

Equipment::Equipment()
{}

Equipment::Equipment(IntPoint _coords, EquipType eqp) : Item(eqp.rarity, eqp.weight, eqp.sprite, eqp.name, coords, eqp.description, eqp.category, eqp.use)
{
    rarity = eqp.rarity;
    weight = eqp.weight;
    sprite = eqp.sprite;
    description = eqp.description;
    name = eqp.name;
    body_part = eqp.body_part;
    type = eqp.type;
    size = eqp.size;
    to_hit.assign(&eqp.hit[0], &eqp.hit[0] + 3);
    to_dam.assign(eqp.dam, eqp.dam + sizeof(eqp.dam)/sizeof(eqp.dam[0]));
    material = eqp.material;
    can_use = eqp.use;
    can_equip = true;
    can_wield = false;
    can_consume = false;
}


void Equipment::perform_action()
{
    if(can_use)
    {
    }
}

int Equipment::get_body_part()
{
    return body_part;
}

int Equipment::get_hit(int type)
{
    return to_hit[type];
}

float Equipment::get_dam(int type)
{
    return to_dam[type];
}

int Equipment::get_material()
{
    return material;
}
