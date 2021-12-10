#ifndef TREES_H
#define TREES_H
#include "subLibrary.h"

class PlayerPerGroupAVLTree : public AVLTree<Player>
{
public:
    Player* HighestLevelInGroup;
    //need to add call to inherited constructor?

    PlayerPerGroupAVLTree() : HighestLevelInGroup(nullptr){};
    explicit PlayerPerGroupAVLTree(AVLTree<Player>& tree) : AVLTree<Player>(tree), HighestLevelInGroup(nullptr)
    {
        if (!tree.isEmpty())
        {
            HighestLevelInGroup = this->getMaxNodeData();
        }
    }

    void safeRemove(Player* to_delete)
    {
        Player* temp_player= new Player(to_delete->PlayerId,to_delete->GroupId,to_delete->Level);
        this->replace(to_delete, temp_player);
        this->remove(temp_player);
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
    Player* HighestLevelOverall; //need to support this when removing players
    //need to add call to inherited constructor?
    GroupAVLTree(): HighestLevelOverall(nullptr){};

    void safeRemove(int id, Group* to_delete)
    {
        Group* temp_group= new Group(id);
        this->replace(to_delete, temp_group);
        this->remove(temp_group);
    }
};

#endif