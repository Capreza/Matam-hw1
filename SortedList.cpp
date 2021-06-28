#include "SortedList.h"

template<class T>
SortedList<T>::SortedList():size(0),start_node(&(node(null,null)))//not sure exactly how to make node through init list
{}

template<class T>
SortedList<T>::SortedList(const SortedList& sorted_list):size(sorted_list.size),start_node(&(node(null,null)))
{
    node *node_to_be_copied = sorted_list.start_node;
    node *node_to_be_copied_to = start_node;
    while(node_to_be_copied->next!=null) //need to take care of start node logic
    {
        node_to_be_copied=node_to_be_copied->next;
        node_to_be_copied_to->next = &(node(node_to_be_copied->data,node_to_be_copied->next));
        node_to_be_copied_to = node_to_be_copied_to->next;
    }
}

template<class T>
SortedList<T>& SortedList<T>::operator=(const SortedList& sorted_list)
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

template<class T>
void SortedList<T>::insert(const T& new_element)  //should be class T
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

template<class T>
int SortedList<T>::length()const
{
    return size;
}

template<class T>
SortedList<T>::node::node(T data,node* next):data(data),next(next)
{}

template<class T>
SortedList<T>::const_iterator::const_iterator(const SortedList* const sorted_list, int index): sorted_list(sorted_list), index(index)
{}

//_______________________________________________________________________________________________________

//_______________________________________________________________________________________________________
template<class T>
class SortedList<T>::const_iterator SortedList<T>::begin()const
{
    return const_iterator(this,0);
}

template<class T>
class SortedList<T>::const_iterator SortedList<T>::end()const
{
    return const_iterator(this,size);
}

template<class T>
void SortedList<T>::remove(const_iterator iterator)
{
    node *node_before_the_one_removed = this->start_node;
    for(int i = 0 ; i<iterator.index;i++)
    {
        node_before_the_one_removed = node_before_the_one_removed->next; //not sure about the math here
    }
    node *node_to_remove = node_before_the_one_removed->next;
    node *node_after_the_one_removed = node_to_remove->next;
    node_before_the_one_removed->next = node_after_the_one_removed; //really hope this makes the one to remove destroy itself
}

template<class T>
template<class Predicate>
SortedList<T>& SortedList<T>::filter(Predicate pred)
{
    SortedList filtered_list;
    node *current_node_to_filter = this->start_node->next;
    for(int i=0;i<this->size;i++)
    {
        if(pred(current_node_to_filter->data)==true)
        {
            filtered_list.insert(current_node_to_filter->data);
        }
        current_node_to_filter = current_node_to_filter->next;
    }
    return filtered_list;
}

template<class T>
template<class Appliance>
SortedList<T>& SortedList<T>::apply(Appliance app)
{
    SortedList applied_list;
    node *current_node_to_apply = this->start_node->next;
    for(int i=0;i<this->size;i++)
    {
        applied_list.insert(app(current_node_to_apply->data));
        current_node_to_apply = current_node_to_apply->next;
    }
    return applied_list;
}





//_______________________________________________________________________________________________________

//_______________________________________________________________________________________________________
template<class T>
SortedList<T>::const_iterator::const_iterator(const const_iterator& iterator): sorted_list(iterator.sorted_list), index(iterator.index)
{}

template<class T>
class SortedList<T>::const_iterator& SortedList<T>::const_iterator::operator=(const const_iterator& iterator)
{
    if (this == &iterator)
    {
        return *this;
    }
    index = iterator.index;
    return *this;
}

template<class T>
const T& SortedList<T>::const_iterator::operator*() const 
{
    node *current_node = ((this->sorted_list)->start_node);
    for(int i = 0; i < index; i++)
    {
        current_node = current_node->next;
    }
    return current_node->data;
}

template<class T>
class SortedList<T>::const_iterator &SortedList<T>::const_iterator::operator++()
{
    ++index;
    return *this;
}

template<class T>
class SortedList<T>::const_iterator SortedList<T>::const_iterator::operator++(int)
{
    const_iterator result = *this;
    ++*this;
    return result;
}

template<class T>
bool SortedList<T>::const_iterator::operator==(const const_iterator& other_iterator) const
{
    return index == other_iterator.index;
}

template<class T>
bool SortedList<T>::const_iterator::operator!=(const const_iterator& other_iterator) const
{
    return !(*this == other_iterator);
}