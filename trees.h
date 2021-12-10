#ifndef TREES_H
#define TREES_H
#include "Player.h"

class PlayerPerGroupAVLTree : public AVLTree<Player>
{
public:
    shared_ptr<Player> HighestLevelInGroup;
    //need to add call to inherited constructor?

    PlayerPerGroupAVLTree() : AVLTree<Player>(), HighestLevelInGroup(nullptr) {};
    explicit PlayerPerGroupAVLTree(AVLTree<Player>& tree) : AVLTree<Player>(tree), HighestLevelInGroup(nullptr)
    {
        if (!tree.isEmpty())
        {
            HighestLevelInGroup = this->getMaxNodeData();
        }
    }

};


//group with a pointer to a tree of players
class Group
{
public:
    int GroupId;
    PlayerPerGroupAVLTree PlayerTree;

    //Group() = default;

    explicit Group(int GroupId) : GroupId(GroupId) {}
    Group(int GroupId, PlayerPerGroupAVLTree& PlayerTree) : GroupId(GroupId), PlayerTree(PlayerTree) {}

    bool operator< (Group const& other) const
    {
        return GroupId < other.GroupId;
    }
    bool operator== (Group const& other) const
    {
        return GroupId == other.GroupId;
    }
    bool operator> (Group const& other) const
    {
        return GroupId > other.GroupId;
    }

    void playerTreeToArray(shared_ptr<Player>* arr) const
    {
        PlayerTree.inOrder(arr);
    }
};


class GroupAVLTree : public AVLTree<Group>
{
public:
    shared_ptr<Player> HighestLevelOverall;
};

#endif