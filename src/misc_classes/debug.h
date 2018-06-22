/**
 *  @file DEBUG.H
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

#ifndef _DEBUG_H
#define _DEBUG_H

#include <string>
#include <vector>
#include <map>

#include "game.h"
#include "utility.h"

class DebugConsole;

typedef void (DebugConsole::*callbackFunction)(std::vector<string>, std::vector<int>);


/**
 * An enum containing the indexes of standard debugging strings.
 */
enum DEBUG_MESSAGES
{
    GENERAL_ERROR,
    NUM_ARG_ERROR,
    TYPE_ARG_ERROR,
    HELP,
    HELP_SPAWN,
    HELP_LIST,
    HELP_KILLALL,
    HELP_TELEPORT,
    LIST_ENEMYTYPE,
    COMPLETE
};

static std::string db_messages[10] = {
    "I'm sorry, I couldn't understand that command.",
    "Too few arguments.",
    "Incorrect argument types.",
    "Commands: spawn, help, list, killall.  Type 'help <command>' for how to use a command.",
    "Spawn enemies.  Args: chunk_x, chunk_y, x, y, depth, type of enemy, times to run command.",
    "List available something. Options are: enemytype, coords",
    "Kill all the enemies.  Like, all of them.",
    "Teleports the player. Args: chunk_x, chunk_y, x, y",
    "EnemyTypes--1: Kobold, 2: Rabbit",
    "Done."
};

class DebugConsole
{
    private:

        /**
         * A buffer to hold the previous commands which have been input.
         */
        std::vector<std::string> buffer;

        /**
         * An index of where the most recent command is in the buffer.
         */
        int buffer_place;
        /**
         * The current place that we are in the buffer.
         */
        int current_place;

        /**
         * The maximum size of the buffer.
         */
        int buffer_size;

        /**
         * The message to show the player.
         */
        std::string debug_message;

        /**
         * An instance of the game so the console can do things.
         */
        Game* game;

        /**
         * A hashmap to make the functions directly callable.
         * This allows the functions to be called directly by name
         * instead of needing to use an if else or switch case.
         */
        std::map<std::string, callbackFunction> func_map;

    public:
        /**
         * Empty constructor.
         */
        DebugConsole();

        /**
         * Constructor for DebugConsole.
         * @param _game Sets the game member object to _game.
         */
        DebugConsole(Game* _game);
        /**
         * Runs a command based on input.
         * Takes in a string, processes it, and runs the command.
         * The string includes the space separated arguments.
         * @param command The command to run.
         */
        void run_command(std::string input);

        /**
         * Accessor for the member variable debug_message.
         * @return Member variable debug_message.
         */
        std::string get_message();

        /**
         * Returns somewhere in the buffer.
         * Takes in a number, and changes current_place by that
         * value.
         * @param direction The direction to move in the buffer.
         * @return The string at that location in the buffer.
         */
        std::string input_from_buffer(int direction);

        /**
         * Brings up the help menu.
         * @param command The list of string arguments for the function.
         * @param args The list of int arguments for the function.
         */
        void help(std::vector<std::string> command, std::vector<int> args);

        /**
         * Lists something (i.e. your coordinates).
         * @param command The list of string arguments for the function.
         * @param args The list of int arguments for the function.
         */
        void list(std::vector<std::string> command, std::vector<int> args);

        /**
         * Kills everything in game.enemy_list.
         * @param command The list of string arguments for the function.
         * @param args The list of int arguments for the function.
         */
        void killall(std::vector<std::string> command, std::vector<int> args);

        /**
         * Spawns enemies.
         * @param command The list of string arguments for the function.
         * @param args The list of int arguments for the function.
         */
        void spawn(std::vector<std::string> command, std::vector<int> args);

        /**
         * Teleports the main character.
         * @param command The list of string arguments for the function.
         * @param args The list of int arguments for the function.
         */
        void teleport(std::vector<std::string> command, std::vector<int> args);

        /**
         * Toggles the visibility system on and off.
         * @param command The list of string arguments for the function.
         * @param args The list of int arguments for the function.
         */
        void togglevis(std::vector<std::string> command, std::vector<int> args);
};

#endif
