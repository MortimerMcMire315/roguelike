/**
 *  GUI_LOOP.CPP
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

#include "gui.h"


void GUI::OnLoop() {
    perform_action_cont();
    if(game.is_initialized() && game.is_paused() == false && current_screen == GAME_SCREEN) {
        game.act(STD_MS_PER_FRAME);
        for(int i=0;i<trees.size();i++)
        {
            trees[i].run_actors(STD_MS_PER_FRAME);
        }
        game.refresh();
        game.run_spawners();
        add_characters(game.flush_characters());
        game.clear_character_queue();
        if(!game.main_char.is_alive())
        {
           current_screen = DEATH_SCREEN;
        }
    }
    else if(game.is_initialized() && game.is_paused())
    {
        //this shouldn't effect anything else because the game is paused,
        //and it'll make navigating the menu less of a pain.
        SDL_Delay(100);
    }
}
