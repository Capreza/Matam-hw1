#include "GroupManager.h"

void GroupManager::AddGroup(int GroupId) //in the outer function i'll write - catch exception
// from avltree about doubles, and catch the exception i'm throwing here, and bad alloc
{
    if (GroupId <= 0)
    {
        throw InvalidError();
    }

    Group* new_group_ptr = new Group(GroupId);
    Group* temp_group = GroupsTree.get(new_group_ptr);
    if (temp_group != nullptr)
    {
        delete new_group_ptr;
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
        delete player_for_id;
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
            delete player_for_level;
            delete player_for_id;
            throw Failure();
        }

        //if group is empty
        NonEmptyGroupsTree.append(group_from_tree);
    }

    //adding player to group player tree
    (group_from_tree->PlayerTree).append(player_for_level);
    if ((group_from_tree->PlayerTree).HighestLevelInGroup == nullptr ||
            *(group_from_tree->PlayerTree).HighestLevelInGroup < *player_for_level)
    {
        (group_from_tree->PlayerTree).HighestLevelInGroup = player_for_level;
    }
    if (NonEmptyGroupsTree.HighestLevelOverall == nullptr ||
        *NonEmptyGroupsTree.HighestLevelOverall < *player_for_level)
    {
        NonEmptyGroupsTree.HighestLevelOverall = player_for_level;
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
    curr_group_ptr = NonEmptyGroupsTree.get(curr_group_ptr);
    Player player_for_level(PlayerId, (*player_from_tree).GroupId, (*player_from_tree).Level);
    Player* player_for_level_ptr = &player_for_level;
    PlayersByIdTree.safeRemove(player_from_tree);//need to fix this
    curr_group_ptr->PlayerTree.safeRemove(player_for_level_ptr);
    curr_group_ptr->PlayerTree.HighestLevelInGroup = curr_group_ptr->PlayerTree.getMaxNodeData();

    PlayersByLevelTree.remove(player_for_level_ptr);
    NonEmptyGroupsTree.HighestLevelOverall = PlayersByLevelTree.getMaxNodeData();
    if (curr_group_ptr->PlayerTree.isEmpty())
    {
        NonEmptyGroupsTree.safeRemove(player_from_tree->GroupId,curr_group_ptr);
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


    Group replacement_group(ReplacementId);
    Group* replacement_group_ptr =  &replacement_group;
    replacement_group_ptr = GroupsTree.get(replacement_group_ptr);

    if (group_to_delete_ptr == nullptr || replacement_group_ptr == nullptr) {
        throw Failure();
    }


    int group_to_delete_size = (*group_to_delete_ptr).PlayerTree.getSize();
    int replacement_group_size = (*replacement_group_ptr).PlayerTree.getSize();

    Player** players_second_group;
    Player* from_replace_highest;
    if (replacement_group_size == 0) {
        if (group_to_delete_size == 0) {
            GroupsTree.remove(group_to_delete_ptr);
            return;
        }
        players_second_group = nullptr;
        from_replace_highest = nullptr;
    }
    else
    {
        players_second_group = new Player *[replacement_group_size];
        from_replace_highest = replacement_group_ptr->PlayerTree.HighestLevelInGroup;
    }

    Player **players_first_group = new Player *[group_to_delete_size];
    Player* from_delete_highest = group_to_delete_ptr->PlayerTree.HighestLevelInGroup;

    group_to_delete_ptr->playerTreeToArray(players_first_group);
    for (int i = 0; i < group_to_delete_size; i++)
    {
        players_first_group[i]->GroupId = ReplacementId;
    }
    replacement_group_ptr->playerTreeToArray(players_second_group);


    int final_size = group_to_delete_size + replacement_group_size;
    Player **all_players = new Player *[final_size];
    arrayMerge(all_players, players_first_group, group_to_delete_size,
               players_second_group, replacement_group_size);
    delete[] players_first_group;
    delete[] players_second_group;

    replacement_group_ptr->PlayerTree.buildAndFillTree(all_players, final_size); //maybe need to safe remove each player?
    replacement_group_ptr->PlayerTree.HighestLevelInGroup = replacement_group_ptr->PlayerTree.getMaxNodeData();
    NonEmptyGroupsTree.safeRemove(GroupId,&group_to_delete);


    GroupsTree.safeRemove(group_to_delete_ptr);

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
    Player player_for_level(PlayerId, 0, player_for_id_ptr->Level);
    Player* player_for_level_ptr = &player_for_level;
    player_for_level_ptr = PlayersByLevelTree.get(player_for_level_ptr);

    int new_level = player_for_level_ptr->Level + LevelIncrease;
    int curr_group_id = player_for_level_ptr->GroupId;
    Group temp_group(curr_group_id);
    Group* temp_group_ptr = &temp_group;
    temp_group_ptr = NonEmptyGroupsTree.get(temp_group_ptr);
    PlayersByIdTree.remove(player_for_id_ptr);
    SubPlayer* new_player_id = new SubPlayer(PlayerId, curr_group_id, new_level);
    PlayersByIdTree.append(new_player_id);

    temp_group_ptr->PlayerTree.safeRemove(player_for_level_ptr);
    PlayersByLevelTree.remove(player_for_level_ptr);
    Player* new_player_level = new Player(PlayerId, curr_group_id, new_level);
    PlayersByLevelTree.append(new_player_level);
    temp_group_ptr->PlayerTree.append(new_player_level);

    NonEmptyGroupsTree.HighestLevelOverall = PlayersByLevelTree.getMaxNodeData();
    temp_group_ptr->PlayerTree.HighestLevelInGroup = temp_group_ptr->PlayerTree.getMaxNodeData();
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


void GroupManager::GetAllPlayersByLevel(int GroupId, int** arr, int* num_of_players)
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
            arr = nullptr;
        }
        else
        {
            Player** players = new Player*[num_players_in_group];
            temp_group_ptr->PlayerTree.inOrderRev(players);
            int* ret_arr = (int*)malloc(sizeof(*ret_arr) * num_players_in_group);
            if (ret_arr == NULL)
            {
                delete[] players;
                throw std::bad_alloc();
            }
            for (int i = 0; i < num_players_in_group; i++)
            {
                ret_arr[i] = players[i]->PlayerId;
            }

            delete[] players;
            *num_of_players = num_players_in_group;
            *arr = ret_arr;
        }
    }
    else
    {
        int num_players = PlayersByIdTree.getSize();
        if (num_players == 0)
        {
            *num_of_players = 0;
            arr = nullptr;
        }

        Player** players = new Player*[num_players];
        PlayersByLevelTree.inOrderRev(players);
        int* ret_arr = (int*)malloc(sizeof(*ret_arr) * num_players);
        if (ret_arr == NULL)
        {
            delete[] players;
            throw std::bad_alloc();
        }
        for (int i = 0; i < num_players; i++)
        {
            ret_arr[i] = players[i]->PlayerId;
        }

        delete[] players;
        *num_of_players = num_players;
        *arr = ret_arr;
    }
}


void GroupManager::GetGroupsHighestLevel(int numOfGroups, int** arr)
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
    int* ret_arr = (int*)malloc(sizeof(*ret_arr) * numOfGroups);
    if (ret_arr == NULL)
    {
        delete groups_arr;
        throw std::bad_alloc();
    }
    NonEmptyGroupsTree.inOrder(groups_arr, numOfGroups);
    for (int i = 0; i < numOfGroups; i++)
    {
        ret_arr[i] = groups_arr[i]->PlayerTree.HighestLevelInGroup->PlayerId;
    }
    delete[] groups_arr;

    *arr = ret_arr;
}