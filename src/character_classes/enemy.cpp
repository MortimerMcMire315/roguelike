/**
 *  @file ENEMY.CPP
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

#include "enemy.h"

using namespace std;
Enemy::Enemy()
{
}

Enemy::Enemy(int _x, int _y, int _depth, EnemyType enemy) : Character(_x, _y, _depth)
{
    timer = 0;
    //determines if the character is good or evit, on a scale of 1-5 (5 is evil, 3 is passive)
    moral = enemy.moral;
    stats = enemy.stats;
    current_stats = stats;
    id = enemy.id;
    ai_id = enemy.ai_id;
    name = enemy.name;
    sight = enemy.sight;
    view = enemy.view;
    direction = 0;
    speed = enemy.speed;
    sprite = enemy.sprite;
    corpse = new Misc(IntPoint(x, y), enemy.corpse);
    natural_weapon = Weapon(IntPoint(y, x), enemy.natural_weapon);

    //generate the enemy's list of equipment and weapons
    vector<Equipment*> equip_list = generate_equipment(enemy.eq);
    for(int i = 0;i<equip_list.size();i++)
    {
        inventory.push_back(equip_list[i]);
    }
    Weapon* weapon = generate_weapon(vector<WeaponType>(enemy.wep));
    if(weapon != NULL)
    {
        inventory.push_back(weapon);
    }
}

Enemy::~Enemy()
{
    for(int i=0;i<inventory.size();i++)
    {
        delete inventory[i];
    }
    for(int i=0;i<equipment.size();i++)
    {
        delete equipment[i];
    }

    //delete corpse;
}


int Enemy::get_id()
{
    return id;
}

std::vector<Equipment*> Enemy::generate_equipment(std::vector<EquipType> equipment_list)
{
    std::vector<Equipment*> new_equipment;
    for(int i = 0; i<equipment_list.size();i++)
    {
        if(rand() % (5 + equipment_list[i].rarity) == 0)
        {
           new_equipment.push_back(new Equipment(IntPoint(y, x), equipment_list[i]));
        }
    }
    return new_equipment;
}

Weapon* Enemy::generate_weapon(std::vector<WeaponType> weapon_list)
{
    for(int i = 0; i<weapon_list.size();i++)
    {
        if(rand() % (5 + weapon_list[i].rarity) == 0)
        {
            return new Weapon(IntPoint(y, x), weapon_list[i]);
        }
    }
    return NULL;
}

