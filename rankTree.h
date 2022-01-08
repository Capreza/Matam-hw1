#ifndef RANKTREE_H
#define RANKTREE_H
#include <iostream>
#include "node.h"

class RankTree
{
protected:
    Node* head;
    Node* levelZero;
private:
    int size;
    int scale;
    void rrRotate(Node* sub_root);
    void rlRotate(Node* sub_root);
    void lrRotate(Node* sub_root);
    void llRotate(Node* sub_root);
    Node* buildTree(int height, int* removal_size);
    void updateHeights(Node* node);
    void balance(Node* new_node, bool inserting);
    void recursiveInOrder(Node* const &sub_root, int* keys, int** arr, int* wanted_size, int* index_ptr, int is_uniting)const;
    void recursiveInOrderRev(Node* const &sub_root, int* keys, int** arr, int* wanted_size, int* index_ptr)const;
    Node* removeNode(Node* node);
    Node* insert(int key, int* data);
    void destroyTree(Node* node);
    void fillTree(int** keys,int*** arr, Node* node);
    void updateTreeScores(Node* node);
    Node* find(const int key) const;
    void postOrderUpdate(Node* node);

public:

    explicit RankTree(int scale, Node* head = nullptr): head(head), levelZero(nullptr), size(0), scale(scale)
    {
        Node* new_levelZero = new Node(scale);
        levelZero = new_levelZero;
    }

    void buildAndFillTree(int* keys, int** arr, int wanted_size)
    {
        int tree_height =0;
        int temp_size = wanted_size;
        while(temp_size>1)
        {
            temp_size/=2;
            tree_height++;
        }

        int current_size =1;
        for(int i=0;i<=tree_height;i++)
        {
            current_size = current_size*2;
        }
        current_size--;
        int removal_size = current_size-wanted_size;
        RankTree return_tree(scale);

        this->head = return_tree.buildTree(tree_height, &removal_size);
        this->size = wanted_size;
        this->fillTree(&keys, &arr, this->head);
        this->postOrderUpdate(head);
    }

    //void replace(int* data, int* replacement);
    void append(int key, int* data);
    //void setDataNull(int** data);
    void remove(int key);
    void inOrder(int* keys, int** arr, int wanted_size =-1, int is_uniting = 0)const;
    void inOrderRev(int* keys, int** arr, int wanted_size=-1)const;
    bool isEmpty()const;
    int* get(int key)const;
    int* getTreeData(int key) const;
    int getSize()const;
    //int* getMaxNodeData()const;
    int* getZero() const;
    void userUpdateTreeScores(const int key);
    ~RankTree();
    Node* findClosestHigh(int high_level, int low_level) const;
    Node* findClosestLow(int low_level, int high_level) const;
    double findPercentage(Node* low_node, Node* high_node, int score) const;
    double findAverage(int num) const;
};

/* void RankTree::replace(int* data, int* replacement)
{
    Node* current = head;
    while(current)
    {
        if(*current->data == *data)
        {
            break;
        }
        else if(*current->data > *data)
        {
            current = current->son1;
        }
        else if(*current->data < *data)
        {
            current = current->son2;
        }
    }
    if(current)
    {
        current->data = replacement;
    }
} */

/*void RankTree::setDataNull(int** data)
{
    *data = nullptr;
}*/

/* int* RankTree::getMaxNodeData() const
{
    Node* current = head;
    if (!current)
    {
        return nullptr;
    }
    while(current->son2)
    {
        current = current->son2;
    }
    return current->data;
} */

#endif //HW1_AVLTREE_H



/*
template<class T>
void AVLTree<T>::recursiveTrim(int *amount, shared_ptr<Node<T>> node)
{
    if(*amount ==0)
    {
        return;
    }
    if(node->son1)
    {
        if (node->son1->son1)
        {
            recursiveTrim(amount, node->son2);
            recursiveTrim(amount, node->son1);
        }
        else
        {
            node->son2->parent = nullptr;
            node->son2 = nullptr;
            *amount--;
            if(*amount >0)
            {
                node->son1->parent = nullptr;
                node->son1 = nullptr;
                *amount--;
            }
        }
    }
}
template<class T>
void AVLTree<T>::trimTree(int wanted_size, int height)
{
    if(wanted_size == 0)
    {
        return;
    }
    int current_size =1;
    for(int i=0;i<height;i++)
        current_size--;
    {
        current_size*=2;
    }
    int removal_size = current_size-wanted_size;
    this->recursiveTrim(&removal_size, head);
}
 */