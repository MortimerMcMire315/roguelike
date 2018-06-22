/**
 *  @file MAIN_MENU.H
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

#ifndef _MENU_H
#define _MENU_H

#include <iostream>
#include <string>
#include <vector>

#include "ASCII_Lib.h"
#include "defs.h"
#include "game_states.h"
#include "game_loader.h"
#include "utility.h"

//forward declarations
class Item;
class Game;
class WorldMapGUI;

using namespace std;

namespace menu_id {
    enum Menu_ID {
        NOTHING,
        AUDIO_MENU,
        EQUIP_ITEM,
        EQUIPMENT_MENU,
        ESCAPE_MENU,
        FONT_MENU,
        INVENTORY_MENU,
        ITEM_MENU,
        MAIN_MENU,
        START_MENU,
        INFO_MENU,
        LOAD_MENU,
        LEVEL_MENU,
        MESSAGE_MENU
    };
}

/**
 * The basis of the menuing system for the roguelike.
 * Each different type of menu is going to be a subclass that overloads the make_selection() function
 * to provide different functionality.  See the documentation for make_selection for more detail.
 */
class Menu {
    public:
        /**
         * An instance of game which allows the menu to interact with most objects in the game.
         * This is in the menu so that the menu is able to access information about almost everything
         * (characters, items, chunks, etc.) so that accessing the menu actually does something instead
         * of...well...nothing.
         */
        Game* game;

        /**
         * The game state to switch to after closing the menu system.
         * This usually becomes GAME_SCREEN, but not always.  This refers to when you close
         * the entire menuing system, not when you switch between two menus, as it will stay
         * with the MENU_SCREEN.
         */
        Screen next_screen;

        /**
         * The menu item which is currently selected (corresponds to an index in options).
         */
        int selection;

        /**
         * The title of the menu.
         */
        string title;

        /**
         * The tile which is used as a background for the menu.
         * It occurs to me that this is actually a background, now a border.
         * \todo Make this an actual border.
         */
        Tile border;

        /**
         * The possible options which can be chosen for the menu.
         */
        vector<string> options;


        /**
         * Whether or not the menu screen should be closed.
         * This exits out of the entire menuing system, not just one menu.
         */
        bool exit;

        /**
         * Checks whether or not a selection is out of the bounds of the menu.
         * @param row The number to check.
         * @return True if the option is larger than the size of the list or smaller than 0.
         */
        bool out_of_bounds(int row);

        /**
         * A numerical id for the menu.
         * Each type of menu has a different id.
         */
        int id;

        /**
         * The constructor for the menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         */
        Menu(int _padding, Tile _border, int _selection = 0);

        /**
         * Changes the currently selected menu option.
         * @param direction Always +/- 1, determines whether the selection goes up or down.
         */
        void move_selection(int direction);

        /**
         * The distance between the menu text and the edge of the menu area.
         */
        int padding;

        /**
         * Anything extra to be displayed in a menu.
         * This is pertinent if more information needs to be given by a menu (
         * as in the case of the InfoMenu).
         */
        vector<string> extra_lines;

        /**
         * The function that handles when a selection is made in the menu.
         * This is the most important function.  There are four possibilities:
         * 1.  The selection just transitions to a new menu.  The menu returned will
         *     be the new menu.
         * 2.  The selection just performs a function and then returns to the same menu.
         *     In this case, "this" will be returned and nothing else will happen.
         * 3.  The selection causes an exit of the menu screen.  When this happens,
         *     "this" will be returned and should_exit will be toggled. On the next pass,
         *     the game_state will then transition to the next_screen.
         * 4.  The function returns NULL, in which case the game is exited.
         * There's no reason that 2 & 3 can't be combined.
        */
        virtual Menu* make_selection() = 0;

        /**
         * Adds an item to the options.
         * @param new_item Item to add to the menu.
         */
        void add_item(string new_item);

        /**
         * PUblic accessor for exit.
         */
        bool should_exit();

        /**
         * Public accessor for selection.
         */
        int get_selection();

        /**
         * Flips the variable exit.
         */
        void toggle_exit();

        /**
         * Returns the screen to switch to upon exit.
         */
        Screen get_screen();

        /**
         * Public accessor for the id.
         */
        int get_id();

        /**
         * Public accessor for number of extra_lines.
         */
        int num_extra_lines();

        /**
         * Public accessor for extra_lines.
         */
        vector<string> get_extra_lines();
};

/**
 * The menu displayed for the opening screen.
 */
class StartMenu : public Menu
{
    public:
        Game* game;
        WorldMapGUI* world_map_gui;
        /**
         * Constructor for the start_menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         */
        StartMenu(int _padding, Tile _border, Game &g, WorldMapGUI &wmg);
        Menu* make_selection();

        void create_save_folder();
};

class LoadMenu : public Menu
{
    public:
        /**
         * Constructor for the load menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         */

        LoadMenu(int _padding, Tile _border, Game &g, WorldMapGUI &wmg);
        Menu* make_selection();

        Game* game;
        WorldMapGUI* world_map_gui;
};

/**
 * The main menu brought up when the player hits "m."
 */
class MainMenu: public Menu
{
    public:
        /**
         * Constructor for the Main Menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         * @param _game Sets the game for the menu.
         */

        MainMenu(int _padding, Tile _border, Game* _game);
        Menu* make_selection();


};

/**
 * The menu which lists all of the equipment that the player is (or isn't) wearing.
 * Upon selection, this menu will bring up and EquipMenu with the selected item,
 * allowing players to interact with it.
 */
class EquipmentMenu: public Menu
{
    public:
        /**
         * Constructor for the Equipment Menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         * @param _game Sets the game for the menu.
         */
        EquipmentMenu(int _padding, Tile _border, Game* _game);
        Menu* make_selection();


};

/**
 * A menu which lists all of the items in the player's inventory.
 * This menu allows the player to intereact with their menu items, and
 * will create a new ItemMenu upon selection.
 */
class InventoryMenu: public Menu
{
    protected:
        /**
         * The list of items in the main_character's inventory.
         */
        vector<Item*>* items;
    public:
        /*
         * Constructor for the Inventory Menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         * @param _game Sets the game for the menu.
         */
        InventoryMenu(int _padding, Tile _border, Game* _game);
        Menu* make_selection();

};

/**
 * The menu which is brought up when a player selects an item in the inventory menu.
 * This menu allows players to perform actions on items.
 */
class ItemMenu: public Menu
{
    protected:
        /**
         * The item to perform actions on.
         */
        Item* item;
    public:
        /*
         * Constructor for the Item Menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         * @param _game Sets the game for the menu.
         * @param _item Sets the item for the menu.
         */
        ItemMenu(int _padding, Tile _border, Game* _game, Item* _item);
        Menu* make_selection();

};

/**
 * The menu which is brought up when a player selects and item from the EquipmentMenu.
 * This menu allows players to interact with the items that they currently have equipped.
 */
class EquipMenu: public Menu
{
    protected:
        /**
         * The index in the main_character's equipment to act on.
         */
        int item;
    public:
        /*
         * Constructor for the Equip Menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         * @param _game Sets the game for the menu.
         * @param _item Sets the item for the menu.
         */
        EquipMenu(int _padding, Tile _border, Game* _game, int _item);
        Menu* make_selection();

};

/**
 * The menu brought up upon hitting escape which allows players to interact with settings.
 */
class EscapeMenu: public Menu
{
    public:
        /*
         * Constructor for the Escape Menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         * @param _game Sets the game for the menu.
         */
        EscapeMenu(int _padding, Tile _border, Game* _game);
        Menu* make_selection();

};

/**
 * The menu to change the audio settings.
 * Currently, this does nothing, as we haven't added audio yet.
 * \todo Priority: Low. Add audio, maybe Dilbert2 theme performed by Seth and Kendall.
 */
class AudioMenu: public Menu
{
    public:
        /*
         * Constructor for the Audio Menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         * @param _game Sets the game for the menu.
         */
        AudioMenu(int _padding, Tile _border, Game* _game);
        Menu* make_selection();

};

/**
 * The menu allowing players to change the font.
 * I suppose this is as good of a place as any on how to document how to
 * add a custom font.  The "font" is actually a 128x204px .bmp image which
 * is filled with 8x12 rectangles.  These are the actual tiles which are used
 * for the game.  Simply place a new .bmp in the data/font, and you can change
 * the font in game in this menu (escape->font menu->your_font_name.bmp).  Maybe
 * some day I'll put out a list of characters we use in the game.  In order to change
 * the default font, edit the settings.conf and change the font=default to the name
 * of the font file you added (without the .bmp and no spaces).  It should be noted that
 * the characters we use in game are the same as the font we use to draw strings, so
 * only change the roman alphabet characters if you want to be really, really confused.
 * \todo Separate the font and the character set.
 */
class FontMenu: public Menu
{
    private:
        /**
         * The font which is currently selected.
         */
        string font;
    public:
        /*
         * Constructor for the Font Menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         * @param _game Sets the game for the menu.
         */
        FontMenu(int _padding, Tile _border, Game* _game);
        Menu* make_selection();

        /**
         * Public accessor for the font.
         */
        string get_font();
};

/**
 * The menu which brings up information about an item.
 * It's...it's not any more complicated than that.  You can make your own puzzles.
 */
class InfoMenu: public Menu
{
    protected:
        /**
         * The item to display the description of.
         */
        Item* item;
    public:
        /*
         * Constructor for the Info Menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         * @param _game Sets the game for the menu.
         * @param _item Sets the item for the menu.
         */
        InfoMenu(int _padding, Tile _border, Game* _game, Item* _item);
        Menu* make_selection();
};

/**
 * The menu which becomes accessible when a character levels up.
 */
class LevelMenu: public Menu
{
    protected:
    public:
        /*
         * Constructor for the Level Menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         * @param _game Sets the game for the menu.
         */
        LevelMenu(int _padding, Tile _border, Game* _game);
        Menu* make_selection();
};

/**
 * The menu which displays the characters previous messages.
 */
class MessageMenu: public Menu
{
    protected:
        /*
         * The page of messages to display.
         */
        int page;
        /*
         * The  number of entries per page.  Is a constant.
         */
        int per_page;
    public:
        /*
         * Constructor for the Level Menu.
         * @param _padding Sets the padding for the menu.
         * @param _border Sets the border for the menu.
         * @param _game Sets the game for the menu.
         */
        MessageMenu(int _padding, Tile _border, Game* _game, int _page, int _selection);
        Menu* make_selection();
};

#endif
