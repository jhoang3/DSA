#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}
        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

    public:
        basic_iterator() { node = nullptr; };
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        reference operator*() const {
            // TODO: returning a reference to the value in the managed node
            return node->data;     
        }
        pointer operator->() const {
            // TODO: return a pointer to the node's managed data, its memory
            return &(node->data);
        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            // TODO
            node = node->next;
            return *this;
        }
        // Postfix Increment: a++
        basic_iterator operator++(int) {
            // TODO: Advance the iterator so that it points to the next node in the list. 
            //  Return a copy of the iterator from before it was advanced.
            basic_iterator copy = *this;
            node = node->next;
            return copy;
        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            // TODO: Retreat the iterator so that it points to the previous node in the list. 
            // Return a reference to this iterator.
            node = node->prev;
            return *this;
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            // TODO
            basic_iterator copy = *this;
            node = node->prev;
            return copy;
        }

        bool operator==(const basic_iterator& other) const noexcept {
            // TODO: Compare two iterators and determine if they are equivalent.
            return node == other.node;
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            // TODO
            // Description: Compare two iterators and determine if they are not equivalent.
            return node != other.node;
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List() { // default constructor
        // TODO - Don't forget to initialize the list beforehand
        _size = 0;
        head.next = &tail;
        tail.prev = &head;

    }
    List( size_type count, const T& value ) { //  inserted copies
        // TODO - Don't forget initialize the list beforehand
        // Constructs a linked list with a copy of the contents of other.
        _size = count;
        head.next = &tail;
        tail.prev = &head;
        
        while(count > 0) {
            Node* node = new Node(value);
            node->next = &tail;
            node->prev = tail.prev;
            tail.prev->next = node;
            tail.prev = node;
            count--;
        }

    }
    explicit List( size_type count ) { // default inserted
        // TODO - Don't forget initialize the list beforehand
        // we dont dedicate head and tail on heap, they live on stack
        //head = Node();
        //tail = Node();
        _size = count;
        // head's next is tail, tail's prev is head
        // we dont use -> because head and tail are not pointers
        head.next = &tail;
        tail.prev = &head;

        while(count > 0) {
            Node* node = new Node(T());
            node->next = &tail;
            node->prev = tail.prev;
            tail.prev->next = node;
            tail.prev = node;
            count--;
        }
        
    }
    List( const List& other ) { // copy constructor
        // TODO - Don't forget initialize the list beforehand
        _size = other._size;
        head.next = &tail;
        tail.prev = &head;
        size_t count = other._size;

        Node* nextNode = other.head.next;

        while (count > 0) {
            Node* node = new Node(nextNode->data);
            node->next = &tail;
            node->prev = tail.prev;
            tail.prev->next = node;
            tail.prev = node;
            nextNode = nextNode->next;
            count--;
        }
    }
   
    List( List&& other ) { // move constructor
        // TODO - Don't forget iniftialize the list beforehand
            _size = other._size;
            head.next = other.head.next;
            head.next->prev = &head;
            tail.prev = other.tail.prev;
            tail.prev->next = &tail;
            other.head.next = &other.tail;
            other.tail.prev = &other.head;
            other._size = 0;
        
    }
    ~List() {
        // TODO: The destructors of the elements are called and the used storage is deallocated. 
        //Note, that if the elements are pointers, the pointed-to objects are not destroyed.
        // Node* node = head.next;
        // while(node != &tail) {
        //     Node* next = node->next;
        //     delete node;
        //     node = next;
        // }
        // head.next = &tail;
        // tail.prev = &head;
        // _size = 0;

        clear();

    }
    List& operator=( const List& other ) { // copy assignment
        // TODO
        if(this != &other) {
            clear();
            _size = other._size;
            head.next = &tail;
            tail.prev = &head;
            size_t count = other._size;

            Node* nextNode = other.head.next;

            while (count > 0) {
                Node* node = new Node(nextNode->data);
                node->next = &tail;
                node->prev = tail.prev;
                tail.prev->next = node;
                tail.prev = node;
                nextNode = nextNode->next;
                count--;
            }

        }
        return *this;
    }
    List& operator=( List&& other ) noexcept { // move assignment
        // TODO
        // attach ends of this to other
        if(this != &other) {
            clear();
            _size = other._size;
            head.next = other.head.next;
            head.next->prev = &head;
            tail.prev = other.tail.prev;
            tail.prev->next = &tail;
            other.head.next = &other.tail;
            other.tail.prev = &other.head;
            other._size = 0;
        }
        return *this;
    }

    reference front() {
        // TODO
        return(head.next->data);
    }
    const_reference front() const {
        // TODO
        return(head.next->data);
    }
	
    reference back() {
        // TODO
        return(tail.prev->data);
    }
    const_reference back() const {
        // TODO
        return(tail.prev->data);
    }
	
    iterator begin() noexcept {
        // TODO
        if(head.next == &tail) {
            // return a const iterator to tail
            return iterator(&tail);
        }
        return iterator(head.next);
    }
    const_iterator begin() const noexcept {
        // TODO
        if(head.next == &tail) {
            // return a const iterator to tail
            return const_iterator(&tail);
        }
        return const_iterator(head.next);
    }
    const_iterator cbegin() const noexcept {
        // TODO
        if(head.next == &tail) {
            // return a const iterator to tail
            return const_iterator(&tail);
        }
        return const_iterator(head.next);
    }

    iterator end() noexcept {
        // TODO
        return iterator(&tail);
    }
    const_iterator end() const noexcept {
        // TODO
        return const_iterator(&tail);
    }
    const_iterator cend() const noexcept {
        // TODO
        return const_iterator(&tail);
    }

    bool empty() const noexcept {
        // TODO
        if(head.next == &tail) {
            return true;
        }
        return false;
    }

    size_type size() const noexcept {
        // TODO: returns number of elements in the list
        return _size;
    }

    void clear() noexcept {
        // TODO: Clears the contents of the linked list. The list should be left in an empty state afterwards.
        while(_size > 0) {
            pop_back();
        }

    }

    iterator insert( const_iterator pos, const T& value ) {
        // TODO
        Node* node = new Node(value);
        node->next = pos.node;
        node->prev = pos.node->prev;
        pos.node->prev->next = node;
        pos.node->prev = node;
        _size++;
        return iterator(node);
    }
    iterator insert( const_iterator pos, T&& value ) {
        // TODO: insert function using move semantics
        Node* node = new Node(std::move(value));
        node->next = pos.node;
        node->prev = pos.node->prev;
        pos.node->prev->next = node;
        pos.node->prev = node;
        _size++;
        return iterator(node);
    }

    iterator erase( const_iterator pos ) {
        // TODO
        // set the previous node's next to the next of pos
        pos.node->prev->next = pos.node->next;
        pos.node->next->prev = pos.node->prev;
        iterator copy = iterator(pos.node->next);
        delete pos.node;
        _size--;
        return iterator(copy);
    }

    void push_back( const T& value ) {
        // TODO
        Node* node = new Node(value);
        node->next = &tail;
        node->prev = tail.prev;
        tail.prev->next = node;
        tail.prev = node;
        _size++;
    }
    void push_back( T&& value ) {
        // TODO
        insert(iterator(&tail), std::move(value));
    }

    void pop_back() {
        // TODO
        erase(iterator(tail.prev));
    }
	
    void push_front( const T& value ) {
        // TODO
        Node* node = new Node(value);
        node->next = head.next;
        node->prev = &head;
        head.next = node;
        node->next->prev = node;
        _size++;

    }
	void push_front( T&& value ) {
        // TODO
        insert(iterator(head.next), std::move(value));
    }

    void pop_front() {
        // TODO
        erase(iterator(head.next));
    }

    /*
      You do not need to modify these methods!
      
      These method provide the non-const complement 
      for the const_iterator methods provided above.
    */
    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/
 
namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}