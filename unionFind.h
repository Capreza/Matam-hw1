#ifndef UNIONFIND_H
#define UNIONFIND_H
#include "UnionNode.h"
#include "rankTree.h"

class UnionFind
{
private:
    GroupControlUnit** groups; //key is name of group, data is size of group
    UnionNode** objects;
    int size;

public:
    explicit UnionFind(int size, int scale): groups(nullptr), objects(nullptr), size(size)
    {
        GroupControlUnit** group_arr = new GroupControlUnit*[size];
        UnionNode** object_arr = new UnionNode*[size];

        for (int i = 0; i < size; i++)
        {
            UnionNode* new_object = new UnionNode(i + 1, scale);
            GroupControlUnit* new_group = new GroupControlUnit(i + 1, scale, 1);
            delete new_group->level_tree;
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
    void unite(int key1, int key2);
    int find(int key) const; //this doesn't address trying to find objects that don't exist,
    //because it shouldn't happen
};

int UnionFind::groupGetSize(int group_key) const
{
    return groups[group_key - 1]->size;
}

UnionFind::~UnionFind()
{
    for (int i = 0; i < size; i++)
    {
        delete groups[i];
        delete objects[i];
    }

    delete[] groups;
    delete[] objects;
}

int UnionFind::getSize() const
{
    return size;
}

int UnionFind::find(int key) const
{
    UnionNode* object = objects[key - 1];
    while(object->next->next != object)
    {
        object = object->next;
    }
    UnionNode* root = object;
    int ret_value = object->next->key;

    object = objects[key - 1];
    UnionNode* next_node = nullptr;
    while (object->next->next != object)
    {
        next_node = object->next;
        object->next = root;
        object = next_node;
    }

    return ret_value;
}


void UnionFind::unite(int key1, int key2)
{
    GroupControlUnit* group1 = groups[key1 - 1];
    GroupControlUnit* group2 = groups[key2 - 1];
    GroupControlUnit* bigger_group = group1->size > group2->size ? group1 : group2;
    GroupControlUnit* smaller_group = bigger_group == group1 ? group2 : group1;
    UnionNode* smaller_root = smaller_group->next;
    UnionNode* bigger_root = bigger_group->next;

    smaller_root->next = bigger_root;
    bigger_group->size += smaller_group->size;
    delete smaller_group;
}


#endif //UNIONFIND_H
