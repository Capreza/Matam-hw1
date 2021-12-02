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
    shared_ptr<Node<T>> junk;
    shared_ptr<Node<T>> parent;
    shared_ptr<Node<T>> son1;
    shared_ptr<Node<T>> son2;
    T data;
    int height;
    void disconnect();
    template<class J>
    friend class AVLTree;

public:
    Node()
    {
        parent = nullptr;
        son1 = nullptr;
        son2 = nullptr;
    }
};




template <class T>
void Node<T>::disconnect()
{
    if (this->son1)
    {
        Node<T> left = *(this->son1);
        left.disconnect();
        this->son1 = nullptr;
    }
    if (this->son2)
    {
        Node<T> right = *(this->son2);
        right.disconnect();
        //printf("num of uses pre right = %ld\n", me.use_count());
        this->son2 = nullptr;

    }
    this->parent = nullptr;

}

#endif //HW1_NODE_H
