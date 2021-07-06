#include <stdexcept>
namespace mtm

{
    template<class T>
    class SortedList
    {
        //not sure about division to private and public
        private:
        int size;
        class node;
        node *start_node;

        public:
        class const_iterator;
        const_iterator begin()const;
        const_iterator end()const;
        
        SortedList();
        ~SortedList(); // not sure about default
        SortedList(const SortedList& sorted_list);
        SortedList& operator=(const SortedList& sorted_list);

        void insert(const T& new_element);  
        void remove(const_iterator iterator);
        int length()const;

        //not sure about this part at all
        template<class Predicate>
        SortedList filter(Predicate pred);
        template<class Appliance>
        SortedList apply(Appliance app);
        //

    };


    template<class T>
    class SortedList<T>::node
    {
        public:
        T data;  
        node *next;
        node();
        node(T data,node *next);//should be class T  - also should this be explicit?
        ~node(); // should i add "=default"?
    };

    template<class T>
    class SortedList<T>::const_iterator
    {
        private:
        const SortedList* sorted_list;
        int index;
        friend class SortedList;
        //?
        const_iterator(const SortedList* const sorted_list, int index); //should this be default?

        public:
        const_iterator(const const_iterator& iterator);
        const_iterator operator=(const const_iterator&);
        const T& operator*() const;
        const_iterator& operator++(); // prefix (++it)
        const_iterator operator++(int);
        bool operator==(const const_iterator& other_iterator) const;
        bool operator!=(const const_iterator& other_iterator) const;
    };


    template<class T>
    SortedList<T>::SortedList() :size(0), start_node(nullptr)
    {}

    template<class T>
    SortedList<T>::~SortedList() 
    {
        if (start_node != nullptr)
        {
            delete(start_node);
        }
    }

    template<class T>
    SortedList<T>::SortedList(const SortedList& sorted_list) : size(sorted_list.size), start_node(nullptr) //the bug is here, start node dosnt update correctly
    {
        if (sorted_list.start_node == nullptr)
        {
            return;
        }  //not sure this part above works
        
        node *new_start = new node(sorted_list.start_node->data, nullptr);
        start_node = new_start;
        node *node_to_be_copied = sorted_list.start_node->next;
        node *node_to_be_copied_to = new_start;
        while (node_to_be_copied != nullptr) //need to take care of start node logic
        {
            node *node_copy = new node(node_to_be_copied->data, nullptr);
            node_to_be_copied_to->next = node_copy;
            node_to_be_copied_to = node_to_be_copied_to->next;
            node_to_be_copied = node_to_be_copied->next;

        }
        
    }

    template<class T>
    SortedList<T>& SortedList<T>::operator=(const SortedList& sorted_list)
    {
        if (this == &sorted_list)  
        {
            return *this;
        }
        size = sorted_list.size;
        if (start_node != nullptr)
        {
            delete(start_node);
        }
        if (sorted_list.start_node == nullptr)
        {
            start_node = nullptr;
            return *this;
        }
        node *new_start = new node(sorted_list.start_node->data, nullptr);
        start_node = new_start;

        node *node_to_be_copied = sorted_list.start_node->next;
        node *node_to_be_copied_to = new_start;
        while (node_to_be_copied != nullptr)
        {
            node *node_copy = new node(node_to_be_copied->data, nullptr);
            node_to_be_copied_to->next = node_copy;
            node_to_be_copied_to = node_to_be_copied_to->next;
            node_to_be_copied = node_to_be_copied->next;

        }
        return *this;
    }

    template<class T>
    void SortedList<T>::insert(const T& new_element) 
    {
        size++;
        if(start_node == nullptr)
        {
            node* new_node = new node(new_element, nullptr);
            start_node = new_node;
            return;
        }
        node *node_to_compare_to = start_node;
        T new_element_copy = new_element;  
        if (new_element < start_node->data)
        {
            node* new_node = new node(new_element_copy, start_node);
            start_node = new_node;
            return;
        }
        while ((node_to_compare_to->next != nullptr) && (node_to_compare_to->next->data < new_element))
        {
            node_to_compare_to = node_to_compare_to->next;
        }
        node *node_to_be_after_new_element = node_to_compare_to->next;
        node *new_node = new node(new_element_copy, node_to_be_after_new_element);
        node_to_compare_to->next = new_node;
    }

    template<class T>
    int SortedList<T>::length()const
    {
        return size;
    }


    template<class T>
    SortedList<T>::node::node(T data, node* next) :data(data), next(next)
    {}


    template<class T>
    SortedList<T>::node::~node()
    {
        if (next != nullptr)
        {
            delete next;
        }
    }

    template<class T>
    SortedList<T>::const_iterator::const_iterator(const SortedList* const sorted_list, int index) : sorted_list(sorted_list), index(index)
    {}

    //_______________________________________________________________________________________________________

    //_______________________________________________________________________________________________________
    template<class T>
    class SortedList<T>::const_iterator SortedList<T>::begin()const
    {
        const_iterator iter = const_iterator(this, 1);
        return iter;
    }

    template<class T>
    class SortedList<T>::const_iterator SortedList<T>::end()const
    {
        const_iterator iter = const_iterator(this, size+1);
        return iter;
    }

    template<class T>
    void SortedList<T>::remove(const_iterator iterator)
    {
        if(start_node == nullptr)
        {
            return;
        }
        size--;
        node *node_before_the_one_removed = start_node;
        if (iterator.index == 1)
        {
            node* temp = start_node;
            start_node = start_node->next;
            temp->next = nullptr;
            delete(temp);
            return;
        }
        for (int i = 1; i+1 < iterator.index; i++)
        {
            node_before_the_one_removed = node_before_the_one_removed->next; //not sure about the math here
        }
        node *node_to_remove = node_before_the_one_removed->next;
        node *node_after_the_one_removed = node_to_remove->next;
        node_to_remove->next = nullptr;
        delete(node_to_remove);
        node_before_the_one_removed->next = node_after_the_one_removed; //really hope this makes the one to remove destroy itself
    }

    template<class T>
    template<class Predicate>
    SortedList<T> SortedList<T>::filter(Predicate pred)
    {
        SortedList<T> filtered_list;
        if(start_node == nullptr)
        {
            return filtered_list;
        }
        node *current_node_to_filter = start_node;
        for (int i = 0; i < this->size; i++)
        {
            if (pred(current_node_to_filter->data) == true)
            {
                filtered_list.insert(current_node_to_filter->data);
            }
            current_node_to_filter = current_node_to_filter->next;
        }
        return filtered_list;
    }

    template<class T>
    template<class Appliance>
    SortedList<T> SortedList<T>::apply(Appliance app)
    {
        SortedList<T> applied_list;

        if(start_node == nullptr)
        {
            return applied_list;
        }
        node *current_node_to_apply = start_node;
        for (int i = 0; i < this->size; i++)
        {
            applied_list.insert(app(current_node_to_apply->data));
            current_node_to_apply = current_node_to_apply->next;
        }
        

        return applied_list;
    }


    template<class T>
    SortedList<T>::const_iterator::const_iterator(const const_iterator& iterator) : sorted_list(iterator.sorted_list), index(iterator.index)
    {}

    template<class T>
    class SortedList<T>::const_iterator SortedList<T>::const_iterator::operator=(const const_iterator& iterator)
    {
        if (this == &iterator)
        {
            return *this;
        }
        sorted_list = iterator.sorted_list;
        index = iterator.index;
        return *this;
    }

    template<class T>
    const T& SortedList<T>::const_iterator::operator*() const
    {
        node *current_node = sorted_list->start_node;
        for (int i = 0; i < index-1; i++)
        {
            current_node = current_node->next;
        }        
        return current_node->data;
    }

    template<class T>
    class SortedList<T>::const_iterator& SortedList<T>::const_iterator::operator++()
    {
        if (this->index  > this->sorted_list->size)
        {
            throw std::out_of_range("out_of_range");
        }
        ++index;
        return *this;
    }

    template<class T>
    class SortedList<T>::const_iterator SortedList<T>::const_iterator::operator++(int)
    {
        if (this->index  > this->sorted_list->size)
        {
            throw std::out_of_range("out_of_range");
        }
        const_iterator result = *this;
        ++*this;
        return result;
    }

    template<class T>
    bool SortedList<T>::const_iterator::operator==(const const_iterator& other_iterator) const
    {
        bool are_same = (index == other_iterator.index && sorted_list == other_iterator.sorted_list);
        bool are_both_end = (index == this->sorted_list->size+1 && other_iterator.index == other_iterator.sorted_list->size+1);
        return (are_same||are_both_end);
    }

    template<class T>
    bool SortedList<T>::const_iterator::operator!=(const const_iterator& other_iterator) const
    {
        return !(*this == other_iterator);
    }
}