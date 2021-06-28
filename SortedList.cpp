#include "SortedList.h"

SortedList::SortedList():size(0),start_node(&(node(null,null)))//not sure exactly how to make node through init list
{};

SortedList::SortedList(const SortedList& sorted_list):size(sorted_list.size),start_node(&(node(null,null)))
{
    node *node_to_be_copied = sorted_list.start_node;
    node *node_to_be_copied_to = start_node;
    while(node_to_be_copied->next!=null) //need to take care of start node logic
    {
        node_to_be_copied=node_to_be_copied->next;
        node_to_be_copied_to->next = &(node(node_to_be_copied->data,node_to_be_copied->next));
        node_to_be_copied_to = node_to_be_copied_to->next;
    }
};

SortedList& SortedList::operator=(const SortedList& sorted_list)
{
    if(this==&sorted_list)  //this looks weird but i just copied what is in lecture 8 slide 29
    {
        return *this;
    }
    //not sure if this will cause a memeory leak, dont have a better idea, i think we can just check at debuging
    start_node = &(node(null,null));
    size = sorted_list.size;
    //does this part count as שכפול קוד?
    node *node_to_be_copied = sorted_list.start_node;
    node *node_to_be_copied_to = start_node;
    while(node_to_be_copied->next!=null)
    {
        node_to_be_copied=node_to_be_copied->next;
        node_to_be_copied_to->next = &(node(node_to_be_copied->data,node_to_be_copied->next));
    }
    return *this;
}

void SortedList::insert(const int& new_element)  //should be class T
{
    node *node_to_compare_to = start_node->next;
    while(new_element>node_to_compare_to->data)
    {
        node_to_compare_to = node_to_compare_to->next;
    }
    node *node_to_be_after_new_element = node_to_compare_to->next;
    const int new_element_copy = new_element;//should be class T
    node_to_compare_to->next = &(node(new_element_copy,node_to_be_after_new_element));
    size++;
}


int SortedList::length()const
{
    return size;
}


SortedList::node::node(int data,node* next=null):data(data),next(next)
{};


SortedList::const_iterator::const_iterator(const SortedList* const sorted_list, int index): sorted_list(sorted_list), index(index)
{};