#include "hashtable.h"

void Hashtable::insert(const int key, Player* x)
{
    int index_to_insert = key % size;

    LinkedListNode<Player>* curr_node = arr[index_to_insert];
    LinkedListNode<Player>* node_to_insert = new LinkedListNode<Player>(key, x);
    if (curr_node)
    {
        node_to_insert->next = curr_node;
    }
    arr[index_to_insert] = node_to_insert;

    curr_occupancy++;
    if (curr_occupancy == size)
    {
        this->enlarge();
    }
}

void Hashtable::remove(const int key)
{
    int index_to_delete = key % size;

    LinkedListNode<Player>* curr_node = arr[index_to_delete];
    if (curr_node == nullptr)
    {
        return;
    }
    LinkedListNode<Player>* prev = nullptr;
    while (curr_node)
    {
        if (curr_node->key == key)
        {
            if (prev == nullptr)
            {
                arr[index_to_delete] = curr_node->next;
            }
            else
            {
                prev->next = curr_node->next;
            }
            delete curr_node;
            curr_occupancy--;
            if (curr_occupancy == (size / 4))
            {
                this->shrink();
            }
            break;
        }
        else
        {
            prev = curr_node;
            curr_node = curr_node->next;
        }
    }
}

bool Hashtable::member(const int key) const
{
    int index_to_find = key % size;

    LinkedListNode<Player>* curr_node = arr[index_to_find];
    while (curr_node)
    {
        if (curr_node->key == key)
        {
            return true;
        }
        else
        {
            curr_node = curr_node->next;
        }
    }
    return false;
}

void Hashtable::replaceArray(Hashtable* hash, LinkedListNode<Player>** new_arr, int new_size)
{
    for (int i = 0; i < new_size; i++)
    {
        new_arr[i] = nullptr;
    }
    for (int i = 0; i < hash->size; i++)
    {
        if (hash->arr[i])
        {
            LinkedListNode<Player>* curr_node = hash->arr[i];
            while (curr_node)
            {
                int index_to_insert = curr_node->key % new_size;

                LinkedListNode<Player>* possible_node = new_arr[index_to_insert];
                LinkedListNode<Player>* next_node = curr_node->next;
                if (possible_node)
                {
                    curr_node->next = possible_node;
                }
                else
                {
                    curr_node->next = nullptr;
                }
                new_arr[index_to_insert] = curr_node;
                curr_node = next_node;
            }

        }
    }
    delete[] hash->arr; ///is it okay that i manually deleted every node?
    hash->arr = new_arr;
    hash->size = new_size;
}

void Hashtable::enlarge()
{
    int new_size = size * 2;
    LinkedListNode<Player>** new_arr = new LinkedListNode<Player>*[new_size];
    replaceArray(this, new_arr, new_size);

}

void Hashtable::shrink()
{
    int new_size = size / 2;
    LinkedListNode<Player>** new_arr = new LinkedListNode<Player>*[new_size];
    replaceArray(this, new_arr, new_size);
}

Player* Hashtable::getData(const int key) const
{
    int index_to_find = key % size;

    LinkedListNode<Player>* curr_node = arr[index_to_find];
    while (curr_node)
    {
        if (curr_node->key == key)
        {
            return curr_node->data;
        }
        else
        {
            curr_node = curr_node->next;
        }
    }
    return nullptr;
}