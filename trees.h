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

class SubPlayerTree : public AVLTree<SubPlayer>
{
public:
    void safeRemove(SubPlayer* to_delete)
    {
        SubPlayer* temp_player = new SubPlayer(to_delete->PlayerId, to_delete->GroupId, to_delete->Level);
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

class AllGroupAVLTree : public AVLTree<Group>
{
public:
    void safeRemove(Group* to_delete)
    {
        Player** players_to_delete = new Player*[to_delete->PlayerTree.getSize()];
        to_delete->PlayerTree.inOrder(players_to_delete);
        for (int i = 0; i < to_delete->PlayerTree.getSize(); i++)
        {
            to_delete->PlayerTree.safeRemove(players_to_delete[i]);
        }
        delete[] players_to_delete;
        this->remove(to_delete);
    }
};

#endif