#include "library1.h"
#include "GroupManager.h"

void *Init()
{
    try {
        GroupManager* DS = new GroupManager();
        return (void*)DS;
    } catch (std::bad_alloc&) {
        return NULL;
    }
}


StatusType AddGroup(void *DS, int GroupID)
{
    if (DS == nullptr) //need this?
    {
        return ALLOCATION_ERROR;
    }
    try {
        ((GroupManager*)DS)->AddGroup(GroupID);
    } catch(GroupManager::InvalidError&)
    {
        return INVALID_INPUT;
    } catch(GroupManager::Failure&)
    {
        return FAILURE;
    }
    return SUCCESS;
}


StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int Level)
{
    if (DS == nullptr) //need this?
    {
        return ALLOCATION_ERROR;
    }
    try {
        ((GroupManager*)DS)->AddPlayer(PlayerID, GroupID, Level);
    } catch(GroupManager::InvalidError&)
    {
        return INVALID_INPUT;
    } catch(GroupManager::Failure&)
    {
        return FAILURE;
    }
    return SUCCESS;
}


StatusType RemovePlayer(void *DS, int PlayerID)
{
    if (DS == nullptr) //need this?
    {
        return ALLOCATION_ERROR;
    }
    try {
        ((GroupManager*)DS)->RemovePlayer(PlayerID);
    } catch(GroupManager::InvalidError&)
    {
        return INVALID_INPUT;
    } catch(GroupManager::Failure&)
    {
        return FAILURE;
    }
    return SUCCESS;
}


StatusType ReplaceGroup(void *DS, int GroupID, int ReplacementID)
{
    if (DS == nullptr) //need this?
    {
        return ALLOCATION_ERROR;
    }
    try {
        ((GroupManager*)DS)->ReplaceGroup(GroupID, ReplacementID);
    } catch(GroupManager::InvalidError&)
    {
        return INVALID_INPUT;
    } catch(GroupManager::Failure&)
    {
        return FAILURE;
    }
    return SUCCESS;
}


StatusType IncreaseLevel(void *DS, int PlayerID, int LevelIncrease)
{
    if (DS == nullptr) //need this?
    {
        return ALLOCATION_ERROR;
    }
    try {
        ((GroupManager*)DS)->IncreaseLevel(PlayerID, LevelIncrease);
    } catch(GroupManager::InvalidError&)
    {
        return INVALID_INPUT;
    } catch(GroupManager::Failure&)
    {
        return FAILURE;
    }
    return SUCCESS;
}


StatusType GetHighestLevel(void *DS, int GroupID, int *PlayerID)
{
    if (DS == nullptr) //need this?
    {
        return ALLOCATION_ERROR;
    }
    try {
        ((GroupManager*)DS)->GetHighestLevel(GroupID, PlayerID);
    } catch(GroupManager::InvalidError&)
    {
        return INVALID_INPUT;
    } catch(GroupManager::Failure&)
    {
        return FAILURE;
    }
    return SUCCESS;
}


StatusType GetAllPlayersByLevel(void *DS, int GroupID, int **Players, int *numOfPlayers)
{
    if (DS == nullptr) //need this?
    {
        return ALLOCATION_ERROR;
    }
    try {
        ((GroupManager*)DS)->GetAllPlayersByLevel(GroupID, Players, numOfPlayers);
    } catch(GroupManager::InvalidError&)
    {
        return INVALID_INPUT;
    } catch(GroupManager::Failure&)
    {
        return FAILURE;
    }
    return SUCCESS;
}


StatusType GetGroupsHighestLevel(void *DS, int numOfGroups, int **Players)
{
    if (DS == nullptr) //need this?
    {
        return ALLOCATION_ERROR;
    }
    try {
        ((GroupManager*)DS)->GetGroupsHighestLevel(numOfGroups, Players);
    } catch(GroupManager::InvalidError&)
    {
        return INVALID_INPUT;
    } catch(GroupManager::Failure&)
    {
        return FAILURE;
    }
    return SUCCESS;
}


void Quit(void** DS) //not sure about how i did this
{
    if (*DS != nullptr)
    {
        delete (GroupManager*)*DS;
    }
    *DS = NULL;
}