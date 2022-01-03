#include "squidGames.h"

void SquidGames::mergeGroups(int GroupID1, int GroupID2)
{

}

void SquidGames::addPlayer(int PlayerID, int GroupID, int score) // this dosnt take into account the super-tree
{
    Player* new_player = new Player(PlayerID, GroupID, score);
    Players.insert(PlayerID,new_player);
    int group_index = Groups.find(GroupID);
    RankTree* player_tree = Groups.getGroupTree(group_index);
    player_tree->getZero()[score-1]++;
}
