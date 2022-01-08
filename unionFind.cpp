#include "unionFind.h"

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

static void arrayMerge(int** dest_arr, int* dest_keys, int** arr1, int* keys1, int size1, int** arr2, int* keys2, int size2, int scale)
{
    dest_keys[0] = 0;
    for (int i = 0; i < scale; i++)
    {
        arr1[0][i] += arr2[0][i];
    }
    dest_arr[0] = arr1[0];
    int i = 1, j = 1, k = 1;
    while (i < size1 + 1 && j < size2 + 1)
    {
        if (keys1[i - 1] == keys2[j - 1])
        {
            for (int m = 0; m < scale; m++)
            {
                arr1[i][m] += arr2[j][m];
            }
            dest_arr[k] = arr1[i++];
            j++;
        }
        else if (keys1[i - 1] < keys2[j - 1])
        {
            dest_arr[k] = arr1[i++];
        }
        else
        {
            dest_arr[k] = arr2[j++];
        }
        k++;
    }
    while (i < size1 + 1)
    {
        dest_arr[k++] = arr1[i++];
    }
    while (j < size2 + 1)
    {
        dest_arr[k++] = arr2[j++];
    }


    if (keys1 && keys2)
    {
        i = 0, j = 0, k = 1;
        while (i < size1 && j < size2)
        {
            if (keys1[i] == keys2[j])
            {
                dest_keys[k] = keys1[i++];
                j++;
            }
            else if (keys1[i] < keys2[j])
            {
                dest_keys[k] = keys1[i++];
            }
            else
            {
                dest_keys[k] = keys2[j++];
            }
            k++;
        }
        while (i < size1)
        {
            dest_keys[k] = keys1[i++];
        }
        while (j < size2)
        {
            dest_keys[k] = keys2[j++];
        }
    }
    else if (!keys1)
    {
        for (int t = 0; t < size2; t++)
        {
            dest_keys[t + 1] = keys2[t];
        }
    }
    else if (!keys2)
    {
        for (int t = 0; t < size1; t++)
        {
            dest_keys[t + 1] = keys1[t];
        }
    }
    else
    {
        dest_keys = nullptr;
    }
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

    int smaller_tree_size = smaller_group->level_tree->getSize();
    int bigger_tree_size = bigger_group->level_tree->getSize();

    int** smaller_group_arrays = new int*[smaller_tree_size + 1];
    int** bigger_group_arrays = new int*[bigger_tree_size + 1];
    int* smaller_group_keys = nullptr;
    int* bigger_group_keys = nullptr;
    if (smaller_tree_size != 0)
    {
        smaller_group_keys = new int[smaller_tree_size];
    }
    if (bigger_tree_size != 0)
    {
        bigger_group_keys = new int[bigger_tree_size];
    }

    int final_size = bigger_tree_size + smaller_tree_size;
    int** both_groups_arrays = new int*[final_size + 1];
    int* both_groups_keys = new int[final_size + 1];

    bigger_group->level_tree->inOrder(bigger_group_keys, bigger_group_arrays, -1, 1);
    smaller_group->level_tree->inOrder(smaller_group_keys, smaller_group_arrays, -1, 1);
    arrayMerge(both_groups_arrays, both_groups_keys, bigger_group_arrays, bigger_group_keys, bigger_tree_size,
               smaller_group_arrays, smaller_group_keys, smaller_tree_size, scale);

    bigger_group->level_tree->buildAndFillTree(both_groups_keys, both_groups_arrays, final_size);

    bigger_group->size += smaller_group->size;

    delete[] smaller_group_arrays;
    delete[] smaller_group_keys;
    delete[] bigger_group_keys;
    delete[] bigger_group_arrays;
}