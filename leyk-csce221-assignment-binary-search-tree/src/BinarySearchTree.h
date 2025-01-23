#pragma once

#include <functional> // std::less
#include <iostream>
#include <queue> // std::queue
#include <utility> // std::pair

template <typename K, typename V, typename Comparator = std::less<K>>
class BinarySearchTree
{
  public:
    using key_type        = K;
    using value_type      = V;
    using key_compare     = Comparator;
    using pair            = std::pair<key_type, value_type>;
    using pointer         = pair*;
    using const_pointer   = const pair*;
    using reference       = pair&;
    using const_reference = const pair&;
    using difference_type = ptrdiff_t;
    using size_type       = size_t;

  private:
    struct BinaryNode
    {
        pair element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode( const_reference theElement, BinaryNode *lt, BinaryNode *rt )
          : element{ theElement }, left{ lt }, right{ rt } { }
        
        BinaryNode( pair && theElement, BinaryNode *lt, BinaryNode *rt )
          : element{ std::move( theElement ) }, left{ lt }, right{ rt } { }
    };

    using node           = BinaryNode;
    using node_ptr       = node*;
    using const_node_ptr = const node*;

    node_ptr _root;
    size_type _size;
    key_compare comp;

  public:
  
    // default constructor
    BinarySearchTree() {
        _root = nullptr;
        _size = 0;
    }
    // copy constructor
    BinarySearchTree( const BinarySearchTree & rhs ) {
        // TODO: Clones all the elements from rhs. Preserves the same tree structure.
        if(rhs._root == nullptr) {
            _root = nullptr;
            _size = 0;
        } else {
            _root = clone(rhs._root);
            _size = rhs._size;
        }
    }
    // move constructor
    BinarySearchTree( BinarySearchTree && rhs ) {
        // TODO
        _root = rhs._root;
        _size = rhs._size;
        rhs._root = nullptr;
        rhs._size = 0;

    }
    ~BinarySearchTree() {
        // destructor 
        clear();
    }

    const_reference min() const { return min( _root )->element; }
    const_reference max() const { return max( _root )->element; }
    const_reference root() const {
        return _root->element;
    }
    bool contains( const key_type & x ) const { return contains( x, _root ); }
    value_type & find( const key_type & key ) { return find( key, _root )->element.second; }
    const value_type & find( const key_type & key ) const { return find( key, _root )->element.second; }
    bool empty() const {
        if(_size == 0) {
            return true;
        }
        return false;
    }
    size_type size() const {
        // TODO
        return _size;
    }

    void clear() {
        clear( _root );
        _size = 0;
    }
    void insert( const_reference x ) { insert( x, _root ); }
    void insert( pair && x ) { insert( std::move( x ), _root ); }
    void erase( const key_type & x ) { erase(x, _root); }
    
    // copy assignment
    BinarySearchTree & operator=( const BinarySearchTree & rhs ) {
        // TODO
        if(this != &rhs) {
            clear();
            _root = clone(rhs._root);
            _size = rhs._size;
        }
        return *this;
    }
    // move assignment
    BinarySearchTree & operator=( BinarySearchTree && rhs ) {
        // TODO
        if(this != &rhs) {
            clear();
            _root = rhs._root;
            _size = rhs._size;
            rhs._root = nullptr;
            rhs._size = 0;
        }
        return *this;
    }

  private:
    // copy insert
    void insert( const_reference x, node_ptr & t ) {
        // TODO
        // Base case: tree is empty
        if(t == nullptr) {
            t = new node(x, nullptr, nullptr);
            _size++;
        } 
        //check for less than
        else if (comp(x.first, t->element.first)) {
            insert(x, t->left);
        }
        // check for greater
        else if (comp(t->element.first, x.first)) {
            insert(x, t->right);
        } 
        // if equal, replace
        else {
            t->element.second = x.second;
        }
        
    }
    // move insert
    void insert( pair && x, node_ptr & t ) {
        // "should be same thing as above, but when passing x, pass as an rvalue"
        if(t == nullptr) {
            t = new node(std::move(x), nullptr, nullptr);
            _size++;
        } 
        //check for less than
        else if (comp(x.first, t->element.first)) {
            insert(std::move(x), t->left);
        }
        // check for greater
        else if (comp(t->element.first, x.first)) {
            insert(std::move(x), t->right);
        } 
        // if equal, replace
        else {
            t->element.second = std::move(x.second);
        }
        
    }

    void erase( const key_type & x, node_ptr & t ) {
        // TODO
        // Base case: tree is empty
        if(t == nullptr) {
            return;
        }
        // Recursive steps
        // less first
        if(comp(x, t->element.first)) {
            erase(x, t->left);
        }
        // right
        else if(comp(t->element.first, x)) {
            erase(x, t->right);
        }
        // equal
        else {
            // actual erasing, no children
            if(t->left == nullptr && t->right == nullptr) {
                delete t;
                t = nullptr;
                _size--;
            }
            // 1 child
            else if (t->left == nullptr && t->right != nullptr) { // right alive
                node_ptr temp = t;
                t = t->right;
                delete temp;
                _size--;
            }
            else if (t->left != nullptr && t->right == nullptr) { // left alive
                node_ptr temp = t;
                t = t->left;
                delete temp;
                _size--;
            }
            // 2 children
            else {
                // replace the value with the min of the right subtree
                const_node_ptr min_val = min(t->right);
                t->element = min_val->element;
                erase(min_val->element.first, t->right);
            }
        }


    }

    const_node_ptr min( const_node_ptr t ) const {
        // TODO
        if(t == nullptr) { // Base Case
            return nullptr;
        }
        else if(t->left == nullptr) { // Base Case
            return t;
        }
        else {
            return min(t->left);
        }
    }
    const_node_ptr max( const_node_ptr t ) const {
        // TODO
        if(t == nullptr) { // Base Case
            return nullptr;
        }
        else if(t->right == nullptr) { // Base Case
            return t;
        }
        else {
            return max(t->right);
        }
    }

    bool contains( const key_type & x, const_node_ptr t ) const {
        // TODO
        if(t == nullptr) { // Base Case
            return false;
        } 
        // Recursive
        if(comp(x, t->element.first)) { // search left subtree
            return contains(x, t->left);
        }
        else if(comp(t->element.first, x)) { // search right subtree
            return contains(x, t->right);
        }
        else { // else means its equal, return ptr
            return true;
        }
    }
    node_ptr find( const key_type & key, node_ptr t ) {
        // TODO: 
        /*    Private find helper. Seeks the correct position of the node with key key. 
        Returns a pointer to the node if it exists in the tree and nullptr otherwise. 
        Note: when searching for the key, the elements in nodes are key-value pairs. 
        This means that key should be compared with the first value of the pair 
        (can be accessed with element.first).   */
        if(t == nullptr) { // Base Case
            return nullptr;
        } 
        // Recursive
        if(comp(key, t->element.first)) { // search left subtree
            return find(key, t->left);
        }
        else if(comp(t->element.first, key)) { // search right subtree
            return find(key, t->right);
        }
        else { // else means its equal, return ptr
            return t;
        }
    }
    const_node_ptr find( const key_type & key, const_node_ptr t ) const {
        // TODO: 
        if(t == nullptr) { // Base Case
            return nullptr;
        } 
        // Recursive
        if(comp(key, t->element.first)) { // search left subtree
            return find(key, t->left);
        }
        else if(comp(t->element.first, key)) { // search right subtree
            return find(key, t->right);
        }
        else { // else means its equal, return ptr
            return t;
        }
    }

    void clear( node_ptr & t ) {
        // TODO 
        // Base case: tree is empty
        if(t == nullptr) {
            return;
        }
        clear(t->left);
        clear(t->right);
        delete t;
        t = nullptr;
        _size = 0;
    }
    
    node_ptr clone ( const_node_ptr t ) const {
        // TODO: Private clone helper. Recursively clones every node at and under t,
        // and returns the node at the root of the cloned subtree. 
        // This function is used in Copy Construction and Copy Assignment.

        // pre order traversal
        if(t == nullptr) {
            return nullptr;
        }
        return new node(t->element, clone(t->left), clone(t->right));
    }

  public:
    template <typename KK, typename VV, typename CC>
    friend void printLevelByLevel( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out );

    template <typename KK, typename VV, typename CC>
    friend std::ostream& printNode(std::ostream& o, const typename BinarySearchTree<KK, VV, CC>::node& bn);

    template <typename KK, typename VV, typename CC>
    friend void printTree( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out );

    template <typename KK, typename VV, typename CC>
    friend void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream & out, unsigned depth );

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr node, 
        std::ostream & out,
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev
    );

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        const BinarySearchTree<KK, VV, CC> & bst, 
        std::ostream & out
    );
};

template <typename KK, typename VV, typename CC>
std::ostream& printNode(std::ostream & o, const typename BinarySearchTree<KK, VV, CC>::node & bn) {
    return o << '(' << bn.element.first << ", " << bn.element.second << ')';
}

template <typename KK, typename VV, typename CC>
void printLevelByLevel( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out = std::cout ) {
    using node = typename BinarySearchTree<KK, VV, CC>::node;
    using node_ptr = typename BinarySearchTree<KK, VV, CC>::node_ptr;
    using const_node_ptr = typename BinarySearchTree<KK, VV, CC>::const_node_ptr;
    
    // TODO -- Guide in Instructions
    // level-by-level traversal. Prints Key-value pairs and any null values
    node_ptr temp;
    std::queue<node_ptr> q;
    std::stringstream ss;
    int size;
    bool isNull;

    q.push(bst._root);
    size = q.size();

    while (true) {
        isNull = true;

        for(int i = 0; i < size; i++) {
            temp = q.front();
            q.pop();

            if (temp == nullptr) {
                ss << "null ";
                q.push(nullptr); // next level will have 2 null children
                q.push(nullptr); // dont change isNull because no children
            } else {
                ss << "(" << temp->element.first << ", " << temp->element.second << ") ";
                q.push(temp->left); // add children to queue
                q.push(temp->right);
                isNull = false; 
            }
        }

        if(isNull) {
            break;
        }
    
        size = q.size();
        out << ss.str() << std::endl;
        ss.str("");
    }

}

template <typename KK, typename VV, typename CC>
void printTree( const BinarySearchTree<KK, VV, CC> & bst, std::ostream & out = std::cout ) { printTree<KK, VV, CC>(bst._root, out ); }

template <typename KK, typename VV, typename CC>
void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream & out, unsigned depth = 0 ) {
    if (t != nullptr) {
        printTree<KK, VV, CC>(t->right, out, depth + 1);
        for (unsigned i = 0; i < depth; ++i)
            out << '\t';
        printNode<KK, VV, CC>(out, *t) << '\n';
        printTree<KK, VV, CC>(t->left, out, depth + 1);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr node, 
    std::ostream & out,
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev = nullptr
) {
    if(node) {
        std::hash<KK> khash{};

        out << "\t" "node_" << (uint32_t) khash(node->element.first)
            << "[label=\"" << node->element.first 
            << " [" << node->element.second << "]\"];" << std::endl;
        
        if(prev)
            out << "\tnode_" << (uint32_t) khash(prev->element.first) <<  " -> ";
        else
            out << "\t";
        
        out << "node_" << (uint32_t) khash(node->element.first) << ";" << std::endl;
    
        vizTree<KK, VV, CC>(node->left, out, node);
        vizTree<KK, VV, CC>(node->right, out, node);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    const BinarySearchTree<KK, VV, CC> & bst, 
    std::ostream & out = std::cout
) {
    out << "digraph Tree {" << std::endl;
    vizTree<KK, VV, CC>(bst._root, out);
    out << "}" << std::endl;
}
