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
    int* tree_scores;
    int height;
    friend class RankTree;

public:
    explicit Node(int scale): parent(nullptr), son1(nullptr), son2(nullptr), key(-1), \
    scores(nullptr),tree_scores(nullptr), height(0)
    {
        scores = new int[scale]();
        tree_scores = new int [scale]();
        for(int i=0;i<scale;i++)
        {
            scores[i] =0;
            tree_scores[i]=0;
        }
    }
    ~Node()
    {
        if(scores)
        {
            delete[] scores;
        }
        if(tree_scores)
        {
            delete[] tree_scores;
        }
    }
};



#endif //HW2_NODE_H
