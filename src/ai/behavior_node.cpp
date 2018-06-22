/**
 *  BEHAVIOR_NODE.CPP
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

#include "behavior_node.h"

//---------------------------COMPOSITE NODES-----------------------//

std::vector<BNode*> BNode::get_children()
{
    return nodes;
}

void BNode::add_child(BNode* node, int index)
{
    assert(index < nodes.size());
    nodes[index] = node;
}

SequenceNode::SequenceNode(std::vector<BNode*> _nodes)
{
    nodes = _nodes;
}

int SequenceNode::tick(BActor actor, Game* game)
{
    int did_succeed;
    for(int i=0;i<nodes.size();i++)
    {
        did_succeed = nodes[i]->tick(actor, game);
        if(did_succeed != SUCCESS)
        {
            return did_succeed;
        }
    }
    return SUCCESS;
}

SequenceNode* SequenceNode::clone()
{
    return new SequenceNode(*this);
}

PriorityNode::PriorityNode(std::vector<BNode*> _nodes)
{
    nodes = _nodes;
}

int PriorityNode::tick(BActor actor, Game* game)
{
    int did_succeed;
    for(int i=0;i<nodes.size();i++)
    {
        did_succeed = nodes[i]->tick(actor, game);
        if(did_succeed != FAILURE)
        {
            return did_succeed;
        }
    }
    return FAILURE;
}


PriorityNode* PriorityNode::clone()
{
    return new PriorityNode(*this);
}


BranchingCondition::BranchingCondition(BNode* condition, BNode* success, BNode* failure)
{
    nodes.push_back(condition);
    nodes.push_back(success);
    nodes.push_back(failure);
}

int BranchingCondition::tick(BActor actor, Game* game)
{
    int did_succeed = nodes[0]->tick(actor, game);
    if(did_succeed == SUCCESS)
    {
        return nodes[1]->tick(actor, game);
    }
    else if(did_succeed == FAILURE)
    {
        return nodes[2]->tick(actor, game);
    }
    else
    {
        return did_succeed;
    }
}

BranchingCondition* BranchingCondition::clone()
{
    return new BranchingCondition(*this);
}

//---------------------------DECORATOR NODES-------------------------//

InverterNode::InverterNode(BNode* node)
{
    nodes.push_back(node);
}

int InverterNode::tick(BActor actor, Game* game)
{
    int did_succeed = nodes[0]->tick(actor, game);
    if(did_succeed == FAILURE || did_succeed == SUCCESS)
    {
        return !did_succeed;
    }
    else
    {
        return did_succeed;
    }
}

InverterNode* InverterNode::clone()
{
    return new InverterNode(*this);
}

//----------------------------CONDITION NODES------------------------//

int EnemyInRange::tick(BActor actor, Game* game)
{
    return game->find_target(actor.get_character());
}

EnemyInRange* EnemyInRange::clone()
{
    return new EnemyInRange(*this);
}

int GetMTarget::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    if(chara->get_master() != NULL)
    {
        chara->set_target(chara->get_master()->get_target());
        return SUCCESS;
    }
    return FAILURE;
}

GetMTarget* GetMTarget::clone()
{
    return new GetMTarget(*this);
}

int LowHealth::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    return (float)chara->get_cur_hp() < (.2 * (float)chara->get_max_hp());
}

LowHealth* LowHealth::clone()
{
    return new LowHealth(*this);
}

int NextTo::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    if(chara->get_target() == NULL)
    {
        return FAILURE;
    }
    return game->next_to_char(chara, chara->get_target());
}

NextTo* NextTo::clone()
{
    return new NextTo(*this);
}

int NextToM::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    if(chara->get_master() == NULL)
    {
        return FAILURE;
    }
    return game->next_to_char(chara, chara->get_master());
}

NextToM* NextToM::clone()
{
    return new NextToM(*this);
}

int HasHealth::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    if(chara->get_cur_hp() <= 0)
    {
        return FAILURE;
    }
    return SUCCESS;
}

HasHealth* HasHealth::clone()
{
    return new HasHealth(*this);
}

int MHealthChange::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    if(chara->master_health_changed())
    {
        chara->update_master_health();
        return SUCCESS;
    }
    else
    {
        return FAILURE;
    }
}

MHealthChange* MHealthChange::clone()
{
    return new MHealthChange(*this);
}


int ValidTarget::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    return game->valid_target(chara, chara->get_target());
}

ValidTarget* ValidTarget::clone()
{
    return new ValidTarget(*this);
}

int InWorld::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    if(game->character_in_range(chara))
    {
        return SUCCESS;
    }
    else
    {
        return FAILURE;
    }
}

InWorld* InWorld::clone()
{
    return new InWorld(*this);
}

//-------------------------ACTION NODES------------------------------//

int MoveTowards::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    if(chara->get_target() == NULL)
    {
        return FAILURE;
    }
    IntPoint goal_coords = chara->get_target()->get_coords();
    IntPoint goal_chunk = chara->get_target()->get_chunk();
    return game->move_to_point(chara, goal_coords, goal_chunk);
}

MoveTowards* MoveTowards::clone()
{
    return new MoveTowards(*this);
}


int MoveTowardsM::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    if(chara->get_master() == NULL)
    {
        return FAILURE;
    }
    IntPoint goal_coords = chara->get_master()->get_coords();
    IntPoint goal_chunk = chara->get_master()->get_chunk();
    return game->move_to_point(chara, goal_coords, goal_chunk);
}

MoveTowardsM* MoveTowardsM::clone()
{
    return new MoveTowardsM(*this);
}

int MoveAway::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    if(chara->get_target() == NULL)
    {
        return FAILURE;
    }
    IntPoint goal_coords = chara->get_target()->get_coords();
    IntPoint goal_chunk = chara->get_target()->get_chunk();
    return game->run_away(actor.get_character(), goal_coords, goal_chunk);
}

MoveAway* MoveAway::clone()
{
    return new MoveAway(*this);
}

int Attack::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    if(chara->get_target() == NULL)
    {
        return FAILURE;
    }
    return game->attack_char(chara, chara->get_target());
}

Attack* Attack::clone()
{
    return new Attack(*this);
}

int Wander::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    game->wander(chara);
    return RUNNING;
}

Wander* Wander::clone()
{
    return new Wander(*this);
}


int FreakOut::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    game->wander(chara);
    return RUNNING;
}

FreakOut* FreakOut::clone()
{
    return new FreakOut(*this);
}

int Die::tick(BActor actor, Game* game)
{
    return DEAD;
}

Die* Die::clone()
{
    return new Die(*this);
}

int TurnToward::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    game->turn_character(chara, chara->get_target());
    return SUCCESS;
}

TurnToward* TurnToward::clone()
{
    return new TurnToward(*this);
}

int TurnTowardM::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    game->turn_character(chara, chara->get_master());
    return SUCCESS;
}

TurnTowardM* TurnTowardM::clone()
{
    return new TurnTowardM(*this);
}

int TurnAway::tick(BActor actor, Game* game)
{
    Character* chara = actor.get_character();
    game->turn_away(chara, chara->get_target());
    return SUCCESS;
}

TurnAway* TurnAway::clone()
{
    return new TurnAway(*this);
}
