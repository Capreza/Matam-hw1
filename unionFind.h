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
    void unite(int key1, int key2);  //pretty sure we need to work on this
    void unite(int key1, int key2);  //pretty sure we need to work on this
    int find(int key) const; //this doesn't address trying to find objects that don't exist,
    //because it shouldn't happen
};

RankTree* UnionFind::getGroupTree(int group_key) const
{
    return groups[group_key - 1]->level_tree;
}

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

static void arrayMerge(int** dest_arr, int* dest_keys, int** arr1, int* keys1, int size1, int** arr2, int* keys2, int size2)
{
    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2)
    {
        if (keys1[i] < keys2[j])
        {
            dest_keys[k] = keys1[i];
            dest_arr[k] = arr1[i++];
        }
        else
        {
            dest_keys[k] = keys2[j];
            dest_arr[k] = arr2[j++];
        }
        k++;
    }
    while (i < size1)
    {
        dest_keys[k] = keys1[i];
        dest_arr[k++] = arr1[i++];
    }
    while (j < size2)
    {
        dest_keys[k] = keys2[j];
        dest_arr[k++] = arr2[j++];
    }
}

void UnionFind::unite(int key1, int key2)  //not sure about validity of outcome tree, maybe this func works in a way that the output tree will have duplicities?
{
    GroupControlUnit* group1 = groups[key1 - 1];
    GroupControlUnit* group2 = groups[key2 - 1];
    GroupControlUnit* bigger_group = group1->size > group2->size ? group1 : group2;
    GroupControlUnit* smaller_group = bigger_group == group1 ? group2 : group1;
    UnionNode* smaller_root = smaller_group->next;
    UnionNode* bigger_root = bigger_group->next;

    smaller_root->next = bigger_root;
    bigger_group->size += smaller_group->size;

    int smaller_tree_size = smaller_group->level_tree->getSize();
    int bigger_tree_size = bigger_group->level_tree->getSize();

    int** smaller_group_arrays = new int*[smaller_tree_size];
    int** bigger_group_arrays = new int*[bigger_tree_size];
    int* smaller_group_keys = new int[smaller_tree_size];
    int* bigger_group_keys = new int[bigger_tree_size];

    int final_size = bigger_tree_size + smaller_tree_size;
    int** both_groups_arrays = new int*[final_size];
    int* both_groups_keys = new int[final_size];

    bigger_group->level_tree->inOrder(bigger_group_keys, bigger_group_arrays, -1, 1);
    smaller_group->level_tree->inOrder(smaller_group_keys, smaller_group_arrays, -1, 1);
    arrayMerge(both_groups_arrays, both_groups_keys, bigger_group_arrays, bigger_group_keys, bigger_tree_size,
               smaller_group_arrays, smaller_group_keys, smaller_tree_size);

    bigger_group->level_tree->buildAndFillTree(both_groups_keys, both_groups_arrays, final_size);

    delete[] smaller_group_arrays;
    delete[] smaller_group_keys;
    delete[] bigger_group_keys;
    delete[] bigger_group_arrays;
    delete smaller_group;
}


#endif //UNIONFIND_H
