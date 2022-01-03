#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
    int PlayerId;
    int GroupId;
    int Level;
    int score;

    //Player() : PlayerId(0), GroupId(0), Level(0), score(0) {}; //need? don't need?
    Player(int PlayerId, int GroupId, int score) : PlayerId(PlayerId), GroupId(GroupId), Level(0), score(score) {};
    bool operator < (Player const& other) const //need to change this?
    {
        return PlayerId < other.PlayerId;
    }
    bool operator == (Player const& other) const //need to change this?
    {
        return PlayerId == other.PlayerId;
    }
    bool operator > (Player const& other) const //need to change this?
    {
        return PlayerId > other.PlayerId;
    }
};


#endif