/**
 *  GUI_RENDER.CPP
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


void GUI::OnRender() {
    if(current_screen == MENU_SCREEN) {
        render_menu(menu);
    } else if(current_screen == MAP_SCREEN) {
        std::vector<std::vector<MapTile> > map_canvas = world_map_gui.get_canvas();
        for(size_t i = 0; i < map_canvas.size(); i++) {
            for(size_t j = 0; j < map_canvas[i].size(); j++) {
                drawChr(j, i, map_canvas[i][j].char_count, ascii, screen, map_canvas[i][j].color);
            }
        }
        drawStr(0, 48, std::string("Use the arrow keys to move the cursor.").c_str(),
                ascii, screen, WHITE);
        drawStr(0, 49, std::string("Press ENTER to spawn on the selected map tile.").c_str(),
                ascii, screen, WHITE);
    } else if (current_screen == GAME_SCREEN) {
        if(!game.is_initialized()) {
            game.init(world_map_gui.get_map(), world_map_gui.get_selected_chunk());
        }
        render_canvas();
        render_enemies();
        render_character();
        render_interface();
           
    } else if (current_screen == DEATH_SCREEN) 
    {
        clear_screen();
        drawStr(38, 25, std::string("You suck, uninstall bro.").c_str(), ascii, screen, WHITE);
    } 
    if(game.is_paused())
    {
        drawStr(80, 0, std::string("Paused").c_str(), ascii, screen, WHITE);
    }
    SDL_Flip(screen);
}

void GUI::clear_screen()
{
    for(int i =0;i<STARTING_WIDTH;i++)
    {
        for(int j=0;j<STARTING_HEIGHT;j++)
        {
            drawStr(i, j, std::string(" ").c_str(), ascii, screen, WHITE);
        }
    }
}

void GUI::render_canvas()
{
    TilePointerMatrix tm = game.get_canvas();
    for(size_t i = 0; i < tm.size(); i++) {
        for(size_t j = 0; j < tm[i].size(); j++) {
            //If the tile is visible, render it fully.
            if(tm[i][j]->visible) {
                drawChr(j, i, tm[i][j]->char_count, ascii, screen, tm[i][j]->color);
            //If the tile is not visible, but has been seen, render it in
            //grey.
            } else if(tm[i][j]->seen) {
                drawChr(j, i, tm[i][j]->char_count, ascii, screen, VERY_DARK_GRAY);
                //We probably shouldn't draw the enemy layer on non-visible
                //tiles.
            } else {
                //Draw an empty tile
                drawChr(j, i, 0, ascii, screen, 0);
            }
        }
    }
}

void GUI::render_enemies()
{
    Tile current_tile;
    IntPoint current_point;
    TilePointerMatrix tm = game.get_canvas();
    std::vector<Enemy*> tl = game.get_vis_enemies();
    for(size_t i = 0; i < tl.size(); i++) {
        IntPoint temp_chunk = IntPoint(tl[i]->get_chunk_y(),tl[i]->get_chunk_x());
        IntPoint temp_coords = IntPoint(tl[i]->get_y(), tl[i]->get_x());
        current_tile = tl[i]->get_char();
        current_point = game.get_vis_coords(temp_chunk, temp_coords);
        if(tm[current_point.row][current_point.col]->visible) {
            drawChr(current_point.col, current_point.row,
                    current_tile.char_count, ascii, screen, current_tile.color);
        }
    }
}

void GUI::render_character()
{

    drawChr(STARTING_WIDTH/2, STARTING_HEIGHT/2, game.main_char.get_char().char_count, ascii, screen, game.main_char.get_char().color);
}

void GUI::render_interface()
{
    Character* target = game.main_char.get_target();
    if(target != NULL)
    {
        stringstream ss;
        ss << "Target health: " << target->get_cur_hp() << "/" << target->get_max_hp();
        drawStr(0, 0, ss.str().c_str(), ascii, screen, WHITE);
    }
    else
    {
        drawStr(0, 0, std::string("No target.").c_str(), ascii, screen, WHITE);
    }
    
    stringstream ss;
    ss << "Health : " << game.main_char.get_cur_hp() << "/" << game.main_char.get_max_hp();
    stringstream ac;
    ac << "Armor: " <<game.main_char.get_armor();
    drawStr(STARTING_WIDTH/3, 0, ss.str().c_str(), ascii, screen, WHITE);
    drawStr(STARTING_WIDTH/2, 0, ac.str().c_str(), ascii, screen, WHITE);
}


void GUI::render_menu(Menu* menu)
{
    
    //menu's will always be rendered in the middle of the screen, fyi
    //clear the background in the specified height/width
    //width is automatically 20
    int height = menu->options.size() + menu->padding;
    int width = get_max_width(menu->options) + menu->padding;
    int start_row = (STARTING_HEIGHT - height) / 2;
    int start_col = (STARTING_WIDTH - width) / 2;
    int end_row = (STARTING_HEIGHT + height) / 2;
    int end_col = (STARTING_WIDTH + width) / 2;
 
    int extra_row = start_row - menu->padding;
    int extra_end_row = start_row + menu->num_extra_lines() + menu->padding;
    int extra_width = get_max_width(menu->get_extra_lines()) + menu->padding; 
    int extra_col = (STARTING_WIDTH - extra_width)/2;
    int extra_end_col = (STARTING_WIDTH + extra_width)/2;
    
    if(menu->num_extra_lines() != 0)
    {
        start_row = start_row + menu->num_extra_lines() + 2;
        end_row = start_row + menu->num_extra_lines() + 2;
        for(int row = extra_row; row <= extra_end_row; row++)
        {
            for(int col = extra_col; col <= extra_end_col; col++)
            {
                drawChr(col, row, menu->border.char_count, ascii, screen, BLACK);
            }
        }
    }

    //draw a box around the menu
    for(int row = start_row; row <= end_row; row++)
    {
        for(int col = start_col; col <= end_col; col++)
        {
            drawChr(col, row, menu->border.char_count, ascii, screen, BLACK);
        }
    }

    //draw a box around the extra lines 

    int starting_col;
    starting_col = (STARTING_WIDTH - menu->title.size()) / 2;
    
    //draw the title
    drawStr(starting_col, (STARTING_HEIGHT/4), menu->title.c_str(),
            ascii, screen, RED);
    
    
    int color, string_size;

    //draw the extra lines
    for(int i = 0; i< menu->num_extra_lines(); i++)
    {
        string option = menu->get_extra_lines()[i];
        if(option.size() % 2 != 0)
        {
            option = " " + option;
        }

        color = RED;

        drawStr(extra_col, extra_row + menu->padding + i, option.c_str(), ascii, screen, color);
    }

    //Render selections
    for(int i = 0; i < menu->options.size(); i++) {
        string option = menu->options[i];
        if(option.size() % 2 != 0)
        {
            option = " " + option;
        }
        
        string_size = option.size();
        starting_col = (STARTING_WIDTH - string_size) / 2;
        
        if(menu->selection == i) {
            color = WHITE;
        } else {
            color = DARK_GRAY;
        }

        drawStr(starting_col, start_row + menu->padding + i,
                option.c_str(), ascii, screen, color);
    }

}

