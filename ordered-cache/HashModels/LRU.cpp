#include "LRU.hpp"

LRU::LRU(int capacity)
: CacheBase(capacity)
{
    m_block = new ListNode[m_capacity];
    m_front = nullptr;
    m_back = nullptr;
    
    for (int i = 0; i < m_capacity; i++)
    {
        m_block[i].previous = nullptr;
        m_block[i].next     = nullptr;
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
            
            m_map.erase(node->key);
            
            // Tracking
            if (m_tracking) t_deleted_ranks[100]++;
        }
        
        m_map[key] = node;
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
    
    /*
     Attach the node
     */
    
    attach(node);
    node->key = key;
}

void LRU::prefix(const std::string& prefix_key)
{
    // Tracking
    if (m_size == m_capacity) t_ranges++;
        
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
        
    // Tracking
    if (m_size == m_capacity) t_range_hits += counter;
}


void LRU::detach(ListNode *node)
{
    if (node == m_back)  m_back  = node->previous;
    if (node == m_front) m_front = node->next;
    
    if (node->previous) node->previous->next = node->next;
    if (node->next)     node->next->previous = node->previous;
}

void LRU::attach(ListNode *node)
{
    node->previous  = nullptr;
    node->next      = m_front;
    
    if (m_front) m_front->previous = node;
    
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
        node = node->next;
    }
    
    return str;
}
