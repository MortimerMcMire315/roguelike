/**
 *  BEHAVIOR_ACTOR.CPP
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

#include "behavior_actor.h"

BActor::BActor()
{
}

BActor::BActor(Character* _chara)
{
    chara = _chara;
}

Character* BActor::get_character()
{
    return chara;
}

bool BActor::should_tick(long delta_ms)
{
   if(chara->act(delta_ms))
   {
       return true;
   }
}
