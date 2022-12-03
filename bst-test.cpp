#include "avlbst.h"
#include "bst.h"
#include <iostream>
#include <map>

using namespace std;

int main(int argc, char* argv[]) {
    // Binary Search Tree tests
    BinarySearchTree<char, int> bt;
    // BinarySearchTree<char,int> b;
    bt.insert(std::make_pair('a', 1));
    bt.insert(std::make_pair('b', 2));

    cout << "Binary Search Tree contents:" << endl;
    for (BinarySearchTree<char, int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if (bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    } else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    bt.remove('b');
    bt.print();

    // AVL Tree Tests
    AVLTree<char, int> at;
    at.insert(std::make_pair('a', 1));
    at.insert(std::make_pair('b', 2));

    cout << "\nAVLTree contents:" << endl;
    for (AVLTree<char, int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if (at.find('b') != at.end()) {
        cout << "Found b" << endl;
    } else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    at.remove('b');
    at.print();

    return 0;
}
