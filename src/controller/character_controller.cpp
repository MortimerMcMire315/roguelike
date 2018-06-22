/**
 *  GAME.H
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

#include "game.h"

/**
 * This controller is to access character data and control characters.
 * It provides the interface between the character data structures in
 * the game (e.g. character_list) and the ai/key inputs/other data
 * structures.
 */

void Game::update_character_index()
{
    clear_character_index();
    for(int i=0;i<character_list.size();i++)
    {
        if(point_in_buffer(character_list[i]->get_chunk(), character_list[i]->get_coords()))
        {
            character_to_index(character_list[i]);
        }
    }
    character_to_index(&main_char);
}


void Game::clear_character_index()
{
    for(int i=0;i<character_index.size();i++)
    {
        for(int j=0;j<character_index[i].size();j++)
        {
            character_index[i][j] = NULL;
        }
    }
}


void Game::character_to_index(Character* chara)
{
    IntPoint buffer_coords = get_buffer_coords(chara->get_chunk(), chara->get_coords());
    character_index[buffer_coords.row][buffer_coords.col] = chara;
}


void Game::remove_index_char(Character* chara)
{
    IntPoint buffer_coords = get_buffer_coords(chara->get_chunk(), chara->get_coords());
    if(coords_in_buffer(buffer_coords.row, buffer_coords.col))
    {
        character_index[buffer_coords.row][buffer_coords.col] = NULL;
    }
}

std::vector<Character*>& Game::get_characters() {
    return character_list;
}


void Game::remove_targets(Character* enem)
{
    for(int i=0;i<character_list.size();i++)
    {
        if(character_list[i]->get_target() == enem)
        {
            character_list[i]->set_target(NULL);
        }
    }
    if(main_char.get_target() == enem)
    {
        main_char.set_target(NULL);
    }
}


bool Game::find_target(Character* chara){
    //establish the necessary variables
    //the character is 'passive'
    Character* best = NULL;
    std::vector<Character*> characters = characters_in_range(chara);
    if(chara->get_moral() == 3)
    {
        best = passive_target(chara, characters);
    }
    else
    {
        best = normal_target(chara, characters);
    }

    if(best != NULL)
    {
        chara->set_target(best);
        return true;
    }
    else
    {
        return false;
    }
}

Character* Game::normal_target(Character* chara, std::vector<Character*> characters)
{
    Character* best = NULL;
    Character* new_character = NULL;
    int target_id = 5 - chara->get_moral();
    int selectability = 2;

    for(int i=0;i<characters.size();i++)
    {
        new_character = characters[i];
        if(new_character->get_moral() > target_id - selectability && new_character->get_moral() < target_id + selectability)
        {
            //If we don't have a target, this is the best target
            if(best == NULL)
            {
                best = new_character;
            }
            else
            {
                //otherwise, check if this one we found is better
                if((unsigned int)(new_character->get_moral() - target_id) < (unsigned int)(best->get_moral() - target_id))
                {
                    best = new_character;
                }
            }
        }
    }
    return best;
}

Character* Game::passive_target(Character* chara, std::vector<Character*> characters)
{
    Character* best = NULL;
    Character* new_character = NULL;
    for(int i=0;i<characters.size();i++)
    {
        new_character = characters[i];
        if(new_character->get_moral() != chara->get_moral())
        {
            //If we don't have a target, this is the best target
            if(best == NULL)
            {
                best = new_character;
            }
            else
            {
                //otherwise, check if this one we found is better
                if((unsigned int)(new_character->get_moral() - chara->get_moral()) < (unsigned int)(best->get_moral() - chara->get_moral()))
                {
                    best = new_character;
                }
            }
        }
    }
    return best;
}


std::vector<Character*> Game::characters_in_range(Character* chara)
{
    std::vector<IntPoint> sight_tiles = chara->sight_tiles();
    std::vector<Character*> temp;
    Character* new_character;
    for(int i=0;i<sight_tiles.size();i++)
    {
        IntPoint index_coords = get_buffer_coords(chara->get_chunk(), sight_tiles[i]);
        if(coords_in_buffer(index_coords.row, index_coords.col))
        {
            new_character = character_index[index_coords.row][index_coords.col];
        }
        if(new_character != NULL)
        {
            temp.push_back(new_character);
        }
    }
    return temp;
}

int Game::move_to_point(Character* chara, IntPoint coords, IntPoint chunk)
{
    IntPoint goal = get_buffer_coords(chunk, coords);
    IntPoint current = get_buffer_coords(chara->get_chunk(), chara->get_coords());
    IntPoint movement = pathfinding::get_next_step(goal, buffer, current, chara->get_sight());
    if((movement + current) == goal)
    {
        return 1; //success
    }
    else if(move_char(movement.col, movement.row, chara))
    {
        return 2; //running
    }
    else
    {
        return 0; //we failed :(
    }
}


int Game::run_away(Character* chara, IntPoint coords, IntPoint chunk)
{
    IntPoint current_coords = utility::get_abs(chara->get_chunk(), chara->get_coords());
    IntPoint other_coords = utility::get_abs(chunk, coords);
    IntPoint movement = pathfinding::get_opposite(current_coords, other_coords);
    return move_char(movement.col, movement.row, chara);
}

void Game::wander(Character* chara)
{
    int will_move = rand() % 5;
    int x_change = rand() % 3 - 1;
    int y_change = rand() % 3 - 1;
    IntPoint new_coords = IntPoint(chara->get_y() + y_change, chara->get_x() + x_change);

    int turn_amount = rand() % 20 - 10;
    if(will_move == 0)
    {
        chara->turn(turn_amount);
    }

    if(point_in_buffer(chara->get_chunk(), new_coords) && will_move == 0)
    {
        move_char(x_change, y_change, chara);
    }
}

bool Game::next_to_char(Character* chara, Character* target)
{
    IntPoint chara_abs = utility::get_abs(chara->get_chunk(), chara->get_coords());
    IntPoint target_abs = utility::get_abs(target->get_chunk(), target->get_coords());
    IntPoint difference = chara_abs - target_abs;
    bool x = difference.col <= 1 && difference.col >= -1;
    bool y = difference.row <= 1 && difference.row >= -1;
    bool same_coords = difference.col == 0 && difference.row == 0;
    return x && y && !same_coords;
}

void Game::kill(Character* chara)
{
    chara->remove_all();
    vector<Item*>* item_list = chara->get_inventory();
    Item* corpse = chara->get_corpse();
    drop_item(corpse, chara);
    chara->nullify_corpse();
    for(int j=0;j<item_list->size();j++)
    {
        Item* item = item_list->at(j);
        std::cout<<item->get_name()<<std::endl;
        drop_item(item, chara);
    }
    remove_enemy(chara);
}

void Game::remove_enemy(Character* chara)
{
    remove_targets(chara);
    for(int i=0;i<character_list.size();i++)
    {
        if(character_list[i] == chara)
        {
            character_list.erase(character_list.begin() + i);
        }
    }
    remove_index_char(chara);
    delete chara;
}


bool Game::character_in_range(Character* chara)
{
        return chunk_in_buffer(chara->get_chunk_y(), chara->get_chunk_x());
}


/*--------------------Character Controller Functions----------------------*/


void Game::change_depth(int direction, Character* chara) {
    assert(direction == -1 || direction == 1);
    Chunk * current_chunk;
    current_chunk = get_current_chunk();
    Tile* current_tile = current_chunk->get_tile(chara->get_depth(),
            chara->get_y(), chara->get_x());
    if(direction == -1) {
        if (chara->get_depth() >= 1) {
            if(*current_tile == Tileset::get("UP_STAIR")) {
                chara->set_depth(chara->get_depth() - 1);
                chara->set_x(current_chunk->get_down_stairs(chara->get_depth())[0].col);
                chara->set_y(current_chunk->get_down_stairs(chara->get_depth())[0].row);
            }
        }
    } else {
        if (chara->get_depth()+1 < current_chunk->get_depth()) {
            if(*current_tile == Tileset::get("DOWN_STAIR")) {
                chara->set_depth(chara->get_depth() + 1);
                chara->set_x(current_chunk->get_up_stairs(chara->get_depth())[0].col);
                chara->set_y(current_chunk->get_up_stairs(chara->get_depth())[0].row);
            }
        } else {
            cout<<"DEPTH OF THIS CHUNK: "<<current_chunk->get_depth()<<endl;
        }
    }
}

bool Game::move_char(int col_change, int row_change, Character* chara) {
    if(!chara->is_conscious()) {
        return false;
    }

    int row = chara->get_y();
    int col = chara->get_x();
    int next_col = chara->get_x() + col_change;
    int next_row = chara->get_y() + row_change;

    IntPoint new_chunk = IntPoint(chara->get_chunk_y() + (next_row >= CHUNK_HEIGHT) - (next_row<0),
            chara->get_chunk_x() + (next_col>=CHUNK_WIDTH) - (next_col<0));

    next_col = next_col +  (CHUNK_WIDTH * (next_col<0)) - (CHUNK_WIDTH * (next_col>=CHUNK_WIDTH));
    next_row = next_row +  (CHUNK_HEIGHT * (next_row<0)) - (CHUNK_HEIGHT * (next_row>=CHUNK_HEIGHT));
    IntPoint next_coords = IntPoint(next_row, next_col);

    Character* enem = character_at_loc(new_chunk, next_coords);

    IntPoint buffer_coords = get_buffer_coords(new_chunk, IntPoint(next_row, next_col));
    bool can_move = coords_in_buffer(buffer_coords.row, buffer_coords.col) && buffer[buffer_coords.row][buffer_coords.col]->can_be_moved_through;

    if(can_move && (enem == NULL)) {
        remove_index_char(chara);
        col = next_col;
        row = next_row;
        chara->set_x(col);
        chara->set_y(row);
        if(chara->get_chunk() != new_chunk) {
            chara->set_chunk(new_chunk);
        }
        chara->reduce_endurance(1);
        character_to_index(chara);
        return true;
    }
    else if(can_move && (enem != NULL) && chara == &main_char)
    {
        attack_char(&main_char, enem);
    }
    else {
        return false;
    }
}

bool Game::attack_char(Character* chara, Character* target)
{
    chara->attack(target);
    chara->set_target(target);
    chara->reduce_endurance(2);
    if(!target->is_alive())
    {
        chara->gain_experience(target->get_stat(LEVEL));
    }
    return true;
}


void Game::turn_character(Character* chara, Character* target)
{
    if(target != NULL)
    {
        IntPoint chara_coords = utility::get_abs(chara->get_chunk(), chara->get_coords());
        IntPoint target_coords = utility::get_abs(target->get_chunk(), target->get_coords());
        chara->turn(target_coords - chara_coords);
    }
}

void Game::turn_away(Character* chara, Character* target)
{
    if(target != NULL)
    {
        chara->turn(chara->get_coords() - target->get_coords());
    }
}

bool Game::valid_target(Character* chara, Character* target)
{
    bool ret = target != NULL && chara->in_sight_range(target->get_coords(), target->get_chunk());
    return ret;
}

/// \todo Make this take in a character so that other characters can call it?
void Game::get_item(Character* chara)
{
    IntPoint coords = chara->get_coords();
    IntPoint chunk = chara->get_chunk();
    Item* temp_item = item_at_coords(IntPoint(chara->get_y(), chara->get_x()), chara->get_chunk(), chara->get_depth());
    Chunk* current_chunk = chunk_map.get_chunk_abs(chara->get_chunk_y(), chara->get_chunk_x());
    if(temp_item != NULL)
    {
        ///\todo Add a check if the inventory is full
        chara->add_item(temp_item);
        current_chunk->remove_item(temp_item, chara->get_depth());
        IntPoint b_coords = get_buffer_coords(chunk, coords);
        bool was_seen = buffer[b_coords.row][b_coords.col]->visible;
        Item* item = item_at_coords(IntPoint(chara->get_y(), chara->get_x()), chara->get_chunk(), chara->get_depth());
        if(item != NULL)
        {
            buffer[b_coords.row][b_coords.col] = item->get_sprite();
        }
        else
        {
            buffer[b_coords.row][b_coords.col] = current_chunk->get_tile(chara->get_depth(), coords.row, coords.col);
        }
        buffer[b_coords.row][b_coords.col]->visible = was_seen;
    }
}

void Game::drop_item(Item* item, Character* chara)
{
    IntPoint coords = IntPoint(chara->get_y(), chara->get_x());
    IntPoint chunk = IntPoint(chara->get_chunk_y(), chara->get_chunk_x());
    Chunk* current_chunk = chunk_map.get_chunk_abs(chunk.row, chunk.col);

    chara->drop_item(item);
    item->set_coords(coords);

    current_chunk->add_item(item, chara->get_depth());

    add_tile_to_buffer(chunk, coords, item->get_sprite());
}

void Game::harvest_plant(Plant *plant, Character* chara)
{
    if(chara != NULL)
    {
        std::string message;
        if(chara->has_item_category(plant->get_harvest_tool()))
        {
            std::vector<Misc*> drops = plant->pop_drops();
            for(int i=0;i<drops.size();i++)
            {
                chara->add_item(drops[i]);
            }
            chunk_map.get_chunk_abs(plant->get_chunk_y(), plant->get_chunk_x())->kill_plant(plant, chara->get_depth());
            message = "You have harvested a " + plant->get_name();
        }
        else
        {
            message = "You don't have the tool to do that!";
        }

        MessageBoard::instance().add_message(message, chara->get_ai_id());
    }
}


Character* Game::character_at_loc(IntPoint _chunk, IntPoint _coords) {
    IntPoint coords = get_buffer_coords(_chunk, _coords);
    assert(coords_in_buffer(coords.row, coords.col));
    return character_index[coords.row][coords.col];
}
