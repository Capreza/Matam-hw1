#include "rankTree.h"

static int numPlayers(const int* arr, int scale)
{
    int sum = 0;
    for (int i = 0; i < scale; i++)
    {
        sum += arr[i];
    }
    return sum;
}

double RankTree::findAverage(int num) const
{
    Node* curr_node = head;
    int curr_num_players = numPlayers(curr_node->tree_scores, scale);
    while (curr_num_players > num)
    {
        curr_node = curr_node->son2;
        curr_num_players = numPlayers(curr_node->tree_scores, scale);
    }

    double curr_average = curr_node->tree_average_level;
    double curr_sum = curr_average * curr_num_players;
    double extra_sum = (num - curr_num_players) * curr_node->parent->key;
    return (curr_sum + extra_sum)/num;
}

double RankTree::findPercentage(Node* low_node, Node* high_node, int score) const
{
    if (low_node->key == high_node->key)
    {
        return ((double)(low_node->scores[score - 1]))/numPlayers(low_node->scores, scale);
    }

    int players_with_given_score = high_node->scores[score - 1];
    int total_players = numPlayers(high_node->scores, scale);
    if (high_node->son2)
    {
        players_with_given_score -= high_node->son2->tree_scores[score - 1];
        total_players -= numPlayers(high_node->son2->tree_scores, scale);
    }
    players_with_given_score -= low_node->tree_scores[score - 1];
    total_players -= numPlayers(low_node->tree_scores, scale);
    players_with_given_score += low_node->scores[score - 1];
    total_players += numPlayers(low_node->scores, scale);

    return ((double)players_with_given_score)/total_players;
}

Node* RankTree::findClosestHigh(int high_level, int low_level) const
{
    Node* curr_node = head;
    Node* closest = nullptr;
    while(curr_node)
    {
        if (curr_node->key == high_level)
        {
            return curr_node;
        }
        else if (curr_node->key > high_level)
        {
            curr_node = curr_node->son1;
        }
        else
        {
            if ((!closest || closest->key < curr_node->key) && curr_node->key > low_level)
            {
                closest = curr_node;
            }
            curr_node = curr_node->son2;
        }

    }
    return closest;
}

Node* RankTree::findClosestLow(int low_level, int high_level) const
{
    Node* curr_node = head;
    Node* closest = nullptr;
    while(curr_node)
    {
        if (curr_node->key == low_level)
        {
            return curr_node;
        }
        else if (curr_node->key < low_level)
        {
            curr_node = curr_node->son2;
        }
        else
        {
            if ((!closest || closest->key > curr_node->key) && curr_node->key < high_level)
            {
                closest = curr_node;
            }
            curr_node = curr_node->son1;
        }
    }
    return closest;
}

void RankTree::postOrderUpdate(Node* node)
{
    if (!node)
    {
        return;
    }
    postOrderUpdate(node->son1);
    postOrderUpdate(node->son2);
    updateTreeScores(node);
}

void RankTree::userUpdateTreeScores(const int key)
{
    updateHeights(this->find(key));
}

Node* RankTree::find(const int key) const
{
    if (key == 0)
    {
        return levelZero;
    }
    Node* current = head;
    while(current)
    {

        if(current->key == key)
        {
            return current;
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

int* RankTree::getZero() const
{
    return levelZero->scores;
}

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
        new_node = new Node(scale);
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

int* RankTree::getTreeData(int key) const
{
    if (key == 0)
    {
        return levelZero->tree_scores;
    }
    Node* current = head;
    while(current)
    {

        if(current->key == key)
        {
            return current->tree_scores;
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

int* RankTree::get(int key) const
{
    if (key == 0)
    {
        return levelZero->scores;
    }
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

void RankTree::inOrder(int* keys, int** arr, int wanted_size, int is_uniting) const
{
    int size_left = wanted_size;
    int *size_left_ptr = &size_left;
    int index = 0;
    int* index_ptr = &index;
    recursiveInOrder(head,keys, arr, size_left_ptr, index_ptr, is_uniting);
}

void RankTree::inOrderRev(int* keys, int** arr, int wanted_size) const
{
    int size_left = wanted_size;
    int *size_left_ptr = &size_left;
    int index = 0;
    int* index_ptr = &index;
    recursiveInOrderRev(head, keys, arr, size_left_ptr, index_ptr);
}

void RankTree::recursiveInOrderRev(Node* const &sub_root, int* keys, int** arr, int* wanted_size, int* index_ptr)const
{
    if(!sub_root)
    {
        return;
    }

    recursiveInOrderRev(sub_root->son2, keys, arr, wanted_size, index_ptr);
    if(*wanted_size ==0)
    {
        return;
    }
    keys[*index_ptr] = sub_root->key;
    arr[*index_ptr] = &*sub_root->scores;
    (*wanted_size)--;
    (*index_ptr)++;
    recursiveInOrderRev(sub_root->son1, keys, arr, wanted_size, index_ptr);
}

void RankTree::recursiveInOrder(Node* const &sub_root, int* keys, int** arr, int* wanted_size, int* index_ptr, int is_uniting)const
{
    if(!sub_root)
    {
        return;
    }

    recursiveInOrder(sub_root->son1, keys, arr, wanted_size, index_ptr, is_uniting);
    if(*wanted_size ==0)
    {
        return;
    }
    keys[*index_ptr] = sub_root->key;
    arr[*index_ptr] = &*sub_root->scores;
    if (is_uniting)
    {
        sub_root->scores = nullptr;
    }
    (*wanted_size)--;
    (*index_ptr)++;
    recursiveInOrder(sub_root->son2, keys, arr, wanted_size, index_ptr, is_uniting);
}

void RankTree::append(int key, int* data)
{
    Node* new_node = insert(key, data);
    updateHeights(new_node);
    balance(new_node, true);
}

void RankTree::rrRotate(Node* sub_root)
{
    //int prev_root_height = sub_root->height;
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
    updateHeights(root);
}

void RankTree::rlRotate(Node* sub_root)
{
    //int prev_root_height = sub_root->height;
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
    updateHeights(A);
    updateHeights(root);
}

void RankTree::lrRotate(Node* sub_root)
{
    //int prev_root_height = sub_root->height;
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
    updateHeights(A);
    updateHeights(root);
}

void RankTree::llRotate(Node* sub_root)
{
    //int prev_root_height = sub_root->height;
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
    updateHeights(root);
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

void RankTree::updateTreeScores(Node* node)
{
    int num_players_son1 = 0;
    int num_players_son2 = 0;
    int num_players = numPlayers(node->scores, scale);
    double total_sum = num_players * node->key;

    if(!node->son1 && !node->son2)
    {
        for(int i=0;i<scale;i++)
        {
            node->tree_scores[i] = node->scores[i];
        }
    }
    if(!node->son1&& node->son2)
    {
        for(int i=0;i<scale;i++)
        {
            node->tree_scores[i] = node->scores[i] + node->son2->scores[i];
        }
        num_players_son2 = numPlayers(node->son2->tree_scores, scale);
        total_sum += num_players_son2 * node->son2->tree_average_level;
    }
    if(node->son1 && !node->son2)
    {
        for(int i=0;i<scale;i++)
        {
            node->tree_scores[i] = node->scores[i] + node->son1->scores[i];
        }
        num_players_son1 = numPlayers(node->son1->tree_scores, scale);
        total_sum += num_players_son1 * node->son1->tree_average_level;
    }
    if(node->son1 && node->son2)
    {
        for(int i=0;i<scale;i++)
        {
            node->tree_scores[i] = node->scores[i] + node->son1->scores[i] + node->son2->scores[i];
        }
        num_players_son1 = numPlayers(node->son1->tree_scores, scale);
        num_players_son2 = numPlayers(node->son2->tree_scores, scale);
        total_sum += num_players_son1 * node->son1->tree_average_level;
        total_sum += num_players_son2 * node->son2->tree_average_level;
    }
    int total_players = num_players + num_players_son1 + num_players_son2;
    node->tree_average_level = total_sum/total_players;
}

void RankTree::updateHeights(Node* node)
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
        /*
        if(sub_tree_root && sub_tree_root->height == prev_root_height)
        {
            return;
        }
         */
        current->height = new_height;
        updateTreeScores(current);
        current = current->parent;
    }
}