#include "DoubleRotationBase.hpp"

DoubleRotationBase::DoubleRotationBase(int capacity)
: LeafQueueBase(capacity)
{
    
}

void DoubleRotationBase::move_heuristic(TreeNode* node)
{
    if (node->leaf) remove_leaf_queue(node);
    
    while (node != m_root) rotate_up(node);
}

void DoubleRotationBase::rotate_up(TreeNode *x)
{
    TreeNode* y = x->parent;
    TreeNode* z = y->parent;
    
    if (z == nullptr)
    {
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
        
        x->parent = nullptr;
        m_root = x;
    }
    else
    {
        TreeNode* ancestor = z->parent;
        x->parent = ancestor;
        
        /*
         Zig-Zig
                       z            x
                      / \          / \
                     y   D        A   y
                    / \              / \
                   x   C    ===>    B   z
                  / \                  / \
                 A   B                C   D
         */
        if (z->left == y and y->left == x)
        {
            TreeNode* B = x->right;
            TreeNode* C = y->right;
            
            if (B) B->parent = y;
            if (C) C->parent = z;
            
            z->left = C;
            z->parent = y;
            
            y->left = B;
            y->right = z;
            y->parent = x;
            
            x->right = y;
            
            // If z becomes a leaf
            if (!z->left and !z->right) push_leaf_queue(z);
        }
        /*
         Zag-Zag
                       z                    x
                      / \                  / \
                     A   y                y   D
                        / \              / \
                       B   x    ===>    z   C
                          / \          / \
                         C   D        A   B
         */
        else if (z->right == y and y->right == x)
        {
            TreeNode* B = y->left;
            TreeNode* C = x->left;
            
            if (B) B->parent = z;
            if (C) C->parent = y;
            
            z->right = B;
            z->parent = y;
            
            y->right = C;
            y->left = z;
            y->parent = x;
            
            x->left = y;
            
            // If z becomes a leaf
            if (!z->left and !z->right) push_leaf_queue(z);
        }
        /*
         Zig-Zag
                       z
                      / \               x
                     y   D             / \
                    / \      ===>     /   \
                   A   x             y     z
                      / \           / \   / \
                     B   C         A   B C   D
         */
        else if (z->left == y and y->right == x)
        {
            TreeNode* B = x->left;
            TreeNode* C = x->right;
            
            if (B) B->parent = y;
            if (C) C->parent = z;
            
            z->left = C;
            z->parent = x;
            
            y->right = B;
            y->parent = x;
            
            x->left = y;
            x->right = z;
            
            // If y becomes a leaf
            if (!y->left and !y->right) push_leaf_queue(y);
            // If z becomes a leaf
            if (!z->left and !z->right) push_leaf_queue(z);
        }
        /*
         Zag-Zig
                       z
                      / \                x
                     A   y              / \
                        / \    ===>    /   \
                       x   D          z     y
                      / \            / \   / \
                     B   C          A   B C   D
         */
        else
        {
            TreeNode* B = x->left;
            TreeNode* C = x->right;
            
            if (B) B->parent = z;
            if (C) C->parent = y;
            
            z->right = B;
            z->parent = x;
            
            y->left = C;
            y->parent = x;
            
            x->left = z;
            x->right = y;
            
            // If y becomes a leaf
            if (!y->left and !y->right) push_leaf_queue(y);
            // If z becomes a leaf
            if (!z->left and !z->right) push_leaf_queue(z);
        }
        
        if (ancestor == nullptr) m_root = x;
        else
        {
            if (ancestor->left == z)    ancestor->left = x;
            else                        ancestor->right = x;
        }
    }
}
