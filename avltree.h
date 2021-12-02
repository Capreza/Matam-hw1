//
// Created by galra on 11/29/2021.
//

#ifndef HW1_AVLTREE_H
#define HW1_AVLTREE_H
#include <iostream>
#include "node.h"




template<class T>
class AVLTree
{
private:
    shared_ptr<Node<T>> head;

    void rrRotate(shared_ptr<Node<T>> &sub_root);
    void rlRotate(shared_ptr<Node<T>> &sub_root);
    void lrRotate(shared_ptr<Node<T>> &sub_root);
    void llRotate(shared_ptr<Node<T>> &sub_root);
    void updateHeights(shared_ptr<Node<T>> &node);
    void balance(shared_ptr<Node<T>> &new_node, bool inserting);
    void recursiveInOrder(shared_ptr<Node<T>> &sub_root);
    void recursiveInOrderRev(shared_ptr<Node<T>> &sub_root);
    shared_ptr<Node<T>> removeNode(shared_ptr<Node<T>> &node);
    shared_ptr<Node<T>> insert(T data);


public:
    void append(T& data);
    void remove(T& data);
    void inOrder();
    void inOrderRev();
    T const* get(T& data)const;
    ~AVLTree();

};

template <class T>
AVLTree<T>::~AVLTree<T>()
{

    while(head)
    {
        (*this).remove(head->data);
    }
    /*
    if(head)
    {
        Node<T> start = *head;
        start.disconnect();
        head = nullptr;
    }
     */
}

template<class T>
T const* AVLTree<T>::get(T& data) const
{
    shared_ptr<Node<T>> current = head;
    while(current)
    {
        if(current->data == data)
        {
            return &(current->data);
        }
        if(current->data > data)
        {
            current = current->son2;
        }
        else
        {
            current=current->son1;
        }
    }
    return nullptr;
}

template<class T>
shared_ptr<Node<T>> AVLTree<T>::removeNode(shared_ptr<Node<T>> &node)
{
    if(!node->son1 && !node->son2)
    {
        if(node->parent)
        {
            if(node == node->parent->son1)
            {
                node->parent->son1 = nullptr;
            }
            else
            {
                node->parent->son2 = nullptr;
            }
            shared_ptr<Node<T>> return_val = node->parent;
            node->parent = nullptr;
            return return_val;
        }
        else
        {
            this->head = nullptr;
            printf("num of uses case1 = %ld\n", node.use_count());

            return nullptr;
        }
    }
    else if(!node->son1 && node->son2)
    {
        if(node->parent)
        {
            if(node->parent->son1 == node)
            {
                node->parent->son1 = node->son2;
                node->son2->parent = node->parent;
            }
            else
            {
                node->parent->son2 = node->son2;
                node->son2->parent = node->parent;
            }
            node->parent = nullptr;
            shared_ptr<Node<T>> return_val = node->son2;
            node->son2 = nullptr;

            return return_val;
        }
        else
        {
            head = node->son2;
            node->son2->parent = nullptr;
            node->son2 = nullptr;
            printf("num of uses case2 = %ld\n", node.use_count());
            return head;
        }

    }
    else if(node->son1 && !node->son2)
    {
        if (node->parent)
        {
            if (node->parent->son1 == node)
            {
                node->parent->son1 = node->son1;
            }
            else
            {
                node->parent->son2 = node->son1;
            }
            node->son1->parent= node->parent;
            node->parent = nullptr;
            shared_ptr<Node<T>> return_val = node->son1;
            node->son1 = nullptr;
            return return_val;
        }
        else
        {
            head = node->son1;
            node->son1->parent = nullptr;
            node->son1 = nullptr;
            printf("num of uses case3 = %ld\n", node.use_count());

            return head;
        }
    }
    else
    {

        shared_ptr<Node<T>> removal_head = node;
        node = node->son1;
        while(node->son2)
        {
            node = node->son2;
        }
        printf("num of uses case4 PRE!!!! = %ld\n", node.use_count());

        shared_ptr<Node<T>> temp = node->parent;
        removal_head->data = node->data;
        if(node->son1)
        {
            temp->son2 = node->son1;
            node->son1->parent = temp;
            printf("num of uses case4 POST!!!! = %ld\n", node.use_count());

        }
        node->parent->son2 = nullptr;
        node->parent= nullptr;
        printf("num of uses case4 = %ld\n", node.use_count());

        return temp;
    }
}

template<class T>
void AVLTree<T>::remove(T& data)
{
    if(head) {
        shared_ptr<Node<T>> current = head;
        while (true) {
            if (current->data == data) {
                current = removeNode(current);
                break;
            } else if (current->data < data) {
                if (!current->son2) {
                    return;//maybe we want this to throw an error, idk
                }
                current = current->son2;
            } else {
                if (!current->son1) {
                    return;//maybe we want this to throw an error, idk
                }
                current = current->son1;
            }
        }
        if (current) {
            updateHeights(current);
            balance(current, false);
        }
    }
}

template<class T>
void AVLTree<T>::inOrder()
{
    recursiveInOrder(head);
}

template<class T>
void AVLTree<T>::inOrderRev()
{
    recursiveInOrderRev(head);

}

template<class T>
void AVLTree<T>::recursiveInOrderRev(shared_ptr<Node<T>> &sub_root)
{
    if(!sub_root)
    {
        return;
    }

    recursiveInOrderRev(sub_root->son2);
    std::cout << sub_root->data << std::endl;
    recursiveInOrderRev(sub_root->son1);
}

template<class T>
void AVLTree<T>::recursiveInOrder(shared_ptr<Node<T>> &sub_root)
{
    if(!sub_root)
    {
        return;
    }

    recursiveInOrder(sub_root->son1);
    std::cout << sub_root->data << std::endl;

    recursiveInOrder(sub_root->son2);
}

template<class T>
void AVLTree<T>::append(T& data)
{
    shared_ptr<Node<T>> new_node = insert(data);
    updateHeights(new_node);
    balance(new_node, true);
}

template<class T>
void AVLTree<T>::rrRotate(shared_ptr<Node<T>> &sub_root)
{
    shared_ptr<Node<T>> A = sub_root->son2;
    sub_root->son2->son1->parent = sub_root;
    sub_root ->son2 = sub_root->son2->son1;
    A->parent = sub_root->parent;
    sub_root->parent= A;
    A->son1 = sub_root;
}

template<class T>
void AVLTree<T>::rlRotate(shared_ptr<Node<T>> &sub_root)
{
    shared_ptr<Node<T>> A = sub_root->son2;
    shared_ptr<Node<T>> B= A->son1;
    A->son1 = B->son2;
    A->son1->parent = A;
    sub_root->son2 = B->son1;
    sub_root->son2->parent = sub_root;
    B->son2 = A;
    B->son1 = sub_root;
    A->parent = B;
    sub_root->parent = B;
}

template<class T>
void AVLTree<T>::lrRotate(shared_ptr<Node<T>> &sub_root)
{
    shared_ptr<Node<T>> A = sub_root->son1;
    shared_ptr<Node<T>> B= A->son2;
    A->son2 = B->son1;
    A->son2->parent = A;
    sub_root->son1 = B->son2;
    sub_root->son1->parent = sub_root;
    B->son1 = A;
    B->son2 = sub_root;
    A->parent = B;
    sub_root->parent = B;
}

template<class T>
void AVLTree<T>::llRotate(shared_ptr<Node<T>> &sub_root)
{
    shared_ptr<Node<T>> A = sub_root->son1;
    sub_root->son1->son2->parent = sub_root;
    sub_root ->son1 = sub_root->son1->son2;
    A->parent = sub_root->parent;
    sub_root->parent= A;
    A->son2 = sub_root;
}

template<class T>
void AVLTree<T>::balance(shared_ptr<Node<T>> &new_node, bool inserting)
{
    shared_ptr<Node<T>> current = new_node;
    while (current->parent)
    {
        int left_height = (current->parent->son1) ? current->parent->son1->height : 0;
        int right_height = (current->parent->son2) ? current->parent->son2->height : 0;

        int parent_balance = left_height-right_height;
        if(parent_balance == 2)
        {
            int son_ll_height = (current->parent->son1->son1) ? current->parent->son1->son1->height : 0;
            int son_lr_height = (current->parent->son1->son2) ? current->parent->son1->son2->height : 0;
            int son1_balance = son_ll_height-son_lr_height;
            if(son1_balance == -1 )
            {
                lrRotate(current->parent);
            }
            else
            {
                llRotate(current->parent);
            }
            if(inserting)
            {
                return;
            }
        }
        else if(parent_balance==-2)
        {
            int son_rr_height = (current->parent->son2->son2) ? current->parent->son2->son2->height : 0;
            int son_rl_height = (current->parent->son2->son1) ? current->parent->son2->son1->height : 0;
            int son2_balance = son_rl_height-son_rr_height;
            if(son2_balance == 1 )
            {
                rlRotate(current->parent);
            }
            else
            {
                rrRotate(current->parent);
            }
            if(inserting)
            {
                return;
            }
        }
        current= current->parent;
    }
}

template<class T>
shared_ptr<Node<T>> AVLTree<T>::insert(T data)
{
    shared_ptr<Node<T>> current = head;
    if(!current)
    {
        shared_ptr<Node<T>> new_node = (shared_ptr<Node<T>>) new Node<T>;
        head= new_node;
        new_node->height=0;
        new_node->data = data;
        return new_node;
    }

    while(true) //find where to insert new node
    {
        if(data<current->data)
        {
            if(!current->son1)
            {
                //add there
                shared_ptr<Node<T>> new_node = shared_ptr<Node<T>>(new Node<T>);
                new_node->data = data;
                new_node->height=0;
                new_node->parent = current;
                current->son1 = new_node;
                current = current->son1;
                return current;
            }
            else
            {
                //iterate
                current = current->son1;
            }
        }
        else if(data>current->data)
        {
            if(!current->son2)
            {
                //add there
                shared_ptr<Node<T>> new_node = shared_ptr<Node<T>>(new Node<T>);
                new_node->data = data;
                new_node->height=0;
                current->son2 = new_node;
                new_node->parent = current;
                current = current->son2;
                return current;
            }
            else
            {
                //iterate
                current = current->son2;
            }
        }
        else
        {
            return current; //item is already in avltree
        }
    }
}

template<class T>
void AVLTree<T>::updateHeights(shared_ptr<Node<T>> &node)
{
    shared_ptr<Node<T>> current = node;
    while(current) //update heights
    {

        int new_height = 0;
        if(current->son1 && current->son2)
        {
            new_height = (current->son1->height < current->son2->height) ? current->son2->height+1:current->son1->height+1;
        }
        else if(current->son1)
        {
            new_height = current->son1->height;
        }
        else if(current->son2)
        {
            new_height = current->son2->height;
        }
        else
        {
            new_height=0;
        }
        if(current->height == new_height)
        {
            return;
        }
        current = current->parent;
    }
}



#endif //HW1_AVLTREE_H
