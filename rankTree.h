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
    void updateHeights(Node* node, Node* sub_tree_root = nullptr, int prev_root_height = 0);
    void balance(Node* new_node, bool inserting);
    void recursiveInOrder(Node* const &sub_root, int** arr, int* wanted_size, int* index_ptr)const;
    void recursiveInOrderRev(Node* const &sub_root, int** arr, int* wanted_size, int* index_ptr)const;
    Node* removeNode(Node* node);
    Node* insert(int key, int* data);
    void destroyTree(Node* node);
    void fillTree(int** keys,int*** arr, Node* node);


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

    }
    //void replace(int* data, int* replacement);
    void append(int key, int* data);
    //void setDataNull(int** data);
    void remove(int key);
    void inOrder(int** arr, int wanted_size =-1)const;
    void inOrderRev(int** arr, int wanted_size=-1)const;
    bool isEmpty()const;
    int* get(int key)const;
    int getSize()const;
    //int* getMaxNodeData()const;
    int* getZero() const;
    ~RankTree();
};

int* RankTree::getZero() const
{
    return levelZero->scores;
}

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

int RankTree::getSize() const
{
    return size;
}

void RankTree::fillTree(int** keys, int*** arr, Node* node) //check if this works
{
    if(!node)
    {
        return;
    }
    fillTree(keys, arr, node->son1);
    node->key = *(keys[0]);
    *keys = *keys+1;
    node->scores = *(arr[0]);
    *arr= *arr+1;
    fillTree(keys, arr, node->son2);
}

Node* RankTree::buildTree(int height, int* removal_size)
{
    Node* new_node = nullptr;
    if(height >= 0) // not sure about this condition
    {
        if(height ==0)
        {
            if(*removal_size>0)
            {
                *removal_size= *removal_size-1;
                return new_node;
            }
        }
        Node* new_node = new Node(scale);
        new_node ->height = height;
        new_node->son2 = buildTree(height - 1, removal_size);
        new_node->son1 = buildTree(height - 1, removal_size);
        if(new_node->son1)
        {
            new_node->son1->parent = new_node;
        }
        if(new_node->son2)
        {
            new_node->son2->parent = new_node;
        }
    }
    return new_node;
}

void RankTree::destroyTree(Node* node)
{
    delete levelZero;
    if(!node)
    {
        return;
    }
    destroyTree(node->son1);
    destroyTree(node->son2);
    node->son1 = nullptr;
    node->son2 = nullptr;
    node->parent = nullptr;
    delete node;
}

bool RankTree::isEmpty() const //returns true if tree is empty, false otherwise
{
    return !head;
}

RankTree::~RankTree()
{
    destroyTree(head);
}

int* RankTree::get(int key) const
{
    Node* current = head;
    while(current)
    {

        if(current->key == key)
        {
            return current->scores;
        }
        else if(current->key > key)
        {
            current = current->son1;
        }
        else if(current->key < key)
        {
            current=current->son2;
        }
    }
    return nullptr;
}

Node* RankTree::removeNode(Node* node)
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
            Node* return_val = node->parent;
            node->parent = nullptr;
            delete node;
            return return_val;
        }
        else
        {
            this->head = nullptr;
            delete node;
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
            Node* return_val = node->son2;
            node->son2 = nullptr;
            delete node;
            return return_val;
        }
        else
        {
            head = node->son2;
            node->son2->parent = nullptr;
            node->son2 = nullptr;
            delete node;
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
            Node* return_val = node->son1;
            node->son1 = nullptr;
            delete node;
            return return_val;
        }
        else
        {
            head = node->son1;
            node->son1->parent = nullptr;
            node->son1 = nullptr;
            delete node;
            return head;
        }
    }
    else // node has two sons and we dont know if he has parents
    {

        Node* removal_head = node;
        node = node->son1;

        while(node->son2)
        {
            node = node->son2;

        }

        Node* temp = node->parent;
        int* temp_data = removal_head->scores;
        removal_head->scores = node->scores;
        node->scores = temp_data;
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
                delete node;
                return temp;
            }
        }
        else
        {
            if (node->parent->son2 == node)
            {
                temp->son2 = nullptr;
            }
            else
            {
                temp->son1 = nullptr;
            }
        }
        node->parent= nullptr;
        delete node;
        return temp;

    }
}

void RankTree::remove(int key)
{
    if(head) {
        Node* current = head;
        while (true)
        {
//            if(!current->data)
//            {
//                current = removeNode(current);
//                break;
//            }
            if (current->key == key)
            {
                current = removeNode(current);
                break;
            }
            else if (current->key < key)
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

void RankTree::inOrder(int** arr, int wanted_size) const
{
    int size_left = wanted_size;
    int *size_left_ptr = &size_left;
    int index = 0;
    int* index_ptr = &index;
    recursiveInOrder(head,arr, size_left_ptr, index_ptr);
}

void RankTree::inOrderRev(int** arr, int wanted_size) const
{
    int size_left = wanted_size;
    int *size_left_ptr = &size_left;
    int index = 0;
    int* index_ptr = &index;
    recursiveInOrderRev(head, arr, size_left_ptr, index_ptr);
}

void RankTree::recursiveInOrderRev(Node* const &sub_root, int** arr, int* wanted_size, int* index_ptr)const
{
    if(!sub_root)
    {
        return;
    }

    recursiveInOrderRev(sub_root->son2, arr, wanted_size, index_ptr);
    if(*wanted_size ==0)
    {
        return;
    }
    arr[*index_ptr] = &*sub_root->scores;
    (*wanted_size)--;
    (*index_ptr)++;
    recursiveInOrderRev(sub_root->son1, arr, wanted_size, index_ptr);
}

void RankTree::recursiveInOrder(Node* const &sub_root, int** arr, int* wanted_size, int* index_ptr)const
{
    if(!sub_root)
    {
        return;
    }

    recursiveInOrder(sub_root->son1, arr, wanted_size, index_ptr);
    if(*wanted_size ==0)
    {
        return;
    }
    arr[*index_ptr] = &*sub_root->scores;
    (*wanted_size)--;
    (*index_ptr)++;
    recursiveInOrder(sub_root->son2, arr, wanted_size, index_ptr);
}

void RankTree::append(int key, int* data)
{
    Node* new_node = insert(key, data);
    updateHeights(new_node);

    balance(new_node, true);
}

void RankTree::rrRotate(Node* sub_root)
{
    int prev_root_height = sub_root->height;
    Node* root = sub_root;
    Node* A = root->son2;
    Node* AL = A->son1;
    Node* sub_parent = root->parent;

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
    updateHeights(root, A, prev_root_height);
}

void RankTree::rlRotate(Node* sub_root)
{
    int prev_root_height = sub_root->height;
    Node* root = sub_root;
    Node* A = root->son2;
    Node* B= A->son1;
    Node* BL= B->son1;
    Node* BR= B->son2;
    Node* sub_parent= root->parent;

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
    updateHeights(A, B, prev_root_height);
    updateHeights(root, B, prev_root_height);
}

void RankTree::lrRotate(Node* sub_root)
{
    int prev_root_height = sub_root->height;
    Node* root = sub_root;
    Node* A = root->son1;
    Node* B= A->son2;
    Node* BL= B->son1;
    Node* BR= B->son2;
    Node* sub_parent= root->parent;

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
    updateHeights(A, B, prev_root_height);
    updateHeights(root, B, prev_root_height);
}

void RankTree::llRotate(Node* sub_root)
{
    int prev_root_height = sub_root->height;
    Node* root = sub_root;
    Node* A = root->son1;
    Node* AR = A->son2;
    Node* sub_parent = root->parent;

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
    updateHeights(root, A, prev_root_height);
}

void RankTree::balance(Node* new_node, bool inserting)
{
    Node* current = new_node;

    while (current)
    {
        int left_height = (current->son1) ? current->son1->height : -1;
        int right_height = (current->son2) ? current->son2->height : -1;

        int current_balance = left_height-right_height;
        if(current_balance == 2)
        {
            int son_ll_height = (current->son1->son1) ? current->son1->son1->height : -1;
            int son_lr_height = (current->son1->son2) ? current->son1->son2->height : -1;
            int son1_balance = son_ll_height-son_lr_height;
            if(son1_balance == -1 )
            {
                lrRotate(current);

            }
            else
            {

                llRotate(current);
            }
            if(inserting)
            {
                return;
            }
        }
        else if(current_balance==-2)
        {
            int son_rr_height = (current->son2->son2) ? current->son2->son2->height : -1;
            int son_rl_height = (current->son2->son1) ? current->son2->son1->height : -1;
            int son2_balance = son_rl_height-son_rr_height;
            if(son2_balance == 1 )
            {

                rlRotate(current);
            }
            else
            {

                rrRotate(current);
            }
            if(inserting)
            {
                return;
            }

        }
        current= current->parent;
    }
}

Node* RankTree::insert(int key, int* data)
{
    Node* current = head;
    if(!current)
    {
        Node* new_node = new Node(scale);

        new_node->key = key;
        new_node->scores = data;
        head= new_node;
        size++;
        return new_node;
    }

    while(true) //find where to insert new node
    {
        if(key < current->key)
        {
            if(!current->son1)
            {
                //add there
                Node* new_node = new Node(scale);
                new_node->key = key;
                new_node->scores = data;
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
        else if(key > current->key)
        {
            if(!current->son2)
            {
                //add there
                Node* new_node = new Node(scale);
                new_node->key = key;
                new_node->scores = data;
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

void RankTree::updateHeights(Node* node, Node* sub_tree_root, int prev_root_height)
{
    Node* current = node;

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
        if(sub_tree_root && sub_tree_root->height == prev_root_height)
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