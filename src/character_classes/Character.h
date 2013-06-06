#ifndef CHARACTER_H
#define CHARACTER_H
#include <SDL/SDL.h>
#include <terrain_defs.h>
#include <ASCII_Lib.h>
#include <def.h>
#include <Dungeon.h>
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
        char sprite;
        int armor;
        Dungeon dungeon;
        //methods
	public:
        Character();
		Character(int, int, int, int, Dungeon);
		void display_character(int character, SDL_Surface* ascii, SDL_Surface* screen, Uint32 color);
		bool is_alive() const;
		void move(int, int);
		void take_damage(int);
        void attack(int x_direction, int y_direction);
		void update_dungeon(Dungeon);
		int get_x_loc();
        int get_y_loc();
        int get_char();
		
		//terrain get_surroundings

};

class Main_Character : public Character{
    protected:
        char name[];
        //inventory
        //equipment
   public:
        Main_Character();
        Main_Character(int, int, int, int, Dungeon);
		void perform_action_cont();
		void perform_action_press(SDLKey);
};

#endif
            

