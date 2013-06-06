#include <Character.h>
#include <iostream>

Main_Character::Main_Character()
{

}

Main_Character::Main_Character(int _max_health, int _x, int _y, int _sprite, Dungeon _dungeon ) : Character(_max_health, _x, _y,  _sprite, _dungeon){
    max_health = _max_health;
    x = _x;
    y = _y;
    sprite = _sprite;
	dungeon = _dungeon;
}

void Main_Character::perform_action_cont(){
	SDL_PumpEvents();
	Uint8* keystate = SDL_GetKeyState(NULL);
	
	if(keystate[SDLK_LEFT]){
		move(-1, 0);
	}
	if(keystate[SDLK_RIGHT]){
		move(1, 0);
	}
	if(keystate[SDLK_UP]){
		move(0, -1);
	}
	if(keystate[SDLK_DOWN]){
		move(0, 1);
	}
}

void Main_Character::perform_action_press(SDLKey key){
	switch (key)
	{
		case SDLK_ESCAPE:
			std::cout<<"I'm escaping now, but not continuously!"<<std::endl;
			break;
		default:
			break;
	}
	
}
