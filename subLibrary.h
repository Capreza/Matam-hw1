#ifndef SUBLIBRARY_H
#define SUBLIBRARY_H
#include "avltree.h"

//Player without level
class SubPlayer
{
public:
    int PlayerId;
    int GroupId;
    int Level;

    SubPlayer() : PlayerId(0), GroupId(0), Level(0) {};
    SubPlayer(int PlayerId, int GroupId, int Level) : PlayerId(PlayerId), GroupId(GroupId), Level(Level) {};
    bool operator< (SubPlayer const& other)
    {
        return PlayerId < other.PlayerId;
    }
    bool operator== (SubPlayer const& other)
    {
        return PlayerId == other.PlayerId;
    }
    bool operator> (SubPlayer const& other)
    {
        return PlayerId > other.PlayerId;
    }
};

//Player with level but without ptr for players by group trees and the big level tree
class Player : public SubPlayer
{
public:
    Player() : SubPlayer() {};
    Player(int PlayerID, int GroupId, int Level) : SubPlayer(PlayerID, GroupId, Level) {};

    bool operator< (Player const& other)
    {
        if (Level == other.Level)
        {
            return PlayerId > other.PlayerId; //??
        }
        return Level < other.Level;
    }
    bool operator== (Player const& other)
    {
        return Level == other.Level;
    }

    bool operator> (Player const& other)
    {
        if (Level == other.Level)
        {
            return PlayerId < other.PlayerId; //??
        }
        return Level > other.Level;
    }

};

//player with ptr for players by level tree
/* class PlayerWithPtr : public SubPlayer
{
public:
    shared_ptr<Node<Player>> ptr_to_player;
    PlayerWithPtr(int PlayerId, int GroupId) : SubPlayer(PlayerId, GroupId), ptr_to_player(nullptr) {};
}; */

#endif