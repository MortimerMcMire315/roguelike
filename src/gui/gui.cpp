/**
 *  GUI.CPP
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

#include <gui.h>

GUI::GUI() : world_map_gui(100, 50),
             main_menu(100, 50),
             game(100, 50) {

    current_screen = MENU_SCREEN;
    screen = NULL;
    asciiBase = NULL;
    ascii = NULL;

    running = true;
}

int GUI::OnExecute() {
    if(OnInit() == false) {
        return -1;
    }

    SDL_Event e;

    while(running) {
        while(SDL_PollEvent(&e)) {
            OnEvent(&e);
        }

        OnLoop();
        OnRender();
    }

    OnCleanup();

    return 0;
}
