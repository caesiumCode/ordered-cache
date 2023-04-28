#include "STree.hpp"

STree::STree(int capacity)
: DoubleRotationBase(capacity)
{
    
}

void STree::attach_heuristic(TreeNode* x)
{
    TreeNode* y = x->parent;
    TreeNode* z = y->parent;
    
    x->parent = nullptr;
    
    if (z == nullptr)
    {
        y->parent = x;
        
        // Zig
        if (x->key <= y->key)   x->right = y;
        // Zag
        else                    x->left = y;
        
        m_root = x;
    }
    else
    {
        TreeNode* w = z->parent;
        
        // Zig-Zig
        if (x->key <= y->key && z->left == y)
        {
            if (y->leaf) remove_leaf_queue(y);
            
            if (y->right) y->right->parent = z;
            z->left = y->right;
            z->parent = y;
            
            y->right = z;
            y->parent = x;
            
            x->right = y;
            x->parent = w;
            
            if (z->left == nullptr && z->right == nullptr) push_leaf_queue(z);
        }
        // Zag-Zag
        if (x->key > y->key && z->right == y)
        {
            if (y->leaf) remove_leaf_queue(y);
            
            if (y->left) y->left->parent = z;
            z->right = y->left;
            z->parent = y;
            
            y->left = z;
            y->parent = x;
            
            x->left = y;
            x->parent = w;
            
            if (z->left == nullptr && z->right == nullptr) push_leaf_queue(z);
        }
        // Zig-Zag
        if (x->key > y->key && z->left == y)
        {
            z->parent = x;
            y->parent = x;
            
            z->left = nullptr;
            
            x->left = y;
            x->right = z;
            x->parent = w;
            
            if (z->left == nullptr && z->right == nullptr) push_leaf_queue(z);
        }
        // Zag-Zig
        if (x->key <= y->key && z->right == y)
        {
            z->parent = x;
            y->parent = x;
            
            z->right = nullptr;
            
            x->left = z;
            x->right = y;
            x->parent = w;
            
            if (z->left == nullptr && z->right == nullptr) push_leaf_queue(z);
        }
        
        if (w == nullptr) m_root = x;
        else
        {
            if (w->left == z)   w->left = x;
            else                w->right = x;
        }
    }
}

std::string STree::get_name()
{
    return "S Tree";
}
