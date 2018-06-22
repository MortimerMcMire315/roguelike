/**
 *  GUI_INIT.CPP
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

namespace pt = boost::posix_time;
namespace gregorian = boost::gregorian;

bool GUI::OnInit() {
    string font_pref = "default";
    std::ifstream pref_file;
    pref_file.open(DATADIR "/settings.conf");
    if(pref_file.good())
    {
        string pref;
        string value;
        while(getline(pref_file, pref))
        {
            value = utility::parse_settings(pref);
            if(pref == "font")
            {
                font_pref = value;
            }
        }
        pref_file.close();
    }
    else
    {
        cout<<"No preference file found. Please settings.conf in /data.\
        Don't worry if you don't know what to put it in, we have defaults :)"<<endl;
        return false;
    }

    //initialize stuff
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO );

    //Set up screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH * 8, SCREEN_HEIGHT * 12, 32, SDL_SWSURFACE | SDL_RESIZABLE);

    //Load ascii characters
    load_font(font_pref);

    if(asciiBase == 0)
    {
        cout<<"No font file found.  Please place default.bmp in the data directory \
        or edit settings.conf accordingly."<<endl;
        return false;
    }

    SDL_EnableUNICODE(SDL_ENABLE);

    game_clock = pt::microsec_clock::local_time();

    return true;
}
