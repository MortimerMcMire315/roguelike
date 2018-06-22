/**
 *  @file LEVEL_MENU.CPP
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

#include "menu.h"
#include "game.h"
#include "defs.h"
#include "sstream"

LevelMenu::LevelMenu(int padding, Tile _border, Game* _game) : Menu(padding, _border)
{
    id = menu_id::LEVEL_MENU;
    next_screen = GAME_SCREEN;
    game = _game;
    options.push_back("Strength");
    options.push_back("Dexterity");
    options.push_back("Intelligence");
    options.push_back("Return");
    title = "Level Up";
    extra_lines.push_back("Select an attribute to put points in.");
    stringstream ss;
    ss << "Number of points: "<<game->main_char.get_new_levels();
    extra_lines.push_back(ss.str());
}

Menu* LevelMenu::make_selection()
{
    int stat;
    if(options[selection] == "Return")
    {
        return new MainMenu(padding, border, game);
    }
    if(game->main_char.get_new_levels() > 0)
    {
        if(options[selection] == "Strength")
        {
            stat = STRENGTH;
        }
        else if(options[selection] == "Dexterity")
        {
            stat = DEXTERITY;
        }
        else if(options[selection] == "Intelligence")
        {
            stat = INTELLIGENCE;
        }
        game->main_char.level_stat(stat);
    }
    return new LevelMenu(padding, border, game);
}




