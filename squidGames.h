#ifndef SQUIDGAMES_H
#define SQUIDGAMES_H
#include "unionFind.h"
#include "hashtable.h"
#include <exception>

class SquidGames
{
private:
    UnionFind Groups;
    Hashtable Players;
    RankTree Levels;
    int scale;
    int num_groups; //not necessary but maybe convenient

public:
    class InvalidError : std::exception {};
    class Failure : std::exception {};

    SquidGames(int k, int scale) : Groups(k, scale), Players(), Levels(scale), scale(scale), num_groups(k) {};

    void mergeGroups(int GroupID1, int GroupID2);
    void addPlayer(int PlayerID, int GroupID, int score);
    void removePlayer(int PlayerID);
    void increasePlayerIDLevel(int PlayerID, int LevelIncrease);
    void changePlayerIDScore(int PlayerID, int NewScore);
    void getPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel, double* players);
    void averageHighestPlayerLevelByGroup(int GroupID, int m, double* avgLevel);
};


#endif //SQUIDGAMES_H
