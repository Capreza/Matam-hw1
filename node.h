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
    Node<T>* parent;
    Node<T>* son1;
    Node<T>* son2;
    shared_ptr<T> data;
    int height;
    template<class J>
    friend class AVLTree;

public:
    Node() : parent(nullptr), son1(nullptr), son2(nullptr), data(nullptr), height(0) {}

    friend void fillTree(shared_ptr<T>* arr, Node<T>* node)
    {
        if(!node)
        {
            return;
        }
        fillTree(arr, node->son1);
        node->data = *arr;
        arr++;
        fillTree(arr, node->son2);
    }
};



#endif //HW1_NODE_H