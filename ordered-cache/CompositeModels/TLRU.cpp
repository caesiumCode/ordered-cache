#include "TLRU.hpp"

TLRU::TLRU(uint32_t capacity)
: CAPACITY(capacity)
{
    m_block = new TreeNode[CAPACITY];
    m_root = nullptr;
    m_leaf_queue_front = nullptr;
    m_leaf_queue_back = nullptr;
    
    for (int i = 0; i < CAPACITY; i++)
    {
        m_block[i].parent   = nullptr;
        m_block[i].left     = nullptr;
        m_block[i].right    = nullptr;
    }
}

TLRU::~TLRU()
{
    delete [] m_block;
}







bool TLRU::empty()
{
    return m_size == 0;
}

std::size_t TLRU::size()
{
    return m_size;
}

std::size_t TLRU::space()
{
    return m_size * (8 * 5 + 1);
}

void TLRU::insert_new(const std::string& key)
{
    TreeNode* loc = find_loc(key);
    
    TreeNode* node = m_block + m_size;
    node->key    = key;
    node->leaf   = false;
    node->left   = nullptr;
    node->right  = nullptr;
    node->parent = loc;
    
    if (m_root == nullptr)
    {
        m_root = node;
        push_leaf_queue(m_root);
    }
    else
    {
        attach_heuristic(node);
        move_heuristic(node);
    }
    
    m_size++;
}

bool TLRU::find(const std::string& key)
{
    TreeNode* node = m_root;
    int cmp = 0;
    if (node) cmp = 1;
    
    while (node && !node->leaf && cmp != 0)
    {
        cmp = node->key.compare(key);
        
        if      (cmp > 0) node = node->left;
        else if (cmp < 0) node = node->right;
    }
    
    bool hit = (node != nullptr && node->key == key);
    if (hit) last_hit = node;
    
    return hit;
}

TreeNode* TLRU::find_loc(const std::string &key)
{
    TreeNode* node = m_root;
    int cmp = 0;
    if (node) cmp = node->key.compare(key);
    
    while (node && !node->leaf && cmp != 0)
    {
        TreeNode* next = nullptr;
        if (cmp > 0) next = node->left;
        else         next = node->right;
        
        if (next == nullptr) return node;
        
        node = next;
        
        cmp = node->key.compare(key);
    }
    
    return node;
}

void TLRU::move_last_hit_to_front()
{
    move_heuristic(last_hit);
}

void TLRU::move_last_hit_to_mru(TLRU& lru)
{
    TreeNode* to_remove = last_hit;
    
    lru.insert_new(to_remove->key);
    
    if (to_remove->leaf) remove_leaf_queue(to_remove);
    else                 remove_internal_node(to_remove);

    remove(to_remove);
}

void TLRU::move_lru_to_mru(TLRU& lru)
{
    lru.insert_new(m_leaf_queue_front->key);
    delete_lru();
}

void TLRU::delete_lru()
{
    TreeNode* to_remove = m_leaf_queue_front;
    
    pop_leaf_queue();
    
    remove(to_remove);
}

void TLRU::remove_internal_node(TreeNode *node)
{
    TreeNode* L = find_largest_node(node->left);
    
    /*
          /
         N            /
        / \          L
       L   B   =>   / \
      /            A   B
     A
     
     */
    if (L)
    {
        // Splay the largest node on left branch
        if (L != node->left)
        {
            if (L->leaf)
            {
                remove_leaf_queue(L);
                attach_heuristic(L);
            }
            
            while (L != node->left && L->parent != node->left) rotate_up(L);
            
            if (L->parent == node->left) rotate_up_single(L);
        }
        
        
        /*
              /
             N            /
            / \          L
           L   B   =>     \
                           B
         
         */
        if (L->leaf && node->right)
        {
            remove_leaf_queue(L);
            
            L->right = node->right;
            node->right->parent = L;
        }
        /*
              /
             N            /
            / \          L
           L   B   =>   / \
          /            A   B
         A
         
         */
        else if (!L->leaf)
        {
            L->right = node->right;
            if (node->right) node->right->parent = L;
        }
        
        if (node->parent)
        {
            if (node->parent->left  == node) node->parent->left  = L;
            if (node->parent->right == node) node->parent->right = L;
        }
        else m_root = L;
        
        L->parent = node->parent;
        
    }
    /*
          /
         N
          \           /
           B   =>    B
                   
     
     
     */
    else
    {
        if (node->parent)
        {
            if (node->parent->left  == node) node->parent->left  = node->right;
            if (node->parent->right == node) node->parent->right = node->right;
        }
        else m_root = node->right;
        
        if (node->right) node->right->parent = node->parent;
    }
}

TreeNode* TLRU::find_largest_node(TreeNode *node)
{
    if (node) while (node->right && !node->leaf) node = node->right;
    
    return node;
}


void TLRU::remove(TreeNode *node)
{
    TreeNode* to_swap = m_block + m_size - 1;
    if (node != to_swap)
    {
        node->key       = to_swap->key;
        node->parent    = to_swap->parent;
        node->left      = to_swap->left;
        node->right     = to_swap->right;
        node->leaf      = to_swap->leaf;
        
        if (m_root == to_swap) m_root = node;
        if (m_leaf_queue_front == to_swap) m_leaf_queue_front = node;
        if (m_leaf_queue_back  == to_swap) m_leaf_queue_back  = node;
        
        if (to_swap->parent)
        {
            if (to_swap->parent->left  == to_swap) to_swap->parent->left  = node;
            if (to_swap->parent->right == to_swap) to_swap->parent->right = node;
        }
        if (to_swap->leaf)
        {
            if (to_swap->left)  to_swap->left->right = node;
            if (to_swap->right) to_swap->right->left = node;
        }
        else
        {
            if (to_swap->left)  to_swap->left->parent  = node;
            if (to_swap->right) to_swap->right->parent = node;
        }
    }
    else if (m_size == 1)
    {
        m_root              = nullptr;
        m_leaf_queue_front  = nullptr;
        m_leaf_queue_back   = nullptr;
    }
    
    m_size--;
}





void TLRU::attach_heuristic(TreeNode* node)
{
    TreeNode* parent    = node->parent;
    TreeNode* gparent   = node->parent->parent;
    
    parent->parent = node;
    node->parent   = gparent;
    
    if (parent->key <= node->key) node->left  = parent;
    else                          node->right = parent;
    
    if (gparent)
    {
        if (gparent->left == parent) gparent->left  = node;
        else                         gparent->right = node;
    }
    else m_root = node;
}

void TLRU::move_heuristic(TreeNode* node)
{
    if (node->leaf && node != m_root)
    {
        remove_leaf_queue(node);
        attach_heuristic(node);
    }
    
    while (node != m_root) rotate_up(node);
}

void TLRU::rotate_up_single(TreeNode *x)
{
    TreeNode* y = x->parent;
    
    if (y->left == x)
    {
        TreeNode* M = x->right;
        
        // Rotate
        x->right  = y;
        x->parent = y->parent;
        y->left   = M;
        y->parent = x;
        if (M) M->parent = y;
        
        // If y becomes a leaf
        if (!y->left and !y->right) push_leaf_queue(y);
    }
    else
    {
        TreeNode* M = x->left;
        
        // Rotate
        x->left   = y;
        x->parent = y->parent;
        y->right  = M;
        y->parent = x;
        if (M) M->parent = y;
        
        // If y becomes a leaf
        if (!y->left and !y->right) push_leaf_queue(y);
    }
    
    
}

void TLRU::rotate_up(TreeNode *x)
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


void TLRU::push_leaf_queue(TreeNode* node)
{
    node->leaf  = true;
    node->left  = nullptr;
    node->right = nullptr;
    
    if (m_leaf_queue_front == nullptr)
    {
        m_leaf_queue_front = node;
        m_leaf_queue_back  = node;
    }
    else
    {
        node->left  = m_leaf_queue_back;
        node->right = nullptr;
        
        m_leaf_queue_back->right = node;
        
        m_leaf_queue_back = node;
    }
}

TreeNode* TLRU::pop_leaf_queue()
{
    TreeNode* node = m_leaf_queue_front;
    
    if (node) remove_leaf_queue(m_leaf_queue_front);
    
    return node;
}

void TLRU::remove_leaf_queue(TreeNode* node)
{
    if (node->left)  node->left->right = node->right;
    if (node->right) node->right->left = node->left;
    if (node == m_leaf_queue_front)
    {
        m_leaf_queue_front = m_leaf_queue_front->right;
        if (m_leaf_queue_front) m_leaf_queue_front->left = nullptr;
    }
    if (node == m_leaf_queue_back)
    {
        m_leaf_queue_back = m_leaf_queue_back->left;
        if (m_leaf_queue_back) m_leaf_queue_back->right = nullptr;
    }
    
    if (node->parent)
    {
        if (node->parent->left  == node) node->parent->left  = nullptr;
        if (node->parent->right == node) node->parent->right = nullptr;
        
        if (!node->parent->left && !node->parent->right) push_leaf_queue(node->parent);
    }
    
    node->leaf  = false;
    node->left  = nullptr;
    node->right = nullptr;
}
