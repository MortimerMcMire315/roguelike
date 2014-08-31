/**
 *  GUI.H
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

#ifndef _GUI_H
#define _GUI_H

#define FONTDIR DATADIR "/font/"

#include <SDL/SDL.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <chunk.h>
#include <virtual_event.h>
#include <game.h>
#include <defs.h>
#include <world_map.h>
#include <world_map_gui.h>
#include <color_def.h>
#include <menu.h>
#include <game_states.h>
#include <stdio.h>
#include <fstream>
#include <debug.h>

namespace pt = boost::posix_time; 
class GUI : public VirtualEvent {


    typedef std::vector<std::vector<Tile*> > TilePointerMatrix;
    typedef std::vector<std::vector<Tile> > TileMatrix;
    private:
        void load_font(string);
        
        static const long STD_MS_PER_FRAME = 70;

        pt::ptime game_clock;

        bool running;
        std::vector<std::string> messages;
        std::string input;
        
        DebugConsole debug;
        KeyState keyset;
        Screen current_screen;
        Screen last_screen;
        WorldMapGUI world_map_gui;
        Menu* menu;
        Game game;

        SDL_Event event;
        SDL_Surface* screen;
        SDL_Surface* asciiBase;
        SDL_Surface* ascii;

        int handle_framerate();
        void clear_screen();
        void render_canvas();
        void render_enemies();
        void render_character();
        void render_interface();
        void render_menu(Menu* menu);
        void render_debug();
        void render_target();
        void clear_area(IntPoint start, IntPoint size);
        int render_stats(Character* chara, int height);
    public:
        GUI();
        ~GUI();
        int OnExecute();
        void OnRender();
        void OnLoop();
        void OnCleanup();
        bool OnInit();

        void OnEvent(SDL_Event* Event);
        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        void OnExit();

        void perform_action_press(SDLKey);
        void perform_action_cont();
        
        /**
         * The function for handling keyboard input for typing text.
         * This function handles text input, as opposed to the player
         * hitting keys to play the game.
         * @param unicode The key pressed.
         */
        void add_key_input(SDLKey key, Uint16 unicode); 
};

#endif
