#include "LSTree.hpp"

LSTree::LSTree(int capacity)
: CacheBase(capacity)
{
    m_block = new LightTreeNode[m_capacity];
    m_root = nullptr;
    
    for (int i = 0; i < m_capacity; i++)
    {
        m_block[i].left     = nullptr;
        m_block[i].right    = nullptr;
    }
    
    // Tracking
    auto tree_node_comp = [](LightTreeNode* a, LightTreeNode* b) { return a->timestamp < b->timestamp; };
    t_timestamp_heap = std::set<LightTreeNode*, std::function<bool(LightTreeNode*, LightTreeNode*)>>(tree_node_comp);
}

LSTree::~LSTree()
{
    delete [] m_block;
}

LightTreeNode* LSTree::find(const std::string &key)
{
    LightTreeNode* node = m_root;
    path.clear();
    
    while (node && node->key != key)
    {
        path.push_back(node);
        
        LightTreeNode* next = nullptr;
        if (key <= node->key)  next = node->left;
        else                    next = node->right;
        
        if (next == nullptr) return node;
        
        node = next;
    }
    
    return node;
}


void LSTree::insert(const std::string &key)
{
    // Tracking
    t_age++;
    if (m_size == m_capacity) t_age_full++;
        
    LightTreeNode* node = find(key);
    bool hit = (node != nullptr && node->key == key);
    
    // Cache hit
    if (hit)    insert_hit(key, node);
    
    // Cache miss
    else        insert_miss(key, node);
}

void LSTree::prefix(const std::string& prefix_key)
{
    // Tracking
    if (m_size == m_capacity) t_ranges++;
        
    long counter = 0;
        
    //std::vector<LightTreeNode*> range;
    std::vector<LightTreeNode*> stack = {m_root};
    if (m_root == nullptr) stack.clear();
    
    while (!stack.empty())
    {
        LightTreeNode* node = stack.back();
        stack.pop_back();
        
        int comp = node->key.compare(0, prefix_key.size(), prefix_key);
        if (comp == 0)
        {
            counter++;
            //range.push_back(node);
            if (node->right) stack.push_back(node->right);
            if (node->left) stack.push_back(node->left);
        }
        else if (comp < 0 && node->right) stack.push_back(node->right);
        else if (comp > 0 && node->left) stack.push_back(node->left);
    }
    
    //for (LightTreeNode* node : range) if (node != m_root) splay(node);
        
    // Tracking
    if (m_size == m_capacity) t_range_hits += counter;
}

void LSTree::insert_miss(const std::string &key, LightTreeNode *node)
{
    // If the tree is full
    if (m_size == m_capacity)
    {
        // Increase rank of all the nodes that arrived after removed node
        if (m_tracking)
        {
            int deleted_rank = std::round(float(node->rank) / float(m_capacity) * 100.f);
            t_deleted_ranks[deleted_rank]++;
                        
            std::set<LightTreeNode*>::iterator it_found = t_timestamp_heap.find(node);
            for (auto it = it_found; it != t_timestamp_heap.end(); it++) (*it)->rank++;
            t_timestamp_heap.erase(it_found);
        }
        
        // Use current node as the new node
        node->key = key;
        node->timestamp = t_age;
        node->rank = 1;
        
        path.pop_back();
    }
    else
    {
        LightTreeNode* new_node = &m_block[m_size];
        m_size++;
        
        new_node->key = key;
        new_node->left = nullptr;
        new_node->right = nullptr;
        new_node->timestamp = t_age;
        new_node->rank = 1;
                
        if (m_tracking) for (auto it = t_timestamp_heap.begin(); it != t_timestamp_heap.end(); it++) (*it)->rank++;
        
        // Attach new node to the tree
        if (node)
        {
            if (key <= node->key)   node->left = new_node;
            else                    node->right = new_node;
        }
        
        node = new_node;
    }
    
    if (m_tracking) t_timestamp_heap.insert(node);
    
    if (m_root == nullptr) m_root = node;
    else splay(node);
}

void LSTree::insert_hit(const std::string &key, LightTreeNode *node)
{
    // Tracking
    if (m_size == m_capacity) t_hits++;
    
    if (m_tracking)
    {
        // Increase the rank of all the nodes that arrived after the inserted node
        std::set<LightTreeNode*>::iterator it_found = t_timestamp_heap.find(node);
        for (auto it = it_found; it != t_timestamp_heap.end(); it++) (*it)->rank++;
        
        // Update the rank of the inserted node to 1
        t_timestamp_heap.erase(node);
        
        node->rank = 1;
        node->timestamp = t_age;
        
        t_timestamp_heap.insert(node);
    }
        
    splay(node);
}

void LSTree::splay(LightTreeNode* x)
{
    LightTreeNode* y = nullptr;
    LightTreeNode* z = nullptr;
    
    if (path.size() == 1)
    {
        y = path.back();
        path.pop_back();
        
        // Zig
        if (y->left == x)
        {
            y->left = x->right;
            x->right = y;
        }
        // Zag
        else
        {
            y->right = x->left;
            x->left = y;
        }
    }
    else if (path.size() >= 2)
    {
        y = path.back();
        path.pop_back();
        z = path.back();
        path.pop_back();
        
        // Zig-Zig
        if (z->left == y and y->left == x)
        {
            z->left = y->right;
            y->left = x->right;
            y->right = z;
            x->right = y;
        }
        // Zag-Zag
        else if (z->right == y and y->right == x)
        {
            z->right = y->left;
            y->right = x->left;
            y->left = z;
            x->left = y;
        }
        // Zig-Zag
        else if (z->left == y and y->right == x)
        {
            y->right = x->left;
            z->left = x->right;
            x->left = y;
            x->right = z;
        }
        // Zag-Zig
        else
        {
            y->left = x->right;
            z->right = x->left;
            x->right = y;
            x->left = z;
        }
    }
    
    if (path.empty()) m_root = x;
    else
    {
        LightTreeNode* parent = path.back();
        
        if      (parent->left == z)     parent->left = x;
        else if (parent->right == z)    parent->right = x;
        
        splay(x);
    }
}

std::string LSTree::get_name()
{
    return "LS Tree";
}


int LSTree::get_space()
{
    return m_capacity * 8 * 4;
}

std::string LSTree::to_string()
{
    return m_root->to_string("", true);
}
