#ifndef HW2_UNIONNODE_H
#define HW2_UNIONNODE_H
#include "rankTree.h"

class UnionNode
{
protected:
    UnionNode* next;
    const int key;
    RankTree* level_tree;
    friend class UnionFind;

public:
    explicit UnionNode(const int key): next(nullptr), key(key), level_tree(nullptr) {}
    ~UnionNode()
    {
        if (level_tree)
        {
            delete level_tree;
        }
    }
};

class GroupControlUnit : public UnionNode
{
public:
    int size;

    GroupControlUnit(const int key, int size): UnionNode(key), size(size) {};
};

#endif //HW2_UNIONNODE_H
