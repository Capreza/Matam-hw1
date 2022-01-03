#ifndef HW2_NODE_H
#define HW2_NODE_H
#include <iostream>

class Node
{
private:
    Node* parent;
    Node* son1;
    Node* son2;
    int key;
    int* scores;
    int height;
    friend class RankTree;

public:
    Node(): parent(nullptr), son1(nullptr), son2(nullptr), key(-1), \
    scores(nullptr), height(0) {}
    ~Node()
    {
        if(scores)
        {
            delete scores;
        }
    }
};



#endif //HW2_NODE_H
