/**
 *  @file AI_DEFS.CPP
 *  @author Seth Yoder, Michael Yoder
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

#include "ai_defs.h"

//Make one of these for EVERY TREE YOU MAKE.


//     DEATH TREE      //
/////////////////////////
//
//            Root^
//             |
//             |
//         +---+------+
//         |          |
//      ShouldDie*   Die
//         |
//         |
//         v
//     +---+----+
//     |        |
//     |        |
//  Inverter Inverter
//     |        +
//     |        |
//     v        v
//  InWorld  HasHealth
//
/////////////////////////////

BNode* ai::DeathNode()
{
    std::vector<BNode*> death_nodes;
    std::vector<BNode*> should_die_nodes;
    should_die_nodes.push_back(new InverterNode(new InWorld));
    should_die_nodes.push_back(new InverterNode(new HasHealth));

    PriorityNode* should_die = new PriorityNode(should_die_nodes);

    death_nodes.push_back(should_die);
    death_nodes.push_back(new Die);
    return new SequenceNode(death_nodes);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//              * = Priority
//              ^ = Sequence
//              } = Branching Conditional
//
//
//                                Root*
//                                 +
//                                 |
//       +----------------------------------------------------+
//       |                         |                          |
//       |                         |                          v
//    DeathNode                Attack^                    Wander
//                                 |
//                                 |
//                        +--------+------------------+
//                        |                           |
//                        |                           v
//                     GetTarget*                     }
//                        |                           +
//                        |                           |
//                 +------+------+          +-------------------------------+
//                 |             |          |         |                     |
//                 v             v          v         |                     |
//             HasValid   EnemyInRange  LowHealth   RunAway^             Attack^
//                                                    |                     |
//                                               +----+----+         +------+-------------+
//                                               |         |         |                    |
//                                               |         |         |                    }
//                                               v         v         v                    |
//                                            TurnAway Mo^eAway   TurnTowards    +----------------+
//                                                                               |        |       |
//                                                                               v        v       v
//                                                                            NextTo    Attack MoveTowards
//////////////////////////////////////////////////////////////////////////////////////////////////////////

BehaviorTree ai::GENERIC_AGGRESSIVE(Game* game)
{
    BranchingCondition* first = new BranchingCondition(new NextTo, new Attack, new MoveTowards);
    std::vector<BNode*> attack_seq_nodes;
    attack_seq_nodes.push_back(new TurnToward);
    attack_seq_nodes.push_back(first);
    SequenceNode* attack_seq = new SequenceNode(attack_seq_nodes);

    std::vector<BNode*> run_seq_nodes;
    run_seq_nodes.push_back(new TurnAway);
    run_seq_nodes.push_back(new MoveAway);
    SequenceNode* run_seq = new SequenceNode(run_seq_nodes);

    BranchingCondition* second = new BranchingCondition(new LowHealth, run_seq, attack_seq);

    std::vector<BNode*> has_target;
    has_target.push_back(new ValidTarget);
    has_target.push_back(new EnemyInRange);
    PriorityNode* target_node = new PriorityNode(has_target);

    std::vector<BNode*> attack_nodes;
    attack_nodes.push_back(target_node);
    attack_nodes.push_back(second);
    SequenceNode* attack = new SequenceNode(attack_nodes);

    //see above
    BNode* death = DeathNode();

    std::vector<BNode*> root;
    root.push_back(death);
    root.push_back(attack);
    root.push_back(new Wander);
    PriorityNode* root_node = new PriorityNode(root);


    BehaviorTree tree = BehaviorTree(root_node, game, 0);
    return tree;
}

///////////////////////////////////////////////////////////////////////////////
//
//                              Root*
//                               +
//                               |
//         +------------------------------------------------+
//         |                     |                          |
//         |                     v                          v
//      DeathNode              Flee^                     Wander
//                               +
//                               |
//                      +--------+------------+
//                      |                     |
//                      |                     |
//                   GetTarget*             RunAway^
//                      |                     |
//                      |                +----+----+
//               +------+------+         |         |
//               |             |         |         |
//               v             v         v         v
//           HasValid   EnemyInRange  TurnAway Mo^eAway
//
////////////////////////////////////////////////////////////////////////////////

BehaviorTree ai::GENERIC_PASSIVE(Game* game)
{

    std::vector<BNode*> run_seq_nodes;
    run_seq_nodes.push_back(new TurnAway);
    run_seq_nodes.push_back(new MoveAway);
    SequenceNode* run_seq = new SequenceNode(run_seq_nodes);

    std::vector<BNode*> has_target;
    has_target.push_back(new ValidTarget);
    has_target.push_back(new EnemyInRange);
    PriorityNode* target_node = new PriorityNode(has_target);

    std::vector<BNode*> flee_nodes;
    flee_nodes.push_back(target_node);
    flee_nodes.push_back(run_seq);
    SequenceNode* flee = new SequenceNode(flee_nodes);

    BNode* death = DeathNode();

    std::vector<BNode*> root;
    root.push_back(death);
    root.push_back(flee);
    root.push_back(new Wander);
    PriorityNode* root_node = new PriorityNode(root);

    BehaviorTree tree = BehaviorTree(root_node, game, 1);
    return tree;
}

/////////////////////////////////////////////////////////////////////////////////////
//                               Root*
//                                |
//  +------------------------------------------------------------------------+
//  |                             |                                          |
//  |                             |                                          v
//DeathNode                    Attack^                                      Follow^
//                                |                                          |
//             +-----------------------------+                     +---------+----------+
//             |         |                   |                     |                    |
//             |         |                   }                     |                    ^
//             |         |                   |                     v                    |
//    MHealthChange  GetMTarget    +---------+------------+     TurnTowardsM       +--------+
//                                 |         |            |                        |        |
//                                 v         v            v                        |        |
//                               HasValid  Attack      FreakOut                 Inverter    |
//                                           |                                     |        |
//                                           }                                     |        |
//                                           |                                     v        v
//                                  +----------------+                          NextToM  MoveTowardM
//                                  |        |       |
//                                  v        v       v
//                               NextTo Mo^eTowards Attack
//
////////////////////////////////////////////////////////////////////////////////////////////
//Note: M designates "Master" here.
BehaviorTree ai::FOLLOW(Game* game)
{
    //Follow
    std::vector<BNode*> follow_sub_nodes;
    follow_sub_nodes.push_back(new InverterNode(new NextToM));
    follow_sub_nodes.push_back(new MoveTowardsM);
    SequenceNode* follow_sub = new SequenceNode(follow_sub_nodes);

    std::vector<BNode*> follow_nodes;
    follow_nodes.push_back(new TurnTowardM);
    follow_nodes.push_back(follow_sub);
    SequenceNode* follow = new SequenceNode(follow_nodes);

    //Attack
    BranchingCondition *bcond = new BranchingCondition(new NextTo, new Attack, new MoveTowards);
    BranchingCondition *attack_branch = new BranchingCondition(new ValidTarget, bcond, new FreakOut);
    std::vector<BNode*> attack_nodes;
    attack_nodes.push_back(new MHealthChange);
    attack_nodes.push_back(new GetMTarget);
    attack_nodes.push_back(attack_branch);
    SequenceNode* attack = new SequenceNode(attack_nodes);


    //Death
    BNode* death = DeathNode();

    std::vector<BNode*> root;
    root.push_back(death);
    root.push_back(attack);
    root.push_back(follow);
    PriorityNode* root_node = new PriorityNode(root);

    BehaviorTree tree = BehaviorTree(root_node, game, 2);
    return tree;
}

//
/////////////////////////////////////////////////////////////////////////////////////
//                               Root*
//                                |
//  -------------------------------------------------------------------
//  |                              |
//  v                              v
// Death                        Wander
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////

BehaviorTree ai::NPC(Game* game)
{
    std::vector<BNode*> root;
    BNode* death = DeathNode();
    root.push_back(death);
    root.push_back(new Wander);
    PriorityNode* root_node = new PriorityNode(root);

    BehaviorTree tree = BehaviorTree(root_node, game, 3);
    return tree;
}
