/**
 *  @file CHARACTER.CPP
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

#include "character.h"

Character::Character() {
    corpse = NULL;
}

Character::Character(int _x, int _y, int _depth)
{
    stats.resize(3);
    x = _x;
    y = _y;
    chunk = IntPoint(0, 0);
    depth = _depth;
    target = NULL;
    equipment = vector<Item*>(7);
    conscious = true;
    level_up = 0;
    master = NULL;
    master_health = 0;
}

Character::Character(std::vector<int> _stats, int _x, int _y, Tile _sprite, MiscType _corpse, int _chunk_x, int _chunk_y, int _depth, int _morality, int _speed, int _ai_id, std::string _name, WeaponType wep) {

    timer = 0;
    stats = _stats;
    current_stats = stats;
    stats[EXPERIENCE] = exp_to_level();
    x = _x;
    y = _y;
    moral = _morality;
    corpse = new Misc(IntPoint(y, x), _corpse);
    //somewhat temporary
    sprite = _sprite;
    chunk = IntPoint(_chunk_y, _chunk_x);
    depth = _depth;
    speed = _speed;
    ai_id = _ai_id;
    target = NULL;
    equipment = vector<Item*>(7);
    conscious = true;
    name = _name;
    master = NULL;
    master_health = 0;

    //These won't do anything for anyone except the enemy, for now.  But,
    //they're here if we need them.
    level_up = 0;
    direction = 0;
    sight = 20;
    view = 20;
    natural_weapon = Weapon(IntPoint(y, x), wep);
}

Character::~Character()
{
    for(int i=0;i<equipment.size();i++)
    {
        delete equipment[i];
    }
    for(int i=0;i<inventory.size();i++)
    {
        delete inventory[i];
    }
    delete corpse;
}

Character::Character(const Character& chara)
{
    timer = chara.timer;
    stats = chara.stats;
    current_stats = chara.current_stats;
    x = chara.x;
    y = chara.y;
    moral = chara.moral;

    if(chara.corpse != NULL)
    {
        corpse = new Misc;
        *corpse = *chara.corpse;
    }
    else
    {
        corpse = NULL;
    }
    for(int i=0;i<chara.equipment.size();i++)
    {
        Item* eq = new Item;
        *eq = *chara.equipment[i];
        equipment.push_back(eq);
    }
    for(int i=0;i<chara.inventory.size();i++)
    {
        Item* inv = new Item;
        *inv = *chara.inventory[i];
        inventory.push_back(inv);
    }

    //somewhat temporary
    sprite = chara.sprite;
    chunk = chara.chunk;
    depth = chara.depth;
    speed = chara.speed;
    ai_id = chara.ai_id;
    target = chara.target;
    conscious = chara.conscious;
    master = chara.master;
    master_health = chara.master_health;
    //These won't do anything for anyone except the enemy, for now.  But,
    //they're here if we need them.
    direction = chara.direction;
    sight = chara.sight;
    view = chara.view;
    level_up = chara.level_up;
    natural_weapon = chara.natural_weapon;
}

Character& Character::operator=(const Character& chara)
{
    timer = chara.timer;
    stats = chara.stats;
    current_stats = chara.current_stats;
    x = chara.x;
    y = chara.y;
    moral = chara.moral;

    delete corpse;
    if(chara.corpse != NULL)
    {
        corpse = new Misc;
        *corpse = *chara.corpse;
    }
    else
    {
        corpse = NULL;
    }
    for(int i=0;i<equipment.size();i++)
    {
        delete equipment[i];
    }
    for(int i=0;i<inventory.size();i++)
    {
        delete inventory[i];
    }
    equipment.resize(chara.equipment.size());
    inventory.resize(chara.inventory.size());
    for(int i=0;i<chara.equipment.size();i++)
    {
        if(chara.equipment[i] != NULL)
        {
            Item* eq = new Item;
            *eq = *chara.equipment[i];
            equipment[i] = eq;
        }
        else
        {
            equipment[i] = NULL;
        }
    }
    for(int i=0;i<chara.inventory.size();i++)
    {
        Item* inv = new Item;
        *inv = *chara.inventory[i];
        inventory[i] = inv;
    }

    //somewhat temporary
    sprite = chara.sprite;
    chunk = chara.chunk;
    depth = chara.depth;
    speed = chara.speed;
    ai_id = chara.ai_id;
    target = chara.target;
    conscious = chara.conscious;
    master = chara.master;
    master_health = chara.master_health;
    //These won't do anything for anyone except the enemy, for now.  But,
    //they're here if we need them.
    direction = chara.direction;
    sight = chara.sight;
    view = chara.view;
    level_up = chara.level_up;
    natural_weapon = chara.natural_weapon;
}

bool Character::act(long ms)
{
    timer += ms;
    if(timer >= speed)
    {
        timer -= speed;
        gain_endurance(1);
        if(!is_conscious())
        {
            if(current_stats[ENDURANCE] > stats[ENDURANCE]/current_stats[ENDURANCE])
            {
                regain_consciousness();
            }
        }
        return true;
    }
    return false;
}

bool Character::is_alive() const {
    if (current_stats[HEALTH] <= 0){
        return false;
    } else {
        return true;
    }
}

void Character::take_damage(Weapon* weapon, int part_hit, int strength){
    //damage should change based on where we hit
    int body_reduction = DAM_REDUCTION[part_hit];

    //start with the base damage of the weapon
    int base = weapon->get_damage();

    //figure out how much armor will reduce it by
    int armor_reduction = get_armor_dam(part_hit, weapon->get_type());

    //get the actual damage done
    int damage = body_reduction * ((base + strength) - (armor_reduction * (base + strength)));


    //deal the damage
    current_stats[HEALTH] -= damage;

    stringstream ss;
    ss<<"You were hit in the "<<BODY_PARTS[part_hit]<<" and have taken "<<damage<<" points of damage.";
    MessageBoard::instance().add_message(ss.str(), ai_id);
}

void Character::attack(Character* _chara)
{
    int hit = rand() % 100 + 1;

    //did we hit?
    bool did_hit = (hit <= accuracy_stat());

    //how solidly did we hit?
    float solid_hit = hit/accuracy_stat();

    //check if it was dodged
    bool did_dodge = _chara->did_dodge(solid_hit);

    //get the body part hit
    int part_hit = get_part_hit(rand() % 100);

    //did the other character block it?
    //we'll leave this out for now
    //bool blocked = chara->did_block(solid_hit, part_hit, get_weapon()->get_type());

    //if we somehow managed to meet all of those conditions
    //then we should do the damage
    if(did_hit && !did_dodge)
    {
        _chara->take_damage(get_weapon(), part_hit, get_current_stat(STRENGTH));
    }
    _chara->set_target(this);

    //deal with the messages
    std::string message;
    if(!did_hit)
    {
        message = "You missed the " + _chara->get_name();
    }
    else if(did_dodge)
    {
        message = "Your attack was dodged by the " + _chara->get_name();
    }
    else
    {
        message = "You hit the " + _chara->get_name() + " in the " + BODY_PARTS[part_hit];
    }
    MessageBoard::instance().add_message(message, ai_id);
}

int Character::get_sight()
{
    return sight;
}


vector<Item*>* Character::get_inventory()
{
    return &inventory;
}

vector<Item*>* Character::get_equipment()
{
    return &equipment;
}

void Character::add_item(Item* new_item)
{
    inventory.push_back(new_item);
}

void Character::drop_item(Item* item)
{
    for(int i=0;i<inventory.size();i++)
    {
        if(inventory[i] == item)
        {
            inventory.erase(inventory.begin() + i);
        }
    }
}

void Character::drop_item(int item)
{
    if(item < inventory.size())
    {
        inventory.erase(inventory.begin() + item);
    }
}

void Character::destroy_item(Item* item)
{
    for (int i=0; i<inventory.size();i++)
    {
        if(inventory[i] == item)
        {
            delete inventory[i];
            inventory.erase(inventory.begin() + i);
        }
    }
}

void Character::nullify_corpse()
{
    corpse = NULL;
}

void Character::equip_item(Item* item)
{
    if(item->can_equip)
    {
        int body_part = ((Equipment*)item)->get_body_part();
        drop_item(item);
        remove_item(body_part);
        equipment[body_part] = item;
    }
    else if(item->can_wield)
    {
        drop_item(item);
        remove_item(6);
        equipment[6] = item;
        current_stats[ATTACK] = ((Weapon*)item)->get_damage();
    }
}

void Character::remove_item(int item)
{
    if(equipment[item] != NULL)
    {
        if(item < 6)
        {
            add_item(equipment[item]);
            equipment[item] = NULL;
        }
        else
        {
            current_stats[ATTACK] = stats[ATTACK];
            add_item(equipment[item]);
            equipment[item] = NULL;
        }
    }
}

void Character::remove_all()
{
    for(int i=0;i<equipment.size();i++)
    {
        remove_item(i);
    }
}


void Character::set_x(int _x) {
    x = _x;
}

void Character::set_y(int _y) {
    y = _y;
}

void Character::set_depth(int d) {
    depth = d;
}

int Character::get_x() {
    return x;
}

int Character::get_y() {
    return y;
}

IntPoint Character::get_coords(){
    return IntPoint(y, x);
}

int Character::get_chunk_x()
{
    return chunk.col;
}

int Character::get_chunk_y()
{
    return chunk.row;
}

IntPoint Character::get_chunk()
{
    return chunk;
}

void Character::set_chunk(IntPoint _chunk)
{
    chunk = _chunk;
}

void Character::set_chunk_x(int _chunk_x)
{
    chunk.col = _chunk_x;
}

void Character::set_chunk_y(int _chunk_y)
{
    chunk.row = _chunk_y;
}


Tile Character::get_char() {
    return sprite;
}

Item* Character::get_corpse(){
    return corpse;
}

int Character::get_depth() {
    return depth;
}

void Character::set_target(Character* _target)
{
    target = _target;
}

Character* Character::get_target()
{
    return target;
}

int Character::get_max_hp()
{
    return stats[HEALTH];
}

int Character::get_cur_hp()
{
    return current_stats[HEALTH];
}

int Character::get_moral()
{
    return moral;
}

int Character::get_armor_hit(int body_part, int type)
{
    Item* item = equipment[body_part];
    if(item != NULL)
    {
        return ((Equipment*)item)->get_hit(type);
    }
    else{
        return 0;
    }
}

float Character::get_armor_dam(int body_part, int type)
{
    Item* item = equipment[body_part];
    if(item != NULL)
    {
        return ((Equipment*)item)->get_dam(type);
    }
    else{
        return 0;
    }
}

void Character::consume_item(Item* item)
{
    if(item->can_consume)
    {
        int type = ((Consumable*)item)->get_type();
        int stat = ((Consumable*)item)->get_stat();
        int amount = ((Consumable*)item)->get_amount();
        if(type == consumables::INCREASE)
        {
            stats[stat] += amount;
        }
        else
        {
            current_stats[stat] += amount;
            if(((Consumable*)item)->get_type() == consumables::RESTORE)
            {
                if(current_stats[stat] > stats[stat])
                {
                    current_stats[stat] = stats[stat];
                }
            }
        }
        destroy_item(item);
    }
}

int Character::get_stat(int stat)
{
    return stats[stat];
}

void Character::set_stat(int stat, int amount)
{
    stats[stat] = amount;
}

int Character::get_current_stat(int stat)
{
    return current_stats[stat];
}

void Character::set_current_stat(int stat, int amount)
{
    current_stats[stat] = amount;
}

void Character::regain_consciousness()
{
    conscious = true;
}

void Character::pass_out()
{
    conscious = false;
}

bool Character::is_conscious()
{
    return conscious;
}

void Character::reduce_endurance(int factor)
{
    int base = 10;
    int reduction = factor * (base/get_current_stat(STRENGTH));
    current_stats[ENDURANCE] -= reduction;
    if(get_current_stat(ENDURANCE) <= 0)
    {
        set_current_stat(ENDURANCE, 0);
        pass_out();
    }
}

void Character::gain_endurance(int factor)
{
    int base = 10;
    int gain = factor * (base/get_current_stat(STRENGTH));
    current_stats[ENDURANCE] += gain;
    if(get_current_stat(ENDURANCE) > stats[ENDURANCE])
    {
        set_current_stat(ENDURANCE, stats[ENDURANCE]);
    }
}


bool Character::in_sight_range(IntPoint _coords, IntPoint _chunk)
{
    IntPoint point = utility::get_abs(_chunk, _coords);
    IntPoint center = utility::get_abs(chunk, get_coords());

    //get the distance.  sign matters
    IntPoint distance = point - center;

    //flag to check if it's in the distance
    return (distance.row * distance.row) + (distance.col * distance.col) <= sight*sight;
}

bool Character::in_sight(IntPoint _coords, IntPoint _chunk)
{
    IntPoint point = utility::get_abs(_chunk, _coords);
    IntPoint center = utility::get_abs(chunk, get_coords());

    //get the distance.  sign matters
    IntPoint distance = point - center;

    //flag to check if it's in the distance
    bool in_distance = (distance.row * distance.row) + (distance.col * distance.col) <= sight*sight;

    //uses the direction the enemy is pointed and the width of the field of view
    //to establish the two angles that represent the upper and lower limits of what
    //the enemy can see
    IntPoint view_field = get_fov();

    //convert them to radians
    double upper_bound = perc_to_rad(view_field.row);
    double lower_bound = perc_to_rad(view_field.col);

    //get the direction of the target
    double target_rads = coords_to_rad(distance);

    //check if the target's angle is in the acceptable range
    bool in_arc = target_rads <= upper_bound && target_rads >= lower_bound;

    //check to make sure there's not something in the way


    return in_arc && in_distance;
}

void Character::turn(IntPoint difference)
{
    int new_perc = coords_to_perc(difference);
    int turn = new_perc - direction;
    int new_direction = direction + turn;

    if(new_direction > 100)
    {
        new_direction -= 100;
    }
    else if(new_direction < 0)
    {
        new_direction += 100;
    }
    direction = new_direction;
}

void Character::turn(int turn_amount)
{
    direction += turn_amount;
    if(direction > 100)
    {
        direction -= 100;
    }
    else if(direction < 0)
    {
        direction += 100;
    }
}

std::vector<IntPoint> Character::sight_tiles()
{
    IntPoint coords = get_coords();
    std::vector<IntPoint> arc =  bresenham_arc(coords, sight, get_fov());
    std::vector<IntPoint> points;
    for(int i =0;i<arc.size();i++)
    {
        std::vector<IntPoint> line_points;
        line_points = bresenham_line(coords, arc[i]);
        for(int j=0;j<line_points.size();j++)
        {
            points.push_back(line_points[j]);
        }
    }
    return points;
}

IntPoint Character::get_fov()
{
    int upper = direction + (.5 * view);
    int lower = direction - (.5 * view);
    return IntPoint(upper, lower);
}

int Character::get_ai_id()
{
    return ai_id;
}

int Character::exp_to_level()
{
    return (stats[LEVEL] * stats[LEVEL]) * 10;
}

void Character::gain_experience(int level)
{
    int increase = level * 5;
    int cur_xp = get_current_stat(EXPERIENCE);
    int new_xp = cur_xp + increase;
    set_current_stat(EXPERIENCE,  new_xp);
    while(new_xp >= exp_to_level())
    {
        new_xp -= exp_to_level();
        set_current_stat(EXPERIENCE, new_xp);
        gain_level();
        set_stat(EXPERIENCE, exp_to_level());
    }
}

void Character::gain_level()
{
    int current_level = get_stat(LEVEL);
    set_stat(LEVEL, current_level + 1);
    set_current_stat(LEVEL, current_level + 1);
    level_up += 1;
}

int Character::get_new_levels()
{
    return level_up;
}

void Character::level_stat(int stat)
{
    stats[stat] += 1;
    current_stats[stat] += 1;
    level_up -= 1;
}

bool Character::has_item_category(int category)
{
    for(int i=0;i<equipment.size();i++)
    {
        if(equipment[i] != NULL && equipment[i]->has_category(category))
        {
            return true;
        }
    }
    for(int i=0;i<inventory.size();i++)
    {
        if(inventory[i] != NULL && inventory[i]->has_category(category))
        {
            return true;
        }
    }
    return false;
}

std::string Character::get_name()
{
    return name;
}

Character* Character::get_master()
{
    return master;
}

void Character::set_master(Character* new_m)
{
    master = new_m;
    update_master_health();
}

int Character::master_hp()
{
    return master->get_cur_hp();
}

void Character::update_master_health()
{
    if(master != NULL)
    {
        master_health = master->get_cur_hp();
    }
}

bool Character::master_health_changed()
{
    return master_health != master->get_cur_hp();
}

Weapon* Character::get_weapon()
{
    if(equipment[6] == NULL || !equipment[6]->can_wield)
    {
        return &natural_weapon;
    }
    else
    {
        return ((Weapon*)equipment[6]);
    }
}

void Character::kill()
{
    set_current_stat(HEALTH, 0);
}

//------------------Character Stats---------------------//

float Character::dodge_stat()
{
    float dex = (float)get_current_stat(DEXTERITY);
    //yay magic numbers!
    return (((dex)/(dex + 50)) * 100);
}

int Character::accuracy_stat()
{
    float dex = (float)get_current_stat(DEXTERITY);
    float dex_perc = (dex/(dex + 20));
    return (dex_perc * 100);
}

bool Character::did_dodge(float hit)
{
    int dodge = rand() % 100;
    float true_dodge = dodge/100;
    float dodge_score = (1 - hit) * dodge_stat();
    return dodge <= dodge_score;
}

/**
bool Character::did_block(int hit, int body_part, int type)
{
    int block = get_armor_hit(body_part, type);
    return block >

}
**/
