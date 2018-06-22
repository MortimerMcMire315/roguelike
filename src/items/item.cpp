/**
 *  @file ITEM.CPP
 *  @author Michael Yoder
 *
 * @section LICENSE
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

Item::Item()
{
}

Item::Item(IntPoint _coords)
{
    coords = _coords;
}

Item::Item(int _rarity, int _weight, Tile _sprite, std::string _name, IntPoint _coords, std::string _description, int _category, bool _use)
{
    rarity = _rarity;
    weight = _weight;
    sprite = _sprite;
    name = _name;
    coords = _coords;
    category = _category;
    description = _description;
    can_use = _use;
}

int Item::get_weight()
{
    return weight;
}

Tile* Item::get_sprite()
{
    return &sprite;
}

IntPoint Item::get_coords()
{
    return coords;
}

std::string Item::get_name()
{
    return name;
}

void Item::set_coords(IntPoint _c)
{
    coords = _c;
}

int Item::get_rarity()
{
    return rarity;
}

int Item::get_size()
{
    return size;
}

std::string Item::get_description()
{
    return description;
}

int Item::get_category()
{
    return category;
}

bool Item::has_category(int cat)
{
    return category == cat;
}
