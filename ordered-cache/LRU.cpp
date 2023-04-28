#include "LRU.hpp"

LRU::LRU(int capacity)
: CacheBase(capacity)
{
    m_block = new ListNode[m_capacity];
    m_front = nullptr;
    m_back = nullptr;
    
    for (int i = 0; i < m_capacity; i++)
    {
        m_block[i].left     = nullptr;
        m_block[i].right    = nullptr;
    }
}

LRU::~LRU()
{    
    delete [] m_block;
}

void LRU::insert(const std::string &key)
{
    // Tracking
    t_age++;
    if (m_size == m_capacity) t_age_full++;
    
    TimerMeasure START = Timer::now();
    
    std::unordered_map<std::string, ListNode*>::iterator hint = m_map.find(key);
    bool hit = (hint != m_map.end());
    
    /*
     Select a node
     */
    
    ListNode* node;
    
    if (!hit)
    {
        if (m_size < m_capacity)
        {
            node = &m_block[m_size];
            m_size++;
        }
        else
        {
            node = m_back;
            
            // Tracking
            if (m_tracking) t_deleted_ranks[100]++;
        }        
    }
    else
    {
        node = hint->second;
        
        // Tracking
        if (m_size == m_capacity) t_hits++;
    }
    
    /*
     Remove the node
     */
    detach(node);
    
    m_map.erase(node->key);
    
    /*
     Attach the node
     */
    
    node->key = key;
    
    attach(node);
    
    m_map[node->key] = node;
    
    TimerMeasure END = Timer::now();
    
    // Tracking
    if (m_size == m_capacity)
    {
        t_chrono += END - START;
        
        if (hit) t_chrono_hit += END - START;
    }
}

void LRU::prefix(const std::string& prefix_key)
{
    // Tracking
    if (m_size == m_capacity) t_ranges++;
    
    TimerMeasure START = Timer::now();
    
    long counter = 0;
            
    for (int i = 0; i < m_size; i++)
    {
        if (m_block[i].key.compare(0, prefix_key.size(), prefix_key) == 0)
        {
            counter++;
            
            detach(m_block+i);
            attach(m_block+i);
        }
    }
    
    TimerMeasure END = Timer::now();
    
    // Tracking
    if (m_size == m_capacity)
    {
        t_range_hits += counter;
        t_chrono_range += END - START;
    }
}

void LRU::detach(ListNode *node)
{
    if (node == m_back)  m_back  = node->left;
    if (node == m_front) m_front = node->right;
    
    if (node->left)     node->left->right = node->right;
    if (node->right)    node->right->left = node->left;
}

void LRU::attach(ListNode *node)
{
    node->left  = nullptr;
    node->right = m_front;
    
    if (m_front) m_front->left = node;
    
    if (m_size == 1) m_back = node;
    
    m_front = node;
}

int LRU::get_space()
{
    return 8 * (int)m_map.bucket_count() + (8 + 8) * m_size + m_capacity * 8 * 4;
}

std::string LRU::get_name()
{
    return "LRU";
}

std::string LRU::to_string()
{
    std::string str;
    
    ListNode* node = m_front;
    
    while (node)
    {
        str += node->key + " ";
        node = node->right;
    }
    
    return str;
}
