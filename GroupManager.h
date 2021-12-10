#ifndef HW1_GROUPMANAGER_H
#define HW1_GROUPMANAGER_H
#include "trees.h"
#include <exception>


class GroupManager
{
private:
    AllGroupAVLTree GroupsTree;
    AVLTree<Player> PlayersByLevelTree;
    SubPlayerTree PlayersByIdTree;
    GroupAVLTree NonEmptyGroupsTree;

public:
    class InvalidError : std::exception {};
    class Failure : std::exception {};

    void AddGroup(int GroupId);
    void AddPlayer(int PlayerId, int GroupId, int Level);
    void RemovePlayer(int PlayerId);
    void ReplaceGroup(int GroupId, int ReplacementId);
    void IncreaseLevel(int PlayerId, int LevelIncrease);
    void GetHighestLevel(int GroupId, int* player_id);
    void GetAllPlayersByLevel(int GroupId, int** arr, int* num_of_players);
    void GetGroupsHighestLevel(int numOfGroups, int** arr);
};

#endif