#ifndef SUBLIBRARY_H
#define SUBLIBRARY_H
#include "avltree.h"

//Player with level but without ptr for players by group trees and the big level tree
class Player
{
public:
    int PlayerId;
    int GroupId;
    int Level;

    Player() : PlayerId(0), GroupId(0), Level(0) {};
    Player(int PlayerId, int GroupId, int Level) : PlayerId(PlayerId), GroupId(GroupId), Level(Level) {};

    bool operator< (Player const& other) const
    {
        if (Level == other.Level)
        {
            return PlayerId > other.PlayerId; //??
        }
        return Level < other.Level;
    }
    bool operator== (Player const& other) const
    {
        return PlayerId == other.PlayerId;
    }

    bool operator> (Player const& other) const
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