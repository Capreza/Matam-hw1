#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "linkedListNode.h"
#include "player.h"
///This hashtable can accept multiple keys with the same value. it is the responsibility
///of the user not to insert identical keys

class Hashtable
{
private:
    LinkedListNode<Player>** arr;
    int size;
    int curr_occupancy;

    void enlarge();
    void shrink();
    static void replaceArray(Hashtable* hash, LinkedListNode<Player>** new_arr, int new_size);

public:
    Hashtable() : arr(nullptr), size(4), curr_occupancy(0) {
        LinkedListNode<Player>** new_arr = new LinkedListNode<Player>*[size];
        for (int i = 0; i < size; i++) {
            new_arr[i] = nullptr;
        }
        arr = new_arr;
    }
    void insert(const int key, Player* x);
    void remove(const int key);
    bool member(const int key) const;
    //void print() const; //delete after finished with hw
    Player* getData(const int key) const;
};

/*void Hashtable::print() const
{
    for (int i = 0; i < size; i++)
    {
        cout << i << "  ||  ";
        LinkedListNode<Player>* curr_node = arr[i];
        while (curr_node)
        {
            cout << curr_node->key << ",  ";
            curr_node = curr_node->next;
        }
        cout << endl;
    }
}*/

#endif
