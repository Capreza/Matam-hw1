#ifndef HW2_UNIONNODE_H
#define HW2_UNIONNODE_H
#include "rankTree.h"

class UnionNode
{
protected:
    UnionNode* next;
    const int key;
    friend class UnionFind;

public:
    explicit UnionNode(const int key): next(nullptr), key(key) {}
};

class GroupControlUnit : public UnionNode
{
public:
    int size;
    RankTree* level_tree;

    GroupControlUnit(const int key, int scale, int size): UnionNode(key), size(size) , level_tree(nullptr)
    {
        level_tree = new RankTree(scale);
    }
    ~GroupControlUnit()
    {
        if (level_tree)
        {
            delete level_tree;
        }
    }
};

#endif //HW2_UNIONNODE_H
