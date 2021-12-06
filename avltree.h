//
// Created by galra on 11/29/2021.
//

#ifndef HW1_AVLTREE_H
#define HW1_AVLTREE_H
#include <iostream>
#include "node.h"


//the problem is in the rotations, passing of parent of sub root

template<class T>
class AVLTree
{
protected:
    shared_ptr<Node<T>> head;
private:
    int size;
    void rrRotate(shared_ptr<Node<T>> &sub_root);
    void rlRotate(shared_ptr<Node<T>> &sub_root);
    void lrRotate(shared_ptr<Node<T>> &sub_root);
    void llRotate(shared_ptr<Node<T>> &sub_root);
    void updateHeights(shared_ptr<Node<T>> &node);
    void balance(shared_ptr<Node<T>> &new_node, bool inserting);
    void recursiveInOrder(shared_ptr<Node<T>> &sub_root, T** arr, int* size)const;
    void recursiveInOrderRev(shared_ptr<Node<T>> &sub_root, T** arr, int* size)const;
    shared_ptr<Node<T>> removeNode(shared_ptr<Node<T>> &node);
    shared_ptr<Node<T>> insert(T data);
    void destroyTree(shared_ptr<Node<T>> &node);
    static shared_ptr<Node<T>> buildTree(int height);
    void fillTree(T** arr, shared_ptr<Node<T>> node);
    void trimTree(int size, int height);
    void recursiveTrim(int* amount, shared_ptr<Node<T>> node);


public:
    static AVLTree<T>& buildAndFillTree(T** arr, int size);
    void append(T& data);
    void remove(T& data);
    void inOrder(T** arr, int size =-1)const;
    void inOrderRev(T** arr, int size=-1)const;
    bool isEmpty()const;
    T* get(T& data)const;
    int getSize()const;
    T& getMaxNodeData()const;
    ~AVLTree();
    AVLTree(shared_ptr<Node<T>> head = nullptr): size(0), head(head)
    {}
};

template<class T>
T& AVLTree<T>::getMaxNodeData() const
{
    shared_ptr<Node<T>> current = head;
    while(current->son2)
    {
        current->son2;
    }
    return current->data;
}

template<class T>
int AVLTree<T>::getSize() const
{
    return size;
}

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
void AVLTree<T>::trimTree(int size, int height)
{
    if(size == 0)
    {
        return;
    }
    int current_size =1;
    for(int i=0;i<height;i++)
    {
        current_size*=2;
    }
    current_size--;
    int removal_size = current_size-size;
    this->recursiveTrim(&removal_size, head);


}

template <class T>
void AVLTree<T>::fillTree(T** arr, shared_ptr<Node<T>> node)
{
    if(!node)
    {
        return;
    }
    fillTree(arr, node->son1);
    node->data = *(arr[0]);
    arr++;
    fillTree(node->son2);
}

template <class T>
shared_ptr<Node<T>> AVLTree<T>::buildTree(int height)
{
    shared_ptr<Node<T>> new_node = nullptr;
    if(height >= 0) // not sure about this condition
    {
        new_node = (shared_ptr<Node<T>>)new Node<T>;
        new_node ->height = height;
        new_node->son1 = buildTree(height - 1);
        new_node->son2 = buildTree(height - 1);
        if(new_node->son1)
        {
            new_node->son1->parent = new_node;
            new_node->son2->parent = new_node;
        }
    }

    return new_node;


}

template <class T>
AVLTree<T>& AVLTree<T>::buildAndFillTree(T **arr, int size)
{
    int tree_height =0;
    int temp_size =size;
    while(temp_size)
    {
        temp_size/=2;
        tree_height++;
    }

    AVLTree<T> return_tree;
    return_tree->head = buildTree(tree_height);
    return_tree.trimTree(size, tree_height);
    return_tree->size = size;
    return_tree.fillTree(arr, return_tree->head);

    return return_tree;
}

template<class T>
void AVLTree<T>::destroyTree(shared_ptr<Node<T>> &node)
{
    if(!node)
    {
        return;
    }
    destroyTree(node->son1);
    destroyTree(node->son2);
    node->son1 = nullptr;
    node->son2 = nullptr;
    node->parent = nullptr;

}

template <class T>
bool AVLTree<T>::isEmpty() const //returns true if tree is empty, false otherwise
{
    return !head;
}

template <class T>
AVLTree<T>::~AVLTree<T>()
{
    destroyTree(head);
}

template<class T>
T* AVLTree<T>::get(T& data) const
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
    size--;
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
            }
            else
            {
                node->parent->son2 = node->son2;
            }
            node->son2->parent = node->parent;
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
            return head;
        }
    }
    else // node has two sons and we dont know if he has parents
    {

        shared_ptr<Node<T>> removal_head = node;
        node = node->son1;

        while(node->son2)
        {
            node = node->son2;

        }

        shared_ptr<Node<T>> temp = node->parent;
        removal_head->data = node->data;
        if(node->son1)
        {
            if(node->parent->son1 == node)
            {
                temp->son1 = node->son1;
                node->son1->parent = temp;
                node->son1 = nullptr;
                node->parent = nullptr;
            }
            else
            {
                temp->son2 = node->son1;
                node->son1->parent = temp;
                node->son1 = nullptr;
                node->parent = nullptr;
                return temp;
            }
        }
        {
            temp->son2 = nullptr;
        }
        node->parent= nullptr;
        return temp;

    }
}

template<class T>
void AVLTree<T>::remove(T& data)
{
    if(head) {
        shared_ptr<Node<T>> current = head;
        while (true)
        {
            if (current->data == data)
            {
                current = removeNode(current);
                break;
            }
            else if (current->data < data)
            {
                if (!current->son2)
                {
                    return;//maybe we want this to throw an error, idk
                }
                current = current->son2;
            }
            else
                {
                if (!current->son1)
                {
                    return;//maybe we want this to throw an error, idk
                }
                current = current->son1;
            }
        }
        if (current)
        {
            updateHeights(current);
            balance(current, false);
        }
    }
}

template<class T>
void AVLTree<T>::inOrder(T** arr, int size =-1) const
{
    int size_left = size;
    int *size_left_ptr = &size_left;
    recursiveInOrder(head,arr, size_left_ptr);
}

template<class T>
void AVLTree<T>::inOrderRev(T** arr, int size=-1) const
{
    int size_left = size;
    int *size_left_ptr = &size_left;
    recursiveInOrderRev(head, arr, size_left_ptr);
}

template<class T>
void AVLTree<T>::recursiveInOrderRev(shared_ptr<Node<T>> &sub_root, T** arr, int* size)const
{
    if(!sub_root)
    {
        return;
    }

    recursiveInOrderRev(sub_root->son2);
    if(size ==0)
    {
        return;
    }
    *arr = &(sub_root->data);
    *size--;
    arr++;
    recursiveInOrderRev(sub_root->son1);
}

template<class T>
void AVLTree<T>::recursiveInOrder(shared_ptr<Node<T>> &sub_root, T** arr, int* size)const
{
    if(!sub_root)
    {
        return;
    }

    recursiveInOrder(sub_root->son1);
    if(size ==0)
    {
        return;
    }
    *arr = &(sub_root->data);
    *size--;
    arr++;
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

    shared_ptr<Node<T>> root = sub_root;
    shared_ptr<Node<T>> A = root->son2;
    shared_ptr<Node<T>> AL = A->son1;
    shared_ptr<Node<T>> sub_parent = root->parent;

    A->son1 = sub_root;
    A->parent = sub_parent;
    root->son2 = AL;
    root->parent =A;
    if(AL)
    {
        AL->parent = root;
    }
    if(sub_parent)
    {
        if(sub_parent->son1 == root)
        {
            sub_parent->son1 = A;
        }
        else if (sub_parent->son2 == root)
        {
            sub_parent->son2 = A;
        }

    }
    else
    {
        this->head = A;
    }
    updateHeights(root);



}


template<class T>
void AVLTree<T>::rlRotate(shared_ptr<Node<T>> &sub_root)
{
    shared_ptr<Node<T>> root = sub_root;
    shared_ptr<Node<T>> A = root->son2;
    shared_ptr<Node<T>> B= A->son1;
    shared_ptr<Node<T>> BL= B->son1;
    shared_ptr<Node<T>> BR= B->son2;
    shared_ptr<Node<T>> sub_parent= root->parent;

    B->son2 =A;
    B->son1 = root;
    A->parent = B;
    root->parent = B;
    A->son1 = BR;
    root->son2 = BL;
    if(BL)
    {
        BL->parent = root;
    }
    if(BR)
    {
        BR->parent = A;
    }
    if(sub_parent)
    {
        if(sub_parent->son1 == root)
        {
            sub_parent->son1 = B;
        }
        else if(sub_parent->son2 == root)
        {
            sub_parent->son2 = B;
        }

    }
    else
    {
        this->head = B;
    }
    B->parent = sub_parent;
    updateHeights(A);
    updateHeights(root);
}

template<class T>
void AVLTree<T>::lrRotate(shared_ptr<Node<T>> &sub_root)
{
    shared_ptr<Node<T>> root = sub_root;
    shared_ptr<Node<T>> A = root->son1;
    shared_ptr<Node<T>> B= A->son2;
    shared_ptr<Node<T>> BL= B->son1;
    shared_ptr<Node<T>> BR= B->son2;
    shared_ptr<Node<T>> sub_parent= root->parent;

    B->son1 =A;
    B->son2 = root;
    A->parent = B;
    root->parent = B;
    A->son2 = BL;
    root->son1 = BR;
    if(BL)
    {
        BL->parent = A;
    }
    if(BR)
    {
        BR->parent = root;
    }
    if(sub_parent)
    {
        if(sub_parent->son1 == root)
        {
            sub_parent->son1 = B;
        }
        else if(sub_parent->son2 == root)
        {
            sub_parent->son2 = B;
        }

    }
    else
    {
        this->head = B;
    }
    B->parent = sub_parent;
    updateHeights(A);
    updateHeights(root);


}

template<class T>
void AVLTree<T>::llRotate(shared_ptr<Node<T>> &sub_root)
{
    shared_ptr<Node<T>> root = sub_root;
    shared_ptr<Node<T>> A = root->son1;
    shared_ptr<Node<T>> AR = A->son2;
    shared_ptr<Node<T>> sub_parent = root->parent;

    A->son2 = root;
    A->parent = sub_parent;
    root->son1 = AR;
    root->parent =A;
    if(AR)
    {
        AR->parent = root;
    }
    if(sub_parent)
    {
        if(sub_parent->son1 == root)
        {
            sub_parent->son1 = A;
        }
        else if (sub_parent->son2 == root)
        {
            sub_parent->son2 = A;
        }

    }
    else
    {
        this->head = A;
    }
    updateHeights(root);

}

template<class T>
void AVLTree<T>::balance(shared_ptr<Node<T>> &new_node, bool inserting)
{
    shared_ptr<Node<T>> current = new_node;

    while (current->parent)
    {



        int left_height = (current->parent->son1) ? current->parent->son1->height : -1;
        int right_height = (current->parent->son2) ? current->parent->son2->height : -1;

        int parent_balance = left_height-right_height;
        if(parent_balance == 2)
        {
            int son_ll_height = (current->parent->son1->son1) ? current->parent->son1->son1->height : -1;
            int son_lr_height = (current->parent->son1->son2) ? current->parent->son1->son2->height : -1;
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
            int son_rr_height = (current->parent->son2->son2) ? current->parent->son2->son2->height : -1;
            int son_rl_height = (current->parent->son2->son1) ? current->parent->son2->son1->height : -1;
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
        shared_ptr<Node<T>> new_node ( new Node<T>);

       // shared_ptr<Node<T>> new_node = (shared_ptr<Node<T>>) new Node<T>;
        head= new_node;
        new_node->data = data;
        size++;
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
                new_node->parent = current;
                current->son1 = new_node;
                current = current->son1;
                size++;
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
                current->son2 = new_node;
                new_node->parent = current;
                current = current->son2;
                size++;
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
            new_height = current->son1->height+1;

        }
        else if(current->son2)
        {
            new_height = current->son2->height+1;

        }
        else
        {
            new_height=0;
        }
        if(current->height == new_height && new_height !=0)
        {
            return;
        }
        else
        {
            current->height = new_height;
        }

        current = current->parent;
    }
}



#endif //HW1_AVLTREE_H
