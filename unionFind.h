#ifndef UNIONFIND_H
#define UNIONFIND_H
#include "UnionNode.h"

class UnionFind
{
private:
    GroupControlUnit** groups; //key is name of group, data is size of group
    UnionNode** objects;
    int size;
    int scale;

public:
    explicit UnionFind(int size, int scale): groups(nullptr), objects(nullptr), size(size), scale(scale)
    {
        GroupControlUnit** group_arr = new GroupControlUnit*[size];
        UnionNode** object_arr = new UnionNode*[size];

        for (int i = 0; i < size; i++)
        {
            UnionNode* new_object = new UnionNode(i + 1);
            GroupControlUnit* new_group = new GroupControlUnit(i + 1, scale, 1);
            new_group->next = new_object;
            new_object->next = new_group;
            group_arr[i] = new_group;
            object_arr[i] = new_object;
        }
        groups =group_arr;
        objects = object_arr;
    }

    ~UnionFind();
    int getSize() const;
    int groupGetSize(int group_key) const;
    RankTree* getGroupTree(int group_key) const;
    void unite(int key1, int key2);
    int find(int key) const; //this doesn't address trying to find objects that don't exist,
    //because it shouldn't happen
};

#endif //UNIONFIND_H
