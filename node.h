#ifndef HW2_NODE_H
#define HW2_NODE_H

class Node
{
private:
    Node* parent;
    Node* son1;
    Node* son2;
    int key;
    int* scores;
    int* tree_scores;
    double tree_average_level;
    int height;
    friend class RankTree;

public:
    explicit Node(int scale): parent(nullptr), son1(nullptr), son2(nullptr), key(-1), \
    scores(nullptr),tree_scores(nullptr), tree_average_level(0), height(0)
    {
        scores = new int[scale]();
        tree_scores = new int [scale]();
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