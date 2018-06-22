/**
 *  @file MISC.CPP
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

Misc::Misc()
{}

Misc::Misc(IntPoint _coords, MiscType misc) : Item(misc.rarity, misc.weight, misc.sprite, misc.name, coords, misc.description, misc.category, misc.use)
{
    can_equip = false;
    can_wield = false;
    can_consume = false;
}


void Misc::perform_action()
{
    if(can_use)
    {
    }
}
