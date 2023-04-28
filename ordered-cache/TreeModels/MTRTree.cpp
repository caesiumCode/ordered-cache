#include "MTRTree.hpp"

MTRTree::MTRTree(int capacity)
: SingleRotationBase(capacity)
{
    
}

void MTRTree::move_heuristic(TreeNode* node)
{
    if (node->leaf) remove_leaf_queue(node);
    
    while (node != m_root) rotate_up(node);
}

std::string MTRTree::get_name()
{
    return "MTR Tree";
}
