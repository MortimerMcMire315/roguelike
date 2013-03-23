#include <Character.h>

/* PRE: Will be given a max health, a starting x and a starting y
 * The x and the y are the coordinates within the current chunk/dungeon
 * POST: A character object with the desired attributes will be returned
*/
Character::Character(int _max_health, int _x, int _y, char _sprite)
{
	current_health = _max_health;
	max_health = _max_health;
	x = _x;
	y = _y;
    sprite = _sprite;
}

/* PRE: None
 * POST: Returns true if the character is still alive (health is > max_health)
*/
bool Character::is_alive() const{
	if (current_health <= 0){
		return false;
	}
	else{
		return true;
	}
}

/* PRE: Will be passed the desired change in x and y coordinates
 * POST: Will change the character's coordinates to match this
*/
void Character::move(int x_change, int y_change){
	mvdelch(y, x);
    x += x_change;
	y += y_change;
    mvaddch(y, x, sprite);
}

/* PRE: Takes damage to reduce a character's health by
 * POST: Subtracts the damage from health and checks if the character is still alive
*/
void Character::take_damage(int damage){
	current_health -= damage;
	if (is_alive() == false){
		//delete the character, end the game
		}
}

int Character::get_x_loc(){
    return x;
}

int Character::get_y_loc(){
    return y;
}

char Character::get_char(){
    return sprite;
}
