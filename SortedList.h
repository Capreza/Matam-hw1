
template<class T>
class SortedList
{
    //not sure about division to private and public
    private:
    int size;
    class node;
    node* start_node;
    const static int null = 0;

  
    //not sure about this part at all
    template<class Predicate>
    SortedList& filter(Predicate pred);
    template<class Appliance>
    SortedList& apply(Appliance app);
    //
    public:
    class const_iterator;
    const_iterator begin()const;
    const_iterator end()const;
    
    SortedList();
    ~SortedList() = default; // not sure about default
    SortedList(const SortedList& sorted_list);
    SortedList& operator=(const SortedList& sorted_list);

    void insert(const T& new_element);  
    void remove(const_iterator iterator);
    int length()const;

};


template<class T>
class SortedList<T>::node
{
    public:
    T data;  
    node* next;
    node(T data,node* next=null);//should be class T  - also should this be explicit?
    ~node()=default; // should i add "=default"?
};

template<class T>
class SortedList<T>::const_iterator
{
    private:
    const SortedList* const sorted_list;
    int index;
    friend class SortedList;
    //?
    const_iterator(const SortedList* const sorted_list, int index); //should this be default?

    public:
    const_iterator(const const_iterator& iterator);
    const_iterator& operator=(const const_iterator&);
    const T& operator*() const;
    const_iterator& operator++(); // prefix (++it)
    const_iterator operator++(int);
    bool operator==(const const_iterator& other_iterator) const;
    bool operator!=(const const_iterator& other_iterator) const;
};