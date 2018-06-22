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

#include "gui.h"
#include "menu.h"

GUI::GUI() {
    world_map_gui = WorldMapGUI();
    game = Game();
    menu = new StartMenu(1, Tileset::get("BLOCK_WALL"), game, world_map_gui);
    current_screen = MENU_SCREEN;
    screen = NULL;
    asciiBase = NULL;
    ascii = NULL;
    keyset = GAME;
    running = true;
    debug = DebugConsole(&game);
    trees.push_back(ai::GENERIC_AGGRESSIVE(&game));
    trees.push_back(ai::GENERIC_PASSIVE(&game));
    trees.push_back(ai::FOLLOW(&game));
    trees.push_back(ai::NPC(&game));
}

int GUI::OnExecute() {

    if(OnInit() == false) {
        return -1;
    }

    SDL_Event e;

    while(running) {
        game_clock = pt::microsec_clock::local_time();
        while(SDL_PollEvent(&e)) {
            OnEvent(&e);
        }

        OnLoop();
        OnRender();

        SDL_Delay(handle_framerate());

    }

    OnCleanup();


    return 0;
}

//TODO IN CASE THE USER'S COMPUTER IS SLOW, game.act should receive the actual
//frame time instead of the intended frame time. Then it will be true frame
//slowing. As of right now (1/6/2014) this is very unlikely to be an issue, as
//my 10 year old laptop can handle the game twice as fast. However, we don't
//know what it will be like in the future.
int GUI::handle_framerate() {
    pt::ptime clock2 = pt::microsec_clock::local_time();
    long this_frame_time = (clock2 - game_clock).total_milliseconds();
    //cout<<STD_MS_PER_FRAME - this_frame_time<<endl;
    if(this_frame_time < STD_MS_PER_FRAME) {
        return STD_MS_PER_FRAME - this_frame_time;
    } else {
        //No matter what, we want to delay at least a little bit.
        return 10;
    }
}

GUI::~GUI()
{
    delete menu;
}

void GUI::load_font(string font)
{
    stringstream ss;
    ss << FONTDIR << font << string(".bmp").c_str();
    asciiBase = SDL_LoadBMP(ss.str().c_str());
    //Create an optimized image
    ascii = SDL_DisplayFormat( asciiBase );
    //Free the old image
    SDL_FreeSurface( asciiBase );
    //Map the color key
    Uint32 colorkey = SDL_MapRGB( ascii->format, 0xFF, 0, 0xFF );

    //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
    SDL_SetColorKey( ascii, SDL_SRCCOLORKEY, colorkey );
    SDL_SetColorKey( ascii, SDL_SRCCOLORKEY, colorkey );
}

void GUI::add_characters(std::vector<Character*> characters)
{
    for(int i=0;i<characters.size();i++)
    {
        for(int j=0;j<trees.size();j++)
        {
            if(characters[i]->get_ai_id() == trees[j].get_id())
            {
                BActor temp = BActor(characters[i]);
                trees[j].add_actor(temp);
            }
        }
    }
}
