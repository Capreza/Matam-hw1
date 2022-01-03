#include "squidGames.h"

void SquidGames::mergeGroups(int GroupID1, int GroupID2)
{
    if (GroupID1 <= 0 || GroupID2 <= 0 || GroupID1 > num_groups || GroupID2 > num_groups)
    {
        throw InvalidError();
    }

    int first_group = Groups.find(GroupID1);
    int second_group = Groups.find(GroupID2);

}

