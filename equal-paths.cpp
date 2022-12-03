// HW4_rbabayan
#include "equal-paths.h"
using namespace std;

// You may add any prototypes of helper functions here
bool equalpathHelper(Node* root, int height, int* depth) {
    bool left, right;
    // Base case
    if (root == nullptr)
        return (true);

    if (root->left == nullptr && root->right == nullptr) {

        if (*depth != 0) {
            // compare it with the 1st leaf's height
            return (height == *depth);

        } else {  // When we found a leaf  for the first time
            // we need to set first found leaf node
            *depth = height;
            return (true);
        }
    } else {
        // when the node isn't leaf, recurse to the next
        left = equalpathHelper(root->left, height + 1, depth);
        right = equalpathHelper(root->right, height + 1, depth);
        return left && right;
    }
}

bool equalPaths(Node* root) {
    // if all the leafs have the same height
    int depth = 0;
    int height = 0;
    return equalpathHelper(root, height, &depth);
}

/*///Second way to implement the Problem
int helper(Node* root);
bool equalPaths(Node* root) {
    if (root == nullptr) {
        return true;

    }
    return (helper(root->left) == helper(root->right));
}

int helper(Node* root) {
    if (root == nullptr) {
        return 0;
    } else if (root->left == nullptr && root->right == nullptr) {
        return 0;
    } else {
        int leftsubtree = helper(root->left);
        int rightsubtree = helper(root->right);
        if (leftsubtree > rightsubtree) {
            return leftsubtree + 1;
        }
        return rightsubtree + 1;
    }
}
*/