/* ✔ */

#pragma once

#include <AH/Debug/Debug.hpp>
#include <AH/Math/MinMaxFix.hpp>
#include <stdlib.h>

#include <AH/STL/iterator>

/// @addtogroup AH_Containers
/// @{

/**
 * @brief   A class for doubly linked lists.
 * 
 * @tparam  Node
 *          The type of the nodes of the list.
 */
template <class Node>
class DoublyLinkedList {
  public:
    /// Base class for doubly linked list iterators
    template <class INode>
    class node_iterator_base {
      public:
        node_iterator_base(INode *node) : node(node) {}

        bool operator!=(const node_iterator_base &rhs) const {
            return node != rhs.node;
        }

        bool operator==(const node_iterator_base &rhs) const {
            return !(*this != rhs);
        }

        INode &operator*() const {
            // TODO: check node != nullptr
            return *node;
        }

        INode *operator->() const {
            // TODO: check node != nullptr
            return node;
        }

      protected:
        INode *node;
    };

    /// Forward bidirectional doubly linked list iterator
    template <class INode>
    class node_iterator : public node_iterator_base<INode> {
      public:
        node_iterator(INode *node) : node_iterator_base<INode>(node) {}

        using difference_type = long;
        using value_type = INode;
        using pointer = INode *;
        using reference = INode &;
        using iterator_category = std::bidirectional_iterator_tag;

        /// Prefix increment operator
        node_iterator &operator++() {
            // TODO: check node != nullptr
            this->node = this->node->next;
            return *this;
        }

        /// Prefix decrement operator
        node_iterator &operator--() {
            // TODO: check node != nullptr
            this->node = this->node->previous;
            return *this;
        }
    };

    /// Reverse bidirectional doubly linked list iterator
    template <class INode>
    class reverse_node_iterator : public node_iterator_base<INode> {
      public:
        reverse_node_iterator(INode *node) : node_iterator_base<INode>(node) {}

        using difference_type = long;
        using value_type = INode;
        using pointer = INode *;
        using reference = INode &;
        using iterator_category = std::bidirectional_iterator_tag;

        /// Prefix increment operator
        reverse_node_iterator &operator++() {
            // TODO: check node != nullptr
            this->node = this->node->previous;
            return *this;
        }

        /// Prefix decrement operator
        reverse_node_iterator &operator--() {
            // TODO: check node != nullptr
            this->node = this->node->next;
            return *this;
        }
    };

    using iterator = node_iterator<Node>;
    using const_iterator = node_iterator<const Node>;
    using reverse_iterator = reverse_node_iterator<Node>;
    using const_reverse_iterator = reverse_node_iterator<const Node>;

    /**
     * @brief   Append a node to a linked list.
     * 
     * @param   node
     *          A pointer to the node to be appended.
     */
    void append(Node *node) {
        if (first == nullptr)
            first = node;
        node->previous = last;
        if (node->previous != nullptr)
            node->previous->next = node;
        last = node;
        node->next = nullptr;
    }

    /**
     * @brief   Append a node to a linked list.
     * 
     * @param   node
     *          A reference to the node to be appended.
     */
    void append(Node &node) { append(&node); }

    /**
     * @brief   Insert a node before another node.
     * 
     * @param   toBeInserted
     *          The new node to be inserted.
     * @param   before
     *          The node to insert the new node before. It must be in the list
     *          already.
     */
    void insertBefore(Node *toBeInserted, Node *before) {
        if (before == first)
            first = toBeInserted;
        else
            before->previous->next = toBeInserted;
        toBeInserted->previous = before->previous;
        toBeInserted->next = before;
        before->previous = toBeInserted;
    }

    /// @see    insertBefore(Node *, Node *)
    void insertBefore(Node &toBeInserted, Node &before) {
        insertBefore(&toBeInserted, &before);
    }

    /**
     * @brief   Insert a new node at the correct location into a sorted list.
     * 
     * @param   node 
     *          The new node to be inserted.
     * @param   cmp
     *          The function to order the nodes.
     * @tparam  Compare
     *          A functor that compares two Nodes and returns a boolean.
     */
    template <class Compare>
    void insertSorted(Node *node, Compare cmp) {
        iterator it = this->begin();
        iterator end = this->end();
        while (it != end) {
            if (cmp(*node, *it)) {
                insertBefore(*node, *it);
                return;
            }
            ++it;
        }
        append(node);
    }

    /**
     * @brief   Insert a new node at the correct location into a sorted list, 
     *          using `operator<`.
     * 
     * @param   node 
     *          The new node to be inserted.
     */
    void insertSorted(Node *node) {
        insertSorted(node, [](Node &lhs, Node &rhs) { return lhs < rhs; });
    }

    /**
     * @brief   Remove a node from the linked list.
     * 
     * @param   node
     *          A pointer to the node to be removed.
     */
    void remove(Node *node) {
        if (node->previous != nullptr)
            node->previous->next = node->next;
        if (node == last)
            last = node->previous;
        if (node->next != nullptr)
            node->next->previous = node->previous;
        if (node == first)
            first = node->next;
        node->previous = nullptr;
        node->next = nullptr;
    }

    /**
     * @brief   Remove a node from the linked list.
     * 
     * @param   node
     *          A reference to the node to be removed.
     */
    void remove(Node &node) { remove(&node); }

    /**
     * @brief   Move down the given node in the linked list.
     * 
     * For example: moving down node `C`:
     * ```
     * Before:  ... → A → B → C → D → ...
     * After:   ... → A → C → B → D → ...
     * ```
     * @param   node
     *          A pointer to the node to be moved down.
     */
    void moveDown(Node *node) {
        Node *nodeB = node->previous;
        if (nodeB == nullptr) // Can't move first node further down
            return;
        Node *nodeA = nodeB->previous;
        Node *nodeD = node->next;

        if (nodeA != nullptr)
            nodeA->next = node;
        else
            first = node;
        nodeB->next = nodeD;
        nodeB->previous = node;
        node->next = nodeB;
        node->previous = nodeA;
        if (nodeD != nullptr)
            nodeD->previous = nodeB;
        else
            last = nodeB;
    }

    /**
     * @brief   Move down the given node in the linked list.
     * 
     * For example: moving down node `C`:
     * ```
     * Before:  ... → A → B → C → D → ...
     * After:   ... → A → C → B → D → ...
     * ```
     * @param   node
     *          A reference to the node to be moved down.
     */
    void moveDown(Node &node) { moveDown(&node); }

    /** 
     * @brief   Check if the linked list could contain the given node.
     * 
     * @retval  true
     *          The given node is part of some linked list or it is the first
     *          node of the given linked list.  
     *          It could be that the node is part of a different linked list
     *          if it was ever added to a different list.
     *          However, **if this function returns true and the node was never
     *          added to another linked list, it means that this linked list 
     *          contains the given node**.
     * @retval  false
     *          The given node is not part of any linked list, or it is the 
     *          only element of a different linked list.
     */
    bool couldContain(const Node *node) const {
        return node && (node == first || node->next != nullptr ||
                        node->previous != nullptr);
    }

    /// @copydoc DoublyLinkedList::couldContain(const Node *) const
    bool couldContain(const Node &node) const { return couldContain(&node); }

    iterator begin() { return {first}; }
    iterator end() { return {nullptr}; }

    const_iterator begin() const { return {first}; }
    const_iterator end() const { return {nullptr}; }

    reverse_iterator rbegin() { return {last}; }
    reverse_iterator rend() { return {nullptr}; }

    const_reverse_iterator rbegin() const { return {last}; }
    const_reverse_iterator rend() const { return {nullptr}; }

    /// Get a pointer to the first node.
    Node *getFirst() const { return first; }
    /// Get a pointer to the last node.
    Node *getLast() const { return last; }

  private:
    Node *first = nullptr;
    Node *last = nullptr;
};

/**
 * @brief   A class that can be inherited from to allow inserting into a 
 *          DoublyLinkedList.
 * @tparam  Node
 *          The type of the nodes of the list.
 */
template <class Node>
class DoublyLinkable {
  protected:
    friend class DoublyLinkedList<Node>;
    Node *next = nullptr;
    Node *previous = nullptr;
    virtual ~DoublyLinkable() = default;

    DoublyLinkable() = default;
    DoublyLinkable(const DoublyLinkable &) {
        // Don't copy the pointers
    }
    DoublyLinkable &operator=(const DoublyLinkable &) {
        // Don't copy the pointers
        return *this;
    }
    DoublyLinkable(DoublyLinkable &&) {
        // Don't swap the pointers
    }
    DoublyLinkable &operator=(DoublyLinkable &&) {
        // Don't swap the pointers
        return *this;
    }
};

/// @}
