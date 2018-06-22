/**
 *  @file DEBUG.CPP
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

#include "debug.h"

DebugConsole::DebugConsole()
{
    //do nothing
}

DebugConsole::DebugConsole(Game* _game)
{
    buffer_place = 0;
    current_place = 0;
    buffer_size = 20;
    game = _game;
    debug_message = "Input your command";

    //Add the functions to the function map.  It has to be done here
    //as well as creating the function.
    func_map["help"] = &DebugConsole::help;
    func_map["list"] = &DebugConsole::list;
    func_map["killall"] = &DebugConsole::killall;
    func_map["spawn"] = &DebugConsole::spawn;
    func_map["teleport"] = &DebugConsole::teleport;
    func_map["togglevis"] = &DebugConsole::togglevis;
}

void DebugConsole::run_command(std::string input)
{
    //increase the position in the buffer
    buffer_place += 1;
    if(buffer_place >= buffer_size)
    {
        buffer_place = 0;
    }
    if(buffer.size() < buffer_size)
    {
        buffer.push_back(input);
    }
    else
    {
        buffer[buffer_place] = input;
    }
    current_place = buffer_place;

    //split the command into arguments
    std::vector<std::string> command = utility::split_string(input, ' ');

    //The function we should call
    std::string func = command[0];

    //A place for our string arguments
    command = std::vector<std::string>(command.begin() + 1, command.begin() + command.size());

    //convert our string arguments into ints!
    std::vector<int> args = utility::strings_to_ints(command);


    //call the appropriate function
    if(func_map[func])
    {
        (this->*func_map[func])(command, args);
    }
    else
    {
        debug_message = db_messages[GENERAL_ERROR];
    }
}

void DebugConsole::help(std::vector<std::string> command, std::vector<int> args)
{
    if(command.size() <= 0)
    {
        debug_message = db_messages[HELP];
    }
    else if(command[0] == "spawn")
    {
        debug_message = db_messages[HELP_SPAWN];
    }
    else if(command[0] == "list")
    {
        debug_message = db_messages[HELP_LIST];
    }
    else if(command[0] == "killall")
    {
        debug_message = db_messages[HELP_KILLALL];
    }
    else if(command[0] == "teleport")
    {
        debug_message = db_messages[HELP_TELEPORT];
    }
    else
    {
        debug_message = db_messages[GENERAL_ERROR];
    }
}

void DebugConsole::list(std::vector<std::string> command, std::vector<int> args)
{
    if(command.size() < 1)
    {
        debug_message = db_messages[NUM_ARG_ERROR];
    }
    else if(command[0] == "enemytype")
    {
        debug_message = db_messages[LIST_ENEMYTYPE];
    }
    else if(command[0] == "coords")
    {
        std::stringstream ss;
        int chunk_x = game->main_char.get_chunk().col;
        int chunk_y = game->main_char.get_chunk().row;
        int x = game->main_char.get_x();
        int y = game->main_char.get_y();
        ss << "Chunk (x, y): (" << chunk_x<<", "<<chunk_y<<"), Coords (x, y) : ("<<x<<", "<<y<<")";
        debug_message = ss.str();
    }
    else
    {
        debug_message = db_messages[GENERAL_ERROR];
    }
}

void DebugConsole::killall(std::vector<std::string> command, std::vector<int> args)
{
    std::vector<Character*>* enemy_list = &game->get_characters();
    for(int i=0;i<enemy_list->size();i++)
    {
        Character* temp = enemy_list->at(i);
        temp->kill();
    }
}

void DebugConsole::spawn(std::vector<std::string> command, std::vector<int> args)
{
    if(command.size() < 7)
    {
        debug_message = db_messages[NUM_ARG_ERROR];
    }
    else
    {
        for(int i =0;i<args[6];i++)
        {
            game->spawn_enemy(args[0], args[1], args[2], args[3], args[4], args[5]);
        }
        debug_message = db_messages[COMPLETE];
    }
}

void DebugConsole::teleport(std::vector<std::string> command, std::vector<int> args)
{
    if(command.size() < 4)
    {
        debug_message = db_messages[NUM_ARG_ERROR];
    }
    else
    {
        game->teleport(args[0], args[1], args[2], args[3]);
    }
}

void DebugConsole::togglevis(std::vector<std::string> command, std::vector<int> args) {
    if(game->visibility_on) {
        game->visibility_on = false;
    } else {
        game->visibility_on = true;
    }
}

std::string DebugConsole::get_message()
{
    return debug_message;
}

std::string DebugConsole::input_from_buffer(int direction)
{
    if(buffer.size() == 0)
    {
        return "";
    }
    current_place += direction;
    if(current_place == buffer.size())
    {
        current_place = 0;
    }
    else if(current_place < 0)
    {
        current_place = buffer.size() - 1;
    }
    if(current_place + 1 == buffer_place)
    {
        return "";
    }
    return buffer[current_place];
}
