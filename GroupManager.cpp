#include "GroupManager.h"

void GroupManager::AddGroup(int GroupId) //in the outer function i'll write - catch exception
// from avltree about doubles, and catch the exception i'm throwing here, and bad alloc
{
    if (GroupId <= 0)
    {
        throw InvalidError();
    }
////I stopped here, other version with new is in notepad, try to see why this doesn't work
    Group new_group(GroupId);
    Group* new_group_ptr = new Group(GroupId);
    Group* temp_group = GroupsTree.get(new_group_ptr);
    if (temp_group != nullptr)
    {
        throw Failure();
    }
    GroupsTree.append(new_group_ptr);
}


void GroupManager::AddPlayer(int PlayerId, int GroupId, int Level)
{
    if (PlayerId <= 0 || GroupId <= 0 || Level < 0)
    {
        throw InvalidError();
    }

    SubPlayer* player_for_id = new SubPlayer(PlayerId, GroupId, Level);
    if (PlayersByIdTree.get(player_for_id) != nullptr) //maybe doing this while trying to insert?
    {
        throw Failure();
    }

    Player* player_for_level =new Player(PlayerId, GroupId, Level);

    Group temp_group(GroupId);
    Group* group_from_tree =NonEmptyGroupsTree.get(&temp_group);

    //if group is empty or doesn't exist
    if (group_from_tree == nullptr)
    {
        group_from_tree = GroupsTree.get(&temp_group);

        //if group doesn't exist
        if (group_from_tree == nullptr)
        {
            throw Failure();
        }

        //if group is empty
        NonEmptyGroupsTree.append(group_from_tree);
    }

    //adding player to group player tree
    (group_from_tree->PlayerTree).append(player_for_level);
    if ((group_from_tree->PlayerTree).HighestLevelInGroup == nullptr ||
        ((group_from_tree->PlayerTree).HighestLevelInGroup)->Level < player_for_level->Level)
    {
        (group_from_tree->PlayerTree).HighestLevelInGroup = group_from_tree->PlayerTree.get(player_for_level);
    }

    //adding player to player trees
    PlayersByIdTree.append(player_for_id);
    PlayersByLevelTree.append(player_for_level);
}


void GroupManager::RemovePlayer(int PlayerId)
{
    if (PlayerId <= 0)
    {
        throw InvalidError();
    }

    SubPlayer player_for_id(PlayerId, 0, 0);
    SubPlayer* player_for_id_ptr = &player_for_id;
    SubPlayer* player_from_tree = PlayersByIdTree.get(player_for_id_ptr);
    if (player_from_tree == nullptr)
    {
        throw Failure();
    }

    Group curr_group((*player_from_tree).GroupId);
    Group* curr_group_ptr =&curr_group;
    curr_group = *(NonEmptyGroupsTree.get(curr_group_ptr));
    Player player_for_level(PlayerId, (*player_from_tree).GroupId, (*player_from_tree).Level);
    Player* player_for_level_ptr = &player_for_level;
    PlayersByIdTree.remove(player_from_tree);
    PlayersByLevelTree.remove(player_for_level_ptr);
    curr_group.PlayerTree.remove(player_for_level_ptr);
    if (curr_group.PlayerTree.isEmpty())
    {
        NonEmptyGroupsTree.remove(curr_group_ptr);
    }
}


void arrayMerge(Player** dest_arr, Player** arr1, int size1, Player** arr2, int size2)
{
    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2)
    {
        if (*(arr1[i]) < *(arr2[j]))
        {
            dest_arr[k] = arr1[i++];
        }
        else
        {
            dest_arr[k] = arr2[j++];
        }
        k++;
    }
    while (i < size1)
    {
        dest_arr[k++] = arr1[i++];
    }
    while (j < size2)
    {
        dest_arr[k++] = arr2[j++];
    }
}


void GroupManager::ReplaceGroup(int GroupId, int ReplacementId) {
    if (GroupId <= 0 || ReplacementId <= 0 || GroupId == ReplacementId) {
        throw InvalidError();
    }

    Group group_to_delete(GroupId);
    Group* group_to_delete_ptr = &group_to_delete;
    group_to_delete_ptr = GroupsTree.get(group_to_delete_ptr);

    group_to_delete = *group_to_delete_ptr;

    Group replacement_group(ReplacementId);
    Group* replacement_group_ptr =  &replacement_group;
    replacement_group_ptr = GroupsTree.get(replacement_group_ptr);

    if (group_to_delete_ptr == nullptr || replacement_group_ptr == nullptr) {
        throw Failure();
    }

    int group_to_delete_size = (*group_to_delete_ptr).PlayerTree.getSize();
    int replacement_group_size = (*replacement_group_ptr).PlayerTree.getSize();

    if (replacement_group_size == 0) {
        if (group_to_delete_size != 0) {
            replacement_group_ptr->PlayerTree = group_to_delete_ptr->PlayerTree;//will this cause memory leak?
        }
        GroupsTree.remove(group_to_delete_ptr);
        return;
    }

    Player **players_first_group = new Player *[group_to_delete_size];
    Player **players_second_group = new Player *[replacement_group_size];
    (*group_to_delete_ptr).playerTreeToArray(players_first_group);
    (*replacement_group_ptr).playerTreeToArray(players_second_group);

    int final_size = group_to_delete_size + replacement_group_size;
    Player **all_players = new Player *[final_size];
    arrayMerge(all_players, players_first_group, group_to_delete_size,
               players_second_group, replacement_group_size);
    delete[] players_first_group;
    delete[] players_second_group;

    PlayerPerGroupAVLTree new_player_tree(buildAndFillTree(all_players, final_size));
    replacement_group_ptr->PlayerTree = new_player_tree; //will this cause memory leak?
    new_player_tree.HighestLevelInGroup = new_player_tree.getMaxNodeData();
    GroupsTree.remove(group_to_delete_ptr);

    //there might be a mem leak here since nonemptytree dosnt remove the group to delete, not sure
}


void GroupManager::IncreaseLevel(int PlayerId, int LevelIncrease)
{
    if (PlayerId <= 0 || LevelIncrease <= 0)
    {
        throw InvalidError();
    }

    SubPlayer player_for_id(PlayerId, 0, 0);
    SubPlayer* player_for_id_ptr = &player_for_id;
    player_for_id_ptr = PlayersByIdTree.get(player_for_id_ptr);
    if (player_for_id_ptr == nullptr)
    {
        throw Failure();
    }
    player_for_id_ptr->Level += LevelIncrease;

    int curr_group_id = player_for_id_ptr->GroupId;
    Group temp_group(curr_group_id);
    Group* temp_group_ptr = &temp_group;
    temp_group_ptr = NonEmptyGroupsTree.get(temp_group_ptr);
    Player player_for_level(PlayerId, curr_group_id, player_for_id_ptr->Level);
    Player* player_for_level_ptr = &player_for_level;
    PlayersByLevelTree.remove(player_for_level_ptr);
    temp_group_ptr->PlayerTree.remove(player_for_level_ptr);

    player_for_level.Level += LevelIncrease;
    PlayersByLevelTree.append(player_for_level_ptr);
    temp_group_ptr->PlayerTree.append(player_for_level_ptr);
}


void GroupManager::GetHighestLevel(int GroupId, int* player_id)
{
    if (GroupId == 0 || player_id == nullptr)
    {
        throw InvalidError();
    }

    if (GroupId > 0)
    {
        Group temp_group(GroupId);
        Group* temp_group_ptr = &temp_group;
        temp_group_ptr = GroupsTree.get(temp_group_ptr);
        if (temp_group_ptr == nullptr)
        {
            throw Failure();
        }
        if (temp_group_ptr->PlayerTree.getSize() == 0)
        {
            *player_id = 1;
        }
        else
        {
            *player_id = temp_group_ptr->PlayerTree.HighestLevelInGroup->PlayerId;
        }
    }
    else
    {
        if (PlayersByIdTree.getSize() == 0)
        {
            *player_id = 1;
        }
        *player_id = (NonEmptyGroupsTree.HighestLevelOverall)->PlayerId;
    }
}


void GroupManager::GetAllPlayersByLevel(int GroupId, int** players, int* num_of_players)
{
    if (GroupId == 0 || num_of_players == nullptr)
    {
        throw InvalidError();
    }

    if (GroupId > 0)
    {
        Group temp_group(GroupId);
        Group* temp_group_ptr =&temp_group;
        temp_group_ptr = GroupsTree.get(temp_group_ptr);
        if (temp_group_ptr == nullptr)
        {
            throw Failure();
        }

        int num_players_in_group = temp_group_ptr->PlayerTree.getSize();
        if (num_players_in_group == 0)
        {
            *num_of_players = 1;
            players = nullptr;
        }
        else
        {
            Player** arr = new Player*[num_players_in_group];
            temp_group_ptr->PlayerTree.inOrderRev(arr);
            int** ret_arr = new int*[num_players_in_group];
            for (int i = 0; i < num_players_in_group; i++)
            {
                ret_arr[i] = &(arr[i]->PlayerId);
            }

            delete[] arr;
            *num_of_players = num_players_in_group;
            players = ret_arr;
        }
    }
    else
    {
        int num_players = PlayersByIdTree.getSize();
        if (num_players == 0)
        {
            *num_of_players = 0;
            players = nullptr;
        }

        Player** arr = new Player*[num_players];
        PlayersByLevelTree.inOrderRev(arr);
        int** ret_arr = new int*[num_players];
        for (int i = 0; i < num_players; i++)
        {
            ret_arr[i] = &(arr[i]->PlayerId);
        }

        delete[] arr;
        *num_of_players = num_players;
        players = ret_arr;
    }
}


void GroupManager::GetGroupsHighestLevel(int numOfGroups, int** &arr)
{
    if (numOfGroups < 1)
    {
        throw InvalidError();
    }
    if (numOfGroups > NonEmptyGroupsTree.getSize())
    {
        throw Failure();
    }

    Group** groups_arr = new Group*[numOfGroups];
    int* ret_arr = new int[numOfGroups];
    NonEmptyGroupsTree.inOrder(groups_arr, numOfGroups);
    for (int i = 0; i < numOfGroups; i++)
    {
        ret_arr[i] = groups_arr[i]->PlayerTree.HighestLevelInGroup->PlayerId;
    }
    delete[] groups_arr;

    *arr = ret_arr;
}