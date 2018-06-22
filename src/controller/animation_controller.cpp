/**
 *  GAME.CPP
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

#include "game.h"

std::vector<Animation>& Game::get_animations()
{
    return anim_queue;
}

void Game::tick_animations(long delta_ms)
{
    for(int i=0;i<anim_queue.size();i++)
    {
        anim_queue[i].step(delta_ms);
        if(anim_queue[i].get_done())
        {
            anim_queue.erase(anim_queue.begin() + i);
        }
    }
}

//Animation Presets
void Game::create_explosion(int x, int y, int chunk_x, int chunk_y)
{
    anim_queue.push_back(construct_explosion(x, y, chunk_x, chunk_y, 5, YELLOW));
}
