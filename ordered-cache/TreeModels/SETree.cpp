#include "SETree.hpp"

SETree::SETree(int capacity)
: SingleRotationBase(capacity)
{
    
}

void SETree::move_heuristic(TreeNode* node)
{
    if (node->leaf) remove_leaf_queue(node);
    
    if (node != m_root) rotate_up(node);
}

std::string SETree::get_name()
{
    return "SE Tree";
}
