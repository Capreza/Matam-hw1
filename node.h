//
// Created by galra on 11/29/2021.
//
#ifndef HW1_NODE_H
#define HW1_NODE_H
#include <iostream>
#include <memory>
using std::shared_ptr;



template<class T>
class Node
{
private:
    shared_ptr<Node<T>> parent;
    shared_ptr<Node<T>> son1;
    shared_ptr<Node<T>> son2;
    T* data;
    int height;
    template<class J>
    friend class AVLTree;

public:
    Node() {
        data = NULL;
    }
    ~Node()
    {
        if(data)
        {
            delete data;
        }
    }
    friend void fillTree(T** arr, shared_ptr<Node<T>>& node)
    {
        if(!node)
        {
            return;
        }
        fillTree(arr, node->son1);
        node->data = (arr[0]);
        arr++;
        fillTree(arr, node->son2);
    }
};



#endif //HW1_NODE_H