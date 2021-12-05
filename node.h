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
//    shared_ptr<Node<T>> junk;
    shared_ptr<Node<T>> parent;
    shared_ptr<Node<T>> son1;
    shared_ptr<Node<T>> son2;
    T data;
    int height;
    template<class J>
    friend class AVLTree;

public:
    Node():parent(nullptr),son1(nullptr),son2(nullptr),height(0)
    {}
};



#endif //HW1_NODE_H
