//template<class T>

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
    ~SortedList() = default;
    SortedList(const SortedList& sorted_list);
    SortedList& operator=(const SortedList& sorted_list);

    void insert(const int& new_element);
    void remove(const_iterator iterator);
    int length()const;

};

class SortedList::node
{
    public:
    int data;
    node* next;
    node(int data,node* next=null);
    ~node() = default;
};

class SortedList::const_iterator
{
    private:
    const SortedList* sorted_list;
    int index;
    friend class SortedList;
    //?
    const_iterator(const SortedList* const sortedlist, int index);

    public:
    const_iterator(const &const_iterator);
    const_iterator& operator=(const &const_iterator);
    const int& operator*() const;
    const_iterator& operator++(); // prefix (++it)
    const_iterator operator++(int);
    bool operator==(const const_iterator& other_iterator) const;
    bool operator!=(const const_iterator& other_iterator) const;
};