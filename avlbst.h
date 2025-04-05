#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
private:
    //rotate helpers
    void rotateLeft(AVLNode<Key, Value> *node);
    void rotateRight(AVLNode<Key, Value> *node);

    //balance helpers
    void balanceInsert(AVLNode<Key, Value> *node);
    void balanceRemove(AVLNode<Key, Value> *node);
    int8_t getBalanceDynamic(AVLNode<Key, Value> *node) const;
    int8_t getHeight(AVLNode<Key, Value> *node) const;

    AVLNode<Key, Value>* findNode(const Key& key) const;
    AVLNode<Key, Value>* getInOrderPredecessor(AVLNode<Key, Value> *node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (!this->root_){
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }

    AVLNode<Key, Value> * current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value> *parent = nullptr;

    while (current){
        parent = current;
        if (new_item.first < current->getKey()){
            if (current->getLeft()){
                current = current->getLeft();
            }
            else
                break;
        }
        else if (new_item.first > current->getKey()){
            if (current->getRight()){
                current = current->getRight();
            }
            else
                break;
        }
        else {
            current->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key, Value> *newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if (new_item.first < parent->getKey()){
        parent->setLeft(newNode);
    }
    else {
        parent->setRight(newNode);
    }

    balanceInsert(newNode);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if (!this->root_)
        return;

    AVLNode<Key, Value> *remove = findNode(key);
    if (!remove)
        return;
    
    AVLNode<Key, Value> *parent = remove->getParent();
    
    if (remove->getLeft() && remove->getRight()){
        AVLNode<Key, Value> *pred = remove->getLeft();
        while (pred->getRight()){
            pred = pred->getRight();
        }
        //nodeSwap(remove, pred);
        remove->setKey(pred->getKey());
        remove->setValue(pred->getValue());

        parent = remove->getParent();
    }

    AVLNode<Key, Value> *child = remove->getLeft() ? remove->getLeft() : remove->getRight();

    if (parent){
        if (parent->getLeft() == remove)
            parent->left_ = child;
        else
            parent->right_ = child;
    }
    else {
        this->root_ = child;
    }

    if (child){
        child->parent_ = parent;
    }

    delete remove;

    if (parent){
        balanceRemove(parent);
    }
}
*/

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::findNode(const Key& key) const{
    AVLNode<Key, Value> *current = static_cast<AVLNode<Key, Value>*>(this->root_);
    while (current){
        if (key < current->getKey()){
            current = current->getLeft();
        }
        else if (key > current->getKey()){
            current = current->getRight();
        }
        else {
            // found value
            return current;
        }
    }
    return nullptr;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value> *x){
    AVLNode<Key, Value> *y = x->getRight();
    AVLNode<Key, Value> *parent = x->getParent();

    x->setRight(y->getLeft());
    if (y->getLeft()){
        y->getLeft()->setParent(x);
    }

    y->setLeft(x);
    x->setParent(y);
    y->setParent(parent);

    if(!parent){
        this->root_ = y;
    }
    else if(parent->getLeft() == x){
        parent->setLeft(y);
    }
    else{
        parent->setRight(y);
    }

    //int8_t xBalance = x->getBalance() - 1 - std::max<int8_t>(y->getBalance(), 0);
    //int8_t yBalance = y->getBalance() - 1 + std::min<int8_t>(xBalance + 1, 0);

    x->setBalance(x->getBalance() - 1 - std::max<int8_t>(y->getBalance(), 0));
    y->setBalance(y->getBalance() - 1 + std::min<int8_t>(x->getBalance() + 1, 0));
    //x->setBalance(xBalance);
    //y->setBalance(yBalance);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value> *y){
    AVLNode<Key, Value> *x = y->getLeft();
    AVLNode<Key, Value> *parent = y->getParent();
    
    y->setLeft(x->getRight());
    if (x->getRight()){
        x->getRight()->setParent(y);
    }
    
    x->setRight(y);
    y->setParent(x);
    x->setParent(parent);
    
    if(!parent){
        this->root_ = x;
    }
    else if (parent->getLeft() == y){
        parent->setLeft(x);
    }
    else {
        parent->setRight(x);
    }

    //int8_t yBalance = y->getBalance() + 1 - std::min<int8_t>(x->getBalance(), 0);
    //int8_t xBalance = x->getBalance() + 1 + std::max<int8_t>(yBalance - 1, 0);

    y->setBalance(y->getBalance() + 1 - std::min<int8_t>(x->getBalance(), 0));
    x->setBalance(x->getBalance() + 1 + std::max<int8_t>(y->getBalance() - 1, 0));

    //x->setBalance(xBalance);
    //y->setBalance(yBalance);
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key){
    if (!this->root_)
        return;

    AVLNode<Key, Value> *node = findNode(key);
    if (!node)
        return;

    AVLNode<Key, Value> *parent = node->getParent();
    //two children
    if (node->getLeft() && node->getRight()){
        AVLNode<Key, Value> *pred = node->getLeft();
        while (pred->getRight()){
          pred = pred->getRight();
        }

        const_cast<Key&>(node->item_.first) = pred->getKey();
        node->item_.second = pred->getValue();
        node = pred;
        parent = node->getParent();
    }
    // one child
        AVLNode<Key, Value> *child = node->getLeft() ? node->getLeft() : node->getRight();
        if (!parent) {
          this->root_ = child;
          if (child) {
              child->setParent(nullptr);
          }
        } 
        else {
          if (parent->getLeft() == node) {
              parent->setLeft(child);
          } 
          else {
              parent->setRight(child);
          }
          if (child) {
              child->setParent(parent);
          }
        }

        AVLNode<Key, Value> *start = parent ? parent : static_cast<AVLNode<Key, Value>*>(this->root_);
        delete node;
        //if (start){
          while (start){
              start->setBalance(static_cast<int8_t>(getHeight(start->getRight()) - getHeight(start->getLeft())));

              if (start->getBalance() == -2){
                  AVLNode<Key, Value> *left = start->getLeft();
                  int8_t childBalance = getHeight(left->getRight()) - getHeight(left->getLeft());
                  
                  if (childBalance <= 0){
                      rotateRight(start);
                  }
                  else {
                      rotateLeft(start->getLeft());
                      rotateRight(start);
                  }
              }
              else if (start->getBalance() == 2){
                AVLNode<Key, Value> *right = start->getRight();
                int8_t childBalance = getHeight(right->getRight()) - getHeight(right->getLeft());

                  if (childBalance >= 0){
                      rotateLeft(start);
                  }
                  else {
                      rotateRight(right);
                      rotateLeft(start);
                  }
              }

              start->setBalance(getHeight(start->getRight()) - getHeight(start->getLeft()));

              if (abs(start->getBalance()) == 1)
                  break;

              start = start->getParent();
          }
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getInOrderPredecessor(AVLNode<Key, Value> *node){
    if (node->getLeft()){
        AVLNode<Key, Value>* current = node->getLeft();
        while (current->getRight()){
            current = current->getRight();
        }
        return current;
    }
    return nullptr;
}

template<class Key, class Value>
void AVLTree<Key, Value>::balanceInsert(AVLNode<Key, Value> *node){
    AVLNode<Key, Value> *parent = node->getParent();
    while(parent){
      int8_t balance = getHeight(parent->getRight()) - getHeight(parent->getLeft());
      parent->setBalance(balance);

      if (balance == -2){
        AVLNode<Key, Value> *left = parent->getLeft();
        if (getHeight(left->getLeft()) >= getHeight(left->getRight())) {
          rotateRight(parent);
        }
        else {
          rotateLeft(left);
          rotateRight(parent);
        }
      }
      else if (balance == 2){
        AVLNode<Key, Value> *right = parent->getRight();
        if (getHeight(right->getRight()) >= getHeight(right->getLeft())){
          rotateLeft(parent);
        }
        else{
          rotateRight(right);
          rotateLeft(parent);
        }
        break;
      }
      parent = parent->getParent();
    }
    
    /*
    while (node){
        node->setBalance(static_cast<int8_t>(getHeight(node->getRight()) - getHeight(node->getLeft())));

        if (node->getBalance() == -2){
            if (node->getRight()->getBalance() <= 0){
                rotateRight(node);
            }
            else {
                rotateLeft(node->getLeft());
                rotateRight(node);
            }
            break;
        }
        else if (node->getBalance() == 2){
            if (node->getRight()->getBalance() >= 0){
                rotateLeft(node);
            }
            else {
                rotateRight(node->getRight());
                rotateLeft(node);
            }
            break;
        }

        if (abs(node->getBalance()) == 0)
            break;
        node = node->getParent();
    }
    */
}

template<class Key, class Value>
void AVLTree<Key, Value>::balanceRemove(AVLNode<Key, Value> *node){
    while (node){
        node->setBalance(static_cast<int8_t>(getHeight(node->getRight()) - getHeight(node->getLeft())));

        if (node->getBalance() == -2){
            if (node->getLeft()->getBalance() <= 0){
                rotateRight(node);
            }
            else {
                rotateLeft(node->getLeft());
                rotateRight(node);
            }
        }
        else if (node->getBalance() == 2){
            if (node->getRight()->getBalance() >= 0){
                rotateLeft(node);
            }
            else {
                rotateRight(node->getRight());
                rotateLeft(node);
            }
        }

        if (abs(node->getBalance()) == 1)
            break;

        node = node->getParent();
    }
}

template<class Key, class Value>
int8_t AVLTree<Key, Value>::getHeight(AVLNode<Key, Value> *node) const{
    if (!node)
        return -1;
    return static_cast<int8_t>(1) + std::max(
        (getHeight(node->getLeft())), 
        (getHeight(node->getRight()))
        );
}

template<class Key, class Value>
int8_t AVLTree<Key, Value>::getBalanceDynamic(AVLNode<Key, Value> *node) const{
    if (!node)
        return 0;
    return node->getBalance();
}
#endif
