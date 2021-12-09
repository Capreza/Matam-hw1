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
    T* data;
    int height;
    template<class J>
    friend class AVLTree;

public:
    Node(): data(nullptr), parent(nullptr),son1(nullptr),son2(nullptr),height(0){}
    ~Node()
    {
        if(data)
        {
            delete data;
        }
    }
};



#endif //HW1_NODE_H
