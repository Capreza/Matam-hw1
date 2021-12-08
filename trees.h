#ifndef TREES_H
#define TREES_H
#include "subLibrary.h"

class PlayerPerGroupAVLTree : public AVLTree<Player>
{
public:
    shared_ptr<Player> HighestLevelInGroup;
    //need to add call to inherited constructor?

    PlayerPerGroupAVLTree() = default;
    explicit PlayerPerGroupAVLTree(AVLTree<Player>& tree) : AVLTree<Player>(tree), HighestLevelInGroup(nullptr)
    {
        if (!tree.isEmpty())
        {
            HighestLevelInGroup = (shared_ptr<Player>)&(this->getMaxNodeData());
        }
    }

};


//group with a pointer to a tree of players
class Group
{
public:
    int GroupId;
    PlayerPerGroupAVLTree PlayerTree;
    Group() = default;

    explicit Group(int GroupId) : GroupId(GroupId) {};

    bool operator< (Group const& other)
    {
        return GroupId < other.GroupId;
    }
    bool operator== (Group const& other)
    {
        return GroupId == other.GroupId;
    }
    bool operator> (Group const& other)
    {
        return GroupId > other.GroupId;
    }

    void playerTreeToArray(Player** arr) const
    {
        return PlayerTree.inOrder(arr);
    }
};


class GroupAVLTree : public AVLTree<Group>
{
public:
    shared_ptr<Player> HighestLevelOverall; //need to support this when removing players
    //need to add call to inherited constructor?
};

#endif