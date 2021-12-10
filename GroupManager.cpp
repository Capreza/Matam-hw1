#include "GroupManager.h"

void GroupManager::AddGroup(int GroupId) //in the outer function i'll write - catch exception
// from avltree about doubles, and catch the exception i'm throwing here, and bad alloc
{
    if (GroupId <= 0)
    {
        throw InvalidError();
    }

    shared_ptr<Group> new_group = shared_ptr<Group> (new Group(GroupId));
    shared_ptr<Group> temp_group = GroupsTree.get(*new_group);
    if (temp_group != nullptr)
    {
        throw Failure();
    }
    GroupsTree.append(new_group);
}


void GroupManager::AddPlayer(int PlayerId, int GroupId, int Level)
{
    if (PlayerId <= 0 || GroupId <= 0 || Level < 0)
    {
        throw InvalidError();
    }

    shared_ptr<Player> new_player = shared_ptr<Player> (new Player(PlayerId, GroupId, Level));
    if (PlayersByLevelTree.get(*new_player) != nullptr)
    {
        throw Failure();
    }

    Group temp_group(GroupId);
    shared_ptr<Group> group_from_tree =NonEmptyGroupsTree.get(temp_group);

    //if group is empty or doesn't exist
    if (group_from_tree == nullptr)
    {
        group_from_tree = GroupsTree.get(temp_group);

        //if group doesn't exist
        if (group_from_tree == nullptr)
        {
            throw Failure();
        }

        //if group is empty
        NonEmptyGroupsTree.append(group_from_tree);
    }

    //adding player to group player tree
    (group_from_tree->PlayerTree).append(new_player);
    if ((group_from_tree->PlayerTree).HighestLevelInGroup == nullptr ||
        *(group_from_tree->PlayerTree).HighestLevelInGroup < *new_player)
    {
        (group_from_tree->PlayerTree).HighestLevelInGroup = new_player;
    }

    if (NonEmptyGroupsTree.HighestLevelOverall == nullptr ||
        *NonEmptyGroupsTree.HighestLevelOverall < *new_player)
    {
        NonEmptyGroupsTree.HighestLevelOverall = new_player;
    }

    //adding player to player trees
    PlayersByLevelTree.append(new_player);
}


void GroupManager::RemovePlayer(int PlayerId)
{
    if (PlayerId <= 0)
    {
        throw InvalidError();
    }

    Player player_to_remove(PlayerId, 0, 0);
    shared_ptr<Player> player_from_tree = PlayersByLevelTree.get(player_to_remove);
    if (player_from_tree == nullptr)
    {
        throw Failure();
    }

    shared_ptr<Group> curr_group( new Group((*player_from_tree).GroupId));
    curr_group = NonEmptyGroupsTree.get(*curr_group);
    PlayersByLevelTree.remove(player_from_tree);
    curr_group->PlayerTree.remove(player_from_tree);
    if (curr_group->PlayerTree.isEmpty())
    {
        NonEmptyGroupsTree.remove(curr_group);
    }
    if (*curr_group->PlayerTree.HighestLevelInGroup == *player_from_tree)
    {
        curr_group->PlayerTree.HighestLevelInGroup = curr_group->PlayerTree.getMaxNodeData();
    }
    if (*NonEmptyGroupsTree.HighestLevelOverall == *player_from_tree)
    {
        NonEmptyGroupsTree.HighestLevelOverall = PlayersByLevelTree.getMaxNodeData();
    }
}


void arrayMerge(shared_ptr<Player>* dest_arr, shared_ptr<Player>* arr1, int size1,
                shared_ptr<Player>* arr2, int size2)
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


void GroupManager::ReplaceGroup(int GroupId, int ReplacementId)
{
    if (GroupId <= 0 || ReplacementId <= 0 || GroupId == ReplacementId)
    {
        throw InvalidError();
    }

    shared_ptr<Group> group_to_delete = shared_ptr<Group> (new Group(GroupId));
    group_to_delete =  GroupsTree.get(*group_to_delete);
    shared_ptr<Group> replacement_group = shared_ptr<Group> (new Group(ReplacementId));
    replacement_group = GroupsTree.get(*replacement_group);
    if (group_to_delete == nullptr || replacement_group == nullptr)
    {
        throw Failure();
    }

    int group_to_delete_size = group_to_delete->PlayerTree.getSize();
    int replacement_group_size = replacement_group->PlayerTree.getSize();

    shared_ptr<Player>* players_second_group;
    shared_ptr<Player> from_replace_highest;
    if (replacement_group_size == 0)
    {
        if (group_to_delete_size == 0)
        {
            GroupsTree.remove(group_to_delete);
            return;
        }
        players_second_group = nullptr;
        from_replace_highest = nullptr;
    }
    else
    {
        from_replace_highest = replacement_group->PlayerTree.HighestLevelInGroup;
        players_second_group = new shared_ptr<Player>[replacement_group_size];
        NonEmptyGroupsTree.remove(replacement_group);
    }
    shared_ptr<Player> from_delete_highest = group_to_delete->PlayerTree.HighestLevelInGroup;
    shared_ptr<Player>* players_first_group = new shared_ptr<Player>[group_to_delete_size];
    group_to_delete->playerTreeToArray(players_first_group);
    for (int i = 0; i < group_to_delete_size; i++)
    {
        players_first_group[i]->GroupId = ReplacementId;
    }
    replacement_group->playerTreeToArray(players_second_group);

    int final_size = group_to_delete_size + replacement_group_size;
    shared_ptr<Player>* all_players = new shared_ptr<Player>[final_size];
    arrayMerge(all_players, players_first_group, group_to_delete_size,
               players_second_group, replacement_group_size);
    delete[] players_first_group;
    delete[] players_second_group;

    PlayerPerGroupAVLTree new_player_tree(buildAndFillTree(all_players, final_size));
    GroupsTree.remove(replacement_group);
    replacement_group = shared_ptr<Group> (new Group(ReplacementId, new_player_tree));
    GroupsTree.append(replacement_group);
    NonEmptyGroupsTree.append(replacement_group);
    GroupsTree.remove(group_to_delete);
    NonEmptyGroupsTree.remove(group_to_delete);
    if (from_replace_highest && from_delete_highest < from_replace_highest)
    {
        replacement_group->PlayerTree.HighestLevelInGroup = from_replace_highest;
    }
    else
    {
        replacement_group->PlayerTree.HighestLevelInGroup = from_delete_highest;
    }
}


void GroupManager::IncreaseLevel(int PlayerId, int LevelIncrease)
{
    if (PlayerId <= 0 || LevelIncrease <= 0)
    {
        throw InvalidError();
    }

    shared_ptr<Player> player_to_increase = shared_ptr<Player> (new Player(PlayerId, 0, 0));
    player_to_increase = PlayersByLevelTree.get(*player_to_increase);
    if (player_to_increase == nullptr)
    {
        throw Failure();
    }

    int curr_group_id = player_to_increase->GroupId;
    shared_ptr<Group> temp_group = shared_ptr<Group> (new Group(curr_group_id));
    temp_group = NonEmptyGroupsTree.get(*temp_group);
    PlayersByLevelTree.remove(player_to_increase);
    temp_group->PlayerTree.remove(player_to_increase);

    player_to_increase->Level += LevelIncrease;
    PlayersByLevelTree.append(player_to_increase);
    temp_group->PlayerTree.append(player_to_increase);

    if (NonEmptyGroupsTree.HighestLevelOverall == nullptr ||
        *NonEmptyGroupsTree.HighestLevelOverall < *player_to_increase)
    {
        NonEmptyGroupsTree.HighestLevelOverall = player_to_increase;
    }
    if (temp_group->PlayerTree.HighestLevelInGroup == nullptr ||
        *temp_group->PlayerTree.HighestLevelInGroup < *player_to_increase)
    {
        temp_group->PlayerTree.HighestLevelInGroup = player_to_increase;
    }
}


void GroupManager::GetHighestLevel(int GroupId, int* player_id)
{
    if (GroupId == 0 || player_id == nullptr)
    {
        throw InvalidError();
    }

    if (GroupId > 0)
    {
        shared_ptr<Group> temp_group = shared_ptr<Group> (new Group(GroupId));
        temp_group = GroupsTree.get(*temp_group);
        if (temp_group == nullptr)
        {
            throw Failure();
        }
        if (temp_group->PlayerTree.getSize() == 0)
        {
            *player_id = 1;
        }
        else
        {
            *player_id = temp_group->PlayerTree.HighestLevelInGroup->PlayerId;
        }
    }
    else
    {
        if (PlayersByLevelTree.getSize() == 0)
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
        shared_ptr<Group> temp_group = shared_ptr<Group> (new Group(GroupId));
        temp_group = GroupsTree.get(*temp_group);
        if (temp_group == nullptr)
        {
            throw Failure();
        }

        int num_players_in_group = temp_group->PlayerTree.getSize();
        if (num_players_in_group == 0)
        {
            *num_of_players = 1;
            arr = nullptr;
        }
        else
        {
            shared_ptr<Player>* players = new shared_ptr<Player>[num_players_in_group];
            temp_group->PlayerTree.inOrderRev(players);
            int* ret_arr = (int*)malloc(sizeof(*ret_arr) * num_players_in_group);
            if (ret_arr == NULL)
            {
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
        int num_players = PlayersByLevelTree.getSize();
        if (num_players == 0)
        {
            *num_of_players = 0;
            arr = nullptr;
        }

        shared_ptr<Player>* players = new shared_ptr<Player>[num_players];
        PlayersByLevelTree.inOrderRev(players);
        int* ret_arr = (int*)malloc(sizeof(*ret_arr) * num_players);
        if (ret_arr == NULL)
        {
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

    shared_ptr<Group>* groups_arr = new shared_ptr<Group>[numOfGroups];
    int* ret_arr = (int*)malloc(sizeof(*ret_arr) * numOfGroups);
    if (ret_arr == NULL)
    {
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