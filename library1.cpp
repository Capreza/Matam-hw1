#include "library2.h"
#include "squidGames.h"

void *Init(int k, int scale)
{
    if (k <= 0 || scale < 0 || scale > 200)
    {
        return NULL;
    }
    try {
        SquidGames* DS = new SquidGames(k, scale);
        return (void*)DS;
    } catch (std::bad_alloc&) {
        return NULL;
    }
}


StatusType MergeGroups(void *DS, int GroupID1, int GroupID2)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try {
        ((SquidGames*)DS)->mergeGroups(GroupID1, GroupID2);
    } catch(SquidGames::InvalidError& e)
    {
        return INVALID_INPUT;
    } catch(SquidGames::Failure& e)
    {
        return FAILURE;
    } catch(std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}


StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try {
        ((SquidGames*)DS)->addPlayer(PlayerID, GroupID, score);
    } catch(SquidGames::InvalidError& e)
    {
        return INVALID_INPUT;
    } catch(SquidGames::Failure& e)
    {
        return FAILURE;
    } catch(std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}


StatusType RemovePlayer(void *DS, int PlayerID)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try {
        ((SquidGames*)DS)->removePlayer(PlayerID);
    } catch(SquidGames::InvalidError& e)
    {
        return INVALID_INPUT;
    } catch(SquidGames::Failure& e)
    {
        return FAILURE;
    } catch(std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}


StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try {
        ((SquidGames*)DS)->increasePlayerIDLevel(PlayerID, LevelIncrease);
    } catch(SquidGames::InvalidError& e)
    {
        return INVALID_INPUT;
    } catch(SquidGames::Failure& e)
    {
        return FAILURE;
    } catch(std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}


StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try {
        ((SquidGames*)DS)->changePlayerIDScore(PlayerID, NewScore);
    } catch(SquidGames::InvalidError& e)
    {
        return INVALID_INPUT;
    } catch(SquidGames::Failure& e)
    {
        return FAILURE;
    } catch(std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}


StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                                double * players)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try {
        ((SquidGames*)DS)->getPercentOfPlayersWithScoreInBounds(GroupID, score, lowerLevel, higherLevel, players);
    } catch(SquidGames::InvalidError& e)
    {
        return INVALID_INPUT;
    } catch(SquidGames::Failure& e)
    {
        return FAILURE;
    } catch(std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}


StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try {
        ((SquidGames*)DS)->averageHighestPlayerLevelByGroup(GroupID, m, level);
    } catch(SquidGames::InvalidError& e)
    {
        return INVALID_INPUT;
    } catch(SquidGames::Failure& e)
    {
        return FAILURE;
    } catch(std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
                           int * LowerBoundPlayers, int * HigherBoundPlayers)
{
    return SUCCESS;
}

void Quit(void** DS)
{
    if (*DS != nullptr)
    {
        delete (SquidGames*)*DS;
    }
    *DS = NULL;
}