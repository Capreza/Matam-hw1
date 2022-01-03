#ifndef HW2_LIST_NODE_H
#define HW2_LIST_NODE_H

template<class T>
class LinkedListNode
{
private:
    const int key;
    T* data;
    LinkedListNode<T>* next;
    friend class Hashtable;

public:
    LinkedListNode(): key(-1), data(nullptr), next(nullptr) {}
    LinkedListNode(int key, T* data): key(key), data(data), next(nullptr) {}
    ~LinkedListNode() ///check later if we need this
    {
        if(data)
        {
            delete data;
        }
    }
};



#endif //HW1_NODE_H
