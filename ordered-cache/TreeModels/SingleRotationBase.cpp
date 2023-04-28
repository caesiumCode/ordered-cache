#include "SingleRotationBase.hpp"

SingleRotationBase::SingleRotationBase(int capacity)
: LeafQueueBase(capacity)
{
    
}

void SingleRotationBase::attach_heuristic(TreeNode *node)
{
    TreeNode* parent    = node->parent;
    TreeNode* gparent   = node->parent->parent;
    
    parent->parent = node;
    node->parent = gparent;
    
    if (parent->key <= node->key)   node->left = parent;
    else                            node->right = parent;
    
    if (gparent)
    {
        if (gparent->left == parent)    gparent->left = node;
        else                            gparent->right = node;
    }
    
    if (gparent == nullptr) m_root = node;
}

void SingleRotationBase::rotate_up(TreeNode *x)
{
    TreeNode* y         = x->parent;
    TreeNode* ancestor  = y->parent;
    
    /*
     Zig
     
           y                   x
          / \                 / \
         /   \               /   \
        x     R     ===>    L     y
       / \                       / \
      /   \                     /   \
     L     M                   M     R
     
     */
    if (y->left == x)
    {
        TreeNode* M = x->right;
        
        // Rotate
        x->right = y;
        x->parent = y->parent;
        y->left = M;
        y->parent = x;
        if (M) M->parent = y;
        
        // If y becomes a leaf
        if (!y->left and !y->right) push_leaf_queue(y);
    }
    /*
     Zag
     
           y                    x
          / \                  / \
         /   \                /   \
        L     x     ===>     y     R
             / \            / \
            /   \          /   \
           M     R        L     M
     
     */
    else
    {
        TreeNode* M = x->left;
        
        
        // Rotate
        x->left = y;
        x->parent = y->parent;
        y->right = M;
        y->parent = x;
        if (M) M->parent = y;
        
        // If y becomes a leaf
        if (!y->left and !y->right) push_leaf_queue(y);
    }
    
    if (ancestor == nullptr) m_root = x;
    else
    {
        if (ancestor->left == y)    ancestor->left = x;
        else                        ancestor->right = x;
    }
}
