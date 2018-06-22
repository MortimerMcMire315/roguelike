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

#include "gui.h"
#include "animation.h"
#include "menu.h"

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
        drawStr(0, GAME_HEIGHT - 2, std::string("Use the arrow keys to move the cursor.").c_str(),
                ascii, screen, WHITE);
        drawStr(0, GAME_HEIGHT - 1, std::string("Press ENTER to spawn on the selected map tile.").c_str(),
                ascii, screen, WHITE);
    } else if (current_screen == GAME_SCREEN) {
        render_canvas();
        render_target();
        render_characters();
        render_main_char();
        render_animations();
        clear_area(IntPoint(0, UI_START), IntPoint(UI_HEIGHT, UI_WIDTH));
        render_interface();
        render_message();

    } else if(current_screen == DIRECTION_SCREEN)
    {
        drawStr(0, 0, std::string("Pick a direction to perform the action.").c_str(), ascii, screen, WHITE);
    } else if (current_screen == DEATH_SCREEN) {
        clear_screen();
        drawStr(GAME_WIDTH/2 - 12, GAME_HEIGHT/2, std::string("You suck, uninstall bro.").c_str(), ascii, screen, WHITE);
    } else if (current_screen == DEBUG_CONSOLE) {
        render_canvas();
        render_target();
        render_characters();
        render_main_char();
        render_debug();
    }
    if(game.is_paused()) {
        drawStr(GAME_WIDTH-20, 0, std::string("Paused").c_str(), ascii, screen, WHITE);
    }

    SDL_Flip(screen);
}

void GUI::clear_screen()
{
    for(int i =0;i<GAME_WIDTH;i++)
    {
        for(int j=0;j<GAME_HEIGHT;j++)
        {
            drawStr(i, j, std::string(" ").c_str(), ascii, screen, WHITE);
        }
    }
}

void GUI::render_canvas()
{
    clear_screen();
    TilePointerMatrix tm = game.get_canvas();
    for(size_t i = 0; i < tm.size(); i++) {
        for(size_t j = 0; j < tm[i].size(); j++) {
            if(game.visibility_on) {
                //If the tile is visible, render it fully.
                if(tm[i][j]->visible) {
                    drawChr(j, i, tm[i][j]->char_count, ascii, screen, tm[i][j]->color);

                //If the tile is not visible, but has been seen, render it in
                //grey.
                } else if(tm[i][j]->seen) {
                    drawChr(j, i, tm[i][j]->char_count, ascii, screen, VERY_DARK_GRAY);
                    //We probably shouldn't draw the chara layer on non-visible
                    //tiles.
                } else {
                    //Draw an empty tile
                    drawChr(j, i, 0, ascii, screen, 0);
                }
            } else {
                drawChr(j, i, tm[i][j]->char_count, ascii, screen, tm[i][j]->color);
            }
        }
    }
}

void GUI::render_characters() {
    Tile current_tile;
    IntPoint current_point;
    TilePointerMatrix tm = game.get_canvas();
    std::vector<Character*> tl = game.get_vis_characters();
    for(size_t i = 0; i < tl.size(); i++) {
        IntPoint temp_chunk = IntPoint(tl[i]->get_chunk_y(),tl[i]->get_chunk_x());
        IntPoint temp_coords = IntPoint(tl[i]->get_y(), tl[i]->get_x());
        current_tile = tl[i]->get_char();
        current_point = game.get_canvas_coords(temp_chunk, temp_coords);
        if(tm[current_point.row][current_point.col]->visible) {
            drawChr(current_point.col, current_point.row,
                    current_tile.char_count, ascii, screen, current_tile.color);
        }
    }
}

void GUI::render_main_char() {
    drawChr(GAME_WIDTH/2, GAME_HEIGHT/2, game.main_char.get_char().char_count, ascii, screen, game.main_char.get_char().color);
}

void GUI::render_interface() {
    Character* target = game.main_char.get_target();
    int height = 0;

    drawStr(UI_START, height, std::string("Main Character").c_str(), ascii, screen, WHITE);

    height = render_stats(&game.main_char, height + 1);
    height ++;

    drawStr(UI_START, height, std::string("Target").c_str(), ascii, screen, WHITE);

    //Render the current target
    if(target != NULL)
    {
        height = render_stats(target, height + 1);
    }
    else
    {
        drawStr(UI_START, height + 1, std::string("None").c_str(), ascii, screen, WHITE);
    }

    //Render that you have a level!
    if(game.main_char.get_new_levels() > 0)
    {
        drawStr(UI_START, SCREEN_HEIGHT - 1, std::string("Level up!").c_str(), ascii, screen, RED);
    }
}

int GUI::render_stats(Character* chara, int height)
{
    for(int i=0;i<NUM_STATS;i++)
    {
        stringstream ss;
        ss << STAT_NAMES[i] << ": " << chara->get_current_stat(i) << "/" << chara->get_stat(i);
        drawStr(UI_START, height, ss.str().c_str(), ascii, screen, WHITE);
        height++;
    }
    return height;
}


//WHEN DID THIS GET SO BIG?? REFACTOR THIS!
void GUI::render_menu(Menu* menu)
{

    //menus will always be rendered in the middle of the screen, fyi
    //clear the background in the specified height/width
    //width is automatically 20
    int height = menu->options.size() + menu->padding;
    int width = utility::get_max_width(menu->options) + menu->padding;
    int start_row = (GAME_HEIGHT - height) / 2;
    int start_col = (GAME_WIDTH - width) / 2;
    int end_row = (GAME_HEIGHT + height) / 2;
    int end_col = (GAME_WIDTH + width) / 2;

    int extra_row = start_row - menu->padding;
    int extra_end_row = start_row + menu->num_extra_lines() + menu->padding;
    int extra_width = utility::get_max_width(menu->get_extra_lines()) + menu->padding;
    int extra_col = (GAME_WIDTH - extra_width)/2;
    int extra_end_col = (GAME_WIDTH + extra_width)/2;

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
    starting_col = (GAME_WIDTH - menu->title.size()) / 2;

    //draw the title
    drawStr(starting_col, (GAME_HEIGHT/4), menu->title.c_str(),
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
        starting_col = (GAME_WIDTH - string_size) / 2;
        if(menu->selection == i) {
            color = WHITE;
        } else {
            color = DARK_GRAY;
        }

        drawStr(starting_col, start_row + menu->padding + i,
                option.c_str(), ascii, screen, color);
    }

}

void GUI::render_target()
{
    TilePointerMatrix tm = game.get_canvas();
    if(game.main_char.get_target() != NULL)
    {
        Character* chara = game.main_char.get_target();
        IntPoint temp_chunk = chara->get_chunk();

        std::vector<IntPoint> sight = chara->sight_tiles();
        for(int i=0;i<sight.size();i++)
        {
            IntPoint point = game.get_canvas_coords(temp_chunk, sight[i]);
            if(game.is_vis(point) && tm[point.row][point.col]->visible)
            {
                drawChr(point.col, point.row, tm[point.row][point.col]->char_count, ascii, screen, YELLOW);
            }
        }
    }
}

void GUI::render_debug()
{
    std::unordered_map<std::string, Tile>* tileset = &Tileset::instance()->get_tileset();
    for(int i=0;i<GAME_WIDTH;i++)
    {
        drawChr(i, GAME_HEIGHT-3, (*tileset)["BLOCK_WALL"].char_count, ascii, screen, BLACK);
        drawChr(i, GAME_HEIGHT-2, (*tileset)["BLOCK_WALL"].char_count, ascii, screen, BLACK);
    }
    drawChr(input.size(), GAME_HEIGHT-2, (*tileset)["BLOCK_WALL"].char_count, ascii, screen, WHITE);
    drawStr(0, GAME_HEIGHT-3, debug.get_message().c_str(), ascii, screen, WHITE);
    drawStr(0, GAME_HEIGHT-2, input.c_str(), ascii, screen, WHITE);
}

void GUI::render_animations()
{
    std::vector<Animation> anims = game.get_animations();
    TilePointerMatrix tm = game.get_canvas();
    for(int i=0;i<anims.size();i++)
    {
        Frame f = anims[i].get_frame();
        IntPoint coords = IntPoint(anims[i].get_y(), anims[i].get_x());
        IntPoint chunk = IntPoint(anims[i].get_chunk_y(), anims[i].get_chunk_x());
        IntPoint vis = game.get_canvas_coords(chunk, coords);
        for(int j=0;j<f.get_actors().size();j++)
        {
            Actor a = f.actor(j);
            coords = vis + IntPoint(a.get_y(), a.get_x());
            if(game.is_vis(coords) && tm[coords.row][coords.col]->visible)
            {
                drawChr(coords.col, coords.row, a.get_char(), ascii, screen, a.get_color());
            }
        }
    }
}

void GUI::clear_area(IntPoint start, IntPoint size)
{
    clearArea(start.col, start.row, size.row, size.col, screen, BLACK);
}

void GUI::render_message()
{
    drawStr(0, MESSAGE_HEIGHT, MessageBoard::instance().get_current_message().c_str(), ascii, screen, WHITE);
}
