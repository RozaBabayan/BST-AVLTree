// HW4_rbabayan
#ifndef RBBST_H
#define RBBST_H

#include "bst.h"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>

struct KeyError {};

/**
 * A special kind of node for an AVL tree, which adds the balance as a data member, plus
 * other additional helper functions. You do NOT need to implement any functionality or
 * add additional data members or helper functions.
 */
template<typename Key, typename Value>
class AVLNode : public Node<Key, Value> {
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance() const;
    void setBalance(char balance);
    void updateBalance(char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    char balance_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
 * An explicit constructor to initialize the elements by calling the base class constructor and setting
 * the color to red since every new node will be red when it is first inserted.
 */
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
        : Node<Key, Value>(key, value, parent), balance_(0) {}

/**
 * A destructor which does nothing.
 */
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode() {}

/**
 * A getter for the balance of a AVLNode.
 */
template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const {
    return balance_;
}

/**
 * A setter for the balance of a AVLNode.
 */
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance) {
    balance_ = balance;
}

/**
 * Adds diff to the balance of a AVLNode.
 */
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff) {
    balance_ += diff;
}

/**
 * An overridden function for getting the parent since a static_cast is necessary to make sure
 * that our node is a AVLNode.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const {
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
 * Overridden for the same reasons as above.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const {
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
 * Overridden for the same reasons as above.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const {
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}

/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/

template<class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value> {
public:
    virtual void insert(const std::pair<const Key, Value>& new_item);  // TODO
    virtual void remove(const Key& key);                               // TODO
protected:
    virtual void nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    bool isLeftChild(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    bool isRightChild(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* node, char diff);
};
template<class Key, class Value>
bool AVLTree<Key, Value>::isLeftChild(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node) {
    if (parent->getLeft() != node) {
        return false;
    } else {
        return true;
    }
}
template<class Key, class Value>
bool AVLTree<Key, Value>::isRightChild(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node) {
    if (parent->getRight() != node) {
        return false;
    } else {
        return true;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
    // rotate the node down to the left
    if (node == nullptr || node->getRight() == nullptr)
        return;

    AVLNode<Key, Value>* rChild = node->getRight();
    AVLNode<Key, Value>* lChild = rChild->getLeft();
    AVLNode<Key, Value>* gParent = node->getParent();

    if (node->getParent() == nullptr) {
        this->root_ = node->getRight();
    } else if (gParent->getRight() == node) {
        gParent->setRight(rChild);
    } else {
        gParent->setLeft(rChild);
    }
    if (lChild != nullptr) {
        lChild->setParent(node);
    }
    // shift the pointers
    rChild->setParent(gParent);
    rChild->setLeft(node);
    node->setParent(rChild);
    node->setRight(lChild);
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {
    // rotate the node down to the right

    if (node == nullptr || node->getLeft() == nullptr)
        return;
    AVLNode<Key, Value>* lChild = node->getLeft();
    AVLNode<Key, Value>* rchild = lChild->getRight();
    AVLNode<Key, Value>* gParent = node->getParent();

    if (node->getParent() == nullptr) {
        this->root_ = lChild;
    } else if (gParent->getRight() == node) {
        gParent->setRight(lChild);
    } else {
        gParent->setLeft(lChild);
    }
    if (rchild != nullptr) {
        rchild->setParent(node);
    }
    // shift the  pointers
    lChild->setRight(node);
    lChild->setParent(gParent);
    node->setParent(lChild);
    node->setLeft(rchild);
}
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node) {

    // if p is null or parent(p) is null,return
    if (parent == nullptr || parent->getParent() == nullptr) {
        return;
    }
    AVLNode<Key, Value>* gParent = parent->getParent();
    // left child of gParent

    if (isLeftChild(gParent, parent)) {
        gParent->setBalance(gParent->getBalance() - 1);
        // Case1.b(g)==0,return
        if (gParent->getBalance() == 0) {
            return;  // already balanced
        }
        // Case2. b(g)==-1,recurse
        if (gParent->getBalance() == -1) {
            insertFix(gParent, parent);
            return;
        }

        // Case3.  b(gParent)==-2
        if (isLeftChild(parent, node)) {  // zig-zig
            rotateRight(gParent);
            parent->setBalance(0);
            gParent->setBalance(0);

        } else {  // zig-zag
            rotateLeft(parent);
            rotateRight(gParent);

            if (node->getBalance() == -1) {
                parent->setBalance(0);
                gParent->setBalance(1);
                node->setBalance(0);

            } else if (node->getBalance() == 0) {
                parent->setBalance(0);
                gParent->setBalance(0);
                node->setBalance(0);

            } else if (node->getBalance() == 1) {
                parent->setBalance(-1);
                gParent->setBalance(0);
                node->setBalance(0);
            }
        }

    }

    else if (isRightChild(gParent, parent)) {  // right child of gParent
        gParent->setBalance(gParent->getBalance() + 1);

        // Case1.b(g)==0,return
        if (gParent->getBalance() == 0) {
            return;  // already balanced
        }
        // Case2. b(g)==1,recurse
        if (gParent->getBalance() == 1) {
            insertFix(gParent, parent);
            return;
        }

        // Case3: b(gParent)==2
        if (isRightChild(parent, node)) {  // zig-zig
            rotateLeft(gParent);
            parent->setBalance(0);
            gParent->setBalance(0);

        } else {  // zig-zag
            rotateRight(parent);
            rotateLeft(gParent);

            if (node->getBalance() == 1) {
                parent->setBalance(0);
                gParent->setBalance(-1);
                node->setBalance(0);
            } else if (node->getBalance() == 0) {
                parent->setBalance(0);
                gParent->setBalance(0);
                node->setBalance(0);

            } else if (node->getBalance() == -1) {
                parent->setBalance(1);
                gParent->setBalance(0);
                node->setBalance(0);
            }
        }
    }
}
/*
 * Recall: If key is already in the tree, you should
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item) {
    // TODO
    if (this->root_ != nullptr) {
        // case2: insert newItem by walking the tree to a left/right, set all the balances
        Node<Key, Value>* existedNode = BinarySearchTree<Key, Value>::internalFind(new_item.first);
        if (existedNode != nullptr) {
            existedNode->setValue(new_item.second);
        } else {
            Node<Key, Value>* parent = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
            AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
            while (temp != nullptr) {
                if (temp->getKey() < new_item.first) {
                    if (temp->getRight() != nullptr) {
                        temp = temp->getRight();
                    } else {
                        parent->setParent(temp);
                        temp->setRight(parent);
                        break;
                    }

                } else {

                    if (temp->getLeft() != nullptr) {
                        temp = temp->getLeft();
                    } else {
                        parent->setParent(temp);
                        temp->setLeft(parent);
                        break;
                    }
                }
            }
            AVLNode<Key, Value>* updateN = static_cast<AVLNode<Key, Value>*>(parent);
            AVLNode<Key, Value>* updateParent = static_cast<AVLNode<Key, Value>*>(parent->getParent());

            if (updateParent->getBalance() == 1 || updateParent->getBalance() == -1) {
                updateParent->setBalance(0);
                return;
            } else {
                if (isRightChild(updateParent, updateN)) {
                    updateParent->setBalance(1);
                } else if (isLeftChild(updateParent, updateN)) {
                    updateParent->setBalance(-1);
                }
                insertFix(updateParent, updateN);
            }
        }
    } else {
        BinarySearchTree<Key, Value>::root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, char diff) {
    // if n is null return
    char ndiff;
    if (node == nullptr) {
        return;
    }

    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* tallerChild;  // taller child of the node
    AVLNode<Key, Value>* grandChild;   // grandchildren of the node
    if (parent != nullptr) {
        if (isLeftChild(parent, node)) {
            ndiff = 1;
        } else {
            ndiff = -1;
        }
    }
    // CaseA: assume diff== -1 is true
    if (diff == -1) {
        // Case 1: b(n) + diff == -2
        if (node->getBalance() + diff == -2) {
            tallerChild = node->getLeft();
            if (tallerChild->getBalance() == -1)  // Case 1a: zig-zig case
            {
                rotateRight(node);
                node->setBalance(0);
                tallerChild->setBalance(0);
                removeFix(parent, ndiff);
            } else if (tallerChild->getBalance() == 0)  // Case1b: zig-zig case
            {
                rotateRight(node);
                node->setBalance(-1);
                tallerChild->setBalance(1);
                return;
            } else if (tallerChild->getBalance() == 1)  // Case1c:  zig-zag case
            {
                grandChild = tallerChild->getRight();
                rotateLeft(tallerChild);
                rotateRight(node);
                if (grandChild->getBalance() == 1) {
                    node->setBalance(0);
                    tallerChild->setBalance(-1);
                    grandChild->setBalance(0);
                } else if (grandChild->getBalance() == 0) {
                    node->setBalance(0);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                } else if (grandChild->getBalance() == -1) {
                    node->setBalance(1);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                removeFix(parent, ndiff);
            }

        }
        // Case 2: b(n) + diff == -2
        else if (node->getBalance() + diff == 2) {
            tallerChild = node->getRight();
            if (tallerChild->getBalance() == 1)  // Case2a: zig-zig case
            {
                rotateLeft(node);
                node->setBalance(0);
                tallerChild->setBalance(0);
                removeFix(parent, ndiff);
            } else if (tallerChild->getBalance() == 0)  // Case2b: zig-zig case
            {
                rotateLeft(node);
                node->setBalance(1);
                tallerChild->setBalance(-1);
                return;
            } else if (tallerChild->getBalance() == 1)  // Case2c: zig-zag case
            {
                grandChild = tallerChild->getLeft();
                rotateRight(tallerChild);
                rotateLeft(node);
                if (grandChild->getBalance() == -1) {
                    node->setBalance(0);
                    tallerChild->setBalance(1);
                    grandChild->setBalance(0);
                } else if (grandChild->getBalance() == 0) {
                    node->setBalance(0);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                } else if (grandChild->getBalance() == 1) {
                    node->setBalance(-1);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                removeFix(parent, ndiff);
            }
        } else if (node->getBalance() + diff == -1) {
            node->setBalance(-1);
            return;
        } else if (node->getBalance() + diff == 0) {
            node->setBalance(0);
            removeFix(parent, ndiff);
        }
    }
    // CaseB: assume diff== 1 is true
    else if (diff == 1) {
        // Case 1: b(n) + diff == 2
        if (node->getBalance() + diff == 2) {
            tallerChild = node->getRight();
            if (tallerChild->getBalance() == 1)  // Case1a:zig-zig case
            {
                rotateLeft(node);
                node->setBalance(0);
                tallerChild->setBalance(0);
                removeFix(parent, ndiff);
            } else if (tallerChild->getBalance() == 0)  // Case1b:zig-zig case
            {
                rotateLeft(node);
                node->setBalance(1);
                tallerChild->setBalance(-1);
                return;
            } else if (tallerChild->getBalance() == -1)  // Case1c:zig-zag case
            {
                grandChild = tallerChild->getLeft();
                rotateRight(tallerChild);
                rotateLeft(node);
                if (grandChild->getBalance() == -1) {
                    node->setBalance(0);
                    tallerChild->setBalance(1);
                    grandChild->setBalance(0);
                } else if (grandChild->getBalance() == 0) {
                    node->setBalance(0);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                } else if (grandChild->getBalance() == 1) {
                    node->setBalance(-1);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                removeFix(parent, ndiff);
            }

        }
        // Case 2: b(n) + diff == -2
        else if (node->getBalance() + diff == -2) {
            tallerChild = node->getLeft();
            if (tallerChild->getBalance() == -1)  // Case2a: zig-zig case
            {
                rotateRight(node);
                node->setBalance(0);
                tallerChild->setBalance(0);
                removeFix(parent, ndiff);
            } else if (tallerChild->getBalance() == 0)  // Case2b:zig-zig case
            {
                rotateRight(node);
                node->setBalance(-1);
                tallerChild->setBalance(1);
                return;
            } else if (tallerChild->getBalance() == 1)  // Case2c: zig-zag case
            {
                grandChild = tallerChild->getRight();
                rotateLeft(tallerChild);
                rotateRight(node);
                if (grandChild->getBalance() == 1) {
                    node->setBalance(0);
                    tallerChild->setBalance(-1);
                    grandChild->setBalance(0);
                } else if (grandChild->getBalance() == 0) {
                    node->setBalance(0);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                } else if (grandChild->getBalance() == -1) {
                    node->setBalance(1);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                removeFix(parent, ndiff);
            }

        } else if (node->getBalance() + diff == 1) {
            node->setBalance(1);
            return;
        } else if (node->getBalance() + diff == 0) {
            node->setBalance(0);
            removeFix(parent, ndiff);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    // TODO
    // find the key in the BST
    AVLNode<Key, Value>* find_key = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
    AVLNode<Key, Value>* update_node = nullptr;
    char diff;

    // if the key is not in the BST
    if (find_key == nullptr) {
        return;
    }
    // zero child case
    if (find_key->getLeft() == nullptr && find_key->getRight() == nullptr) {
    }
    // two children case
    if (find_key->getLeft() != nullptr && find_key->getRight() != nullptr) {
        AVLNode<Key, Value>* update_node
                = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(find_key));
        nodeSwap(find_key, update_node);  // swap the key with its predecessor
    }

    AVLNode<Key, Value>* pNode = find_key->getParent();
    AVLNode<Key, Value>* cNode = nullptr;
    // one child case
    if (find_key->getRight() == nullptr && find_key->getLeft() != nullptr) {
        cNode = find_key->getLeft();
    } else if (find_key->getRight() != nullptr && find_key->getLeft() == nullptr) {
        cNode = find_key->getRight();
    }

    // if the child node exists set the parent node
    if (cNode != nullptr) {
        cNode->setParent(pNode);
    }
    if (pNode != nullptr) {
        if (isRightChild(pNode, find_key)) {
            pNode->setRight(cNode);
            if (cNode == nullptr) {
                pNode->setRight(nullptr);
            }
            diff = -1;

        } else if (isLeftChild(pNode, find_key)) {
            pNode->setLeft(cNode);
            if (cNode == nullptr) {
                pNode->setLeft(nullptr);
            }
            diff = 1;
        }

    } else if (pNode == nullptr) {
        this->root_ = cNode;
    }
    // when root is changed update it
    if (this->root_ != nullptr) {
        if (this->root_->getKey() == key) {
            this->root_ = update_node;
        }
    }

    delete find_key;
    find_key = nullptr;
    removeFix(pNode, diff);  // Patch tree
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2) {
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

#endif
