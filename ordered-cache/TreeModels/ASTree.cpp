#include "ASTree.hpp"

ASTree::ASTree(int capacity)
: DoubleRotationBase(capacity)
{
    
}

void ASTree::attach_heuristic(TreeNode* node)
{
    TreeNode* parent    = node->parent;
    TreeNode* gparent   = node->parent->parent;
    
    parent->parent = node;
    node->parent = gparent;
    
    if (parent->key <= node->key) node->left = parent;
    else                          node->right = parent;
    
    if (gparent)
    {
        if (gparent->left == parent)    gparent->left = node;
        else                            gparent->right = node;
    }
    
    if (gparent == nullptr) m_root = node;
}

std::string ASTree::get_name()
{
    return "AS Tree";
}
