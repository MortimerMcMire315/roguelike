/**
 *  CHARACTER.H
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

#ifndef CHARACTER_H
#define CHARACTER_H
#include <SDL/SDL.h>
#include <terrain_defs.h>
#include <ASCII_Lib.h>
#include <def.h>
#include <iostream>
#include <int_point.h>
class Character
{
    protected:
        //variables
        int current_health;
        int max_health;
        //current coordinates within dungeon
        int x;
        int y;
        //Skills skill[];
        Tile sprite;
        int armor;
        int depth;
        IntPoint chunk;
        Tile underfoot;
        
        //methods

    public:
        Character();
        Character(int, int, int, Tile, int, int, int);
        Character(int, int, int, int, int);
        bool is_alive() const;
        void take_damage(int);
        void attack(int x_direction, int y_direction);
        int get_x();
        int get_y();
        IntPoint get_chunk();
        int get_chunk_x();
        int get_chunk_y();
        Tile get_char();
        int get_depth();
        void set_x(int);
        void set_y(int);
        void set_chunk(IntPoint);
        void set_chunk_x(int);
        void set_chunk_y(int);
        void set_depth(int);
        
        Tile* get_underfoot();
        //terrain get_surroundings

};

class Main_Character : public Character{
    protected:
        char name[];
        //inventory
        //equipment
   public:
        Main_Character();
        Main_Character(int, int, int, Tile, int, int, int);
};

#endif
