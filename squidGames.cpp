#include "squidGames.h"

void SquidGames::mergeGroups(int GroupID1, int GroupID2)
{
    if (GroupID1 <= 0 || GroupID2 <= 0 || GroupID1 > num_groups || GroupID2 > num_groups)
    {
        throw InvalidError();
    }

    int first_group = Groups.find(GroupID1);
    int second_group = Groups.find(GroupID2);
    Groups.unite(first_group, second_group);
}

void SquidGames::addPlayer(int PlayerID, int GroupID, int score) // this dosnt take into account the super-tree
{
    Player* new_player = new Player(PlayerID, GroupID, score);
    Players.insert(PlayerID,new_player);
    int group_index = Groups.find(GroupID);
    RankTree* player_tree = Groups.getGroupTree(group_index);
    player_tree->getZero()[score-1]++;
}

static void checkScores(RankTree* tree, int level, int* scores, int scale)
{
    if (level != 0)
    {
        bool all_zero = true;
        for (int i = 0; i < scale; i++)
        {
            if (scores[i] != 0)
            {
                all_zero = false;
                break;
            }
        }
        if (all_zero)
        {
            tree->remove(level);
        }
    }
}

static void changeTree(RankTree* tree, int level, int scale, int score, int change_to_score)
{
    int* curr_scores = tree->get(level);
    if (curr_scores == nullptr)
    {
        int* new_scores = new int[scale]();
        new_scores[score - 1] += change_to_score;
        tree->append(level, new_scores);
    }
    else
    {
        curr_scores[score - 1] += change_to_score;
        tree->userUpdateTreeScores(level);
        if (change_to_score < 0)
        {
            checkScores(tree, level, curr_scores, scale);
        }
    }
}

void SquidGames::removePlayer(int PlayerID)
{
    if (PlayerID <= 0)
    {
        throw InvalidError();
    }
    Player* curr_player = Players.getData(PlayerID);
    if (curr_player == nullptr)
    {
        throw Failure();
    }
    int group_of_group = Groups.find(curr_player->GroupId);
    RankTree* curr_tree = Groups.getGroupTree(group_of_group);

    changeTree(curr_tree, curr_player->Level, scale, curr_player->score, -1);
    changeTree(&Levels, curr_player->Level, scale, curr_player->score, -1);

    Players.remove(PlayerID);
}

void SquidGames::increasePlayerIDLevel(int PlayerID, int LevelIncrease)
{
    if (PlayerID <= 0 || LevelIncrease <= 0)
    {
        throw InvalidError();
    }
    Player* curr_player = Players.getData(PlayerID);
    if (curr_player == nullptr)
    {
        throw Failure();
    }
    int group_of_group = Groups.find(curr_player->GroupId);
    RankTree* curr_tree = Groups.getGroupTree(group_of_group);
    changeTree(curr_tree, curr_player->Level, scale, curr_player->score, -1);
    changeTree(&Levels, curr_player->Level, scale, curr_player->score, -1);

    curr_player->Level += LevelIncrease;

    changeTree(curr_tree, curr_player->Level, scale, curr_player->score, +1);
    changeTree(&Levels, curr_player->Level, scale, curr_player->score, +1);
}

void SquidGames::averageHighestPlayerLevelByGroup(int GroupID, int m, double *avgLevel)
{
    if (avgLevel == nullptr || GroupID > num_groups || GroupID < 0 || m <= 0)
    {
        throw InvalidError();
    }

    RankTree* curr_tree = nullptr;
    if (GroupID > 0)
    {
        int group_of_group = Groups.find(GroupID);
        RankTree* curr_tree = Groups.getGroupTree(group_of_group);
    }
    else
    {
        curr_tree = &Levels;
    }

    if (m > curr_tree->getSize())
    {
        throw Failure();
    }
    ////I think we need to add info of regular rank tree for this - so we can know when we have m players

}