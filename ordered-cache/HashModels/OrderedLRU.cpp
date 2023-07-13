#include "OrderedLRU.hpp"

OrderedLRU::OrderedLRU(int capacity)
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

OrderedLRU::~OrderedLRU()
{
    delete [] m_block;
}


void OrderedLRU::insert(const std::string &key)
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
            
            m_map.erase(node->key);
            m_tree.erase(node->key);
            
            // Tracking
            if (m_tracking) t_deleted_ranks[100]++;
        }
        
        m_map[key] = node;
        m_tree[key] = node;
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
        
    TimerMeasure END = Timer::now();
    
    // Tracking
    if (m_size == m_capacity)
    {
        t_chrono += END - START;
        
        if (hit) t_chrono_hit += END - START;
    }
}

struct PrefixProbe { std::string_view prefix; };
bool operator<(PrefixProbe a, std::string_view b) { return a.prefix < b.substr(0, a.prefix.size()); }
bool operator<(std::string_view a, PrefixProbe b) { return a.substr(0, b.prefix.size()) < b.prefix; }

void OrderedLRU::prefix(const std::string& prefix_key)
{
    // Tracking
    if (m_size == m_capacity) t_ranges++;
    
    TimerMeasure START = Timer::now();
    
    long counter = 0;
    
    auto [ prefixBegin, prefixEnd ] = m_tree.equal_range(PrefixProbe{ prefix_key });
     
    for (auto it = prefixBegin; it != prefixEnd; ++it)
    {
        detach(it->second);
        attach(it->second);

        counter++;
    }
    
    TimerMeasure END = Timer::now();
    
    // Tracking
    if (m_size == m_capacity)
    {
        t_range_hits += counter;
        t_chrono_range += END - START;
    }
}

void OrderedLRU::detach(ListNode *node)
{
    if (node == m_back)  m_back  = node->previous;
    if (node == m_front) m_front = node->next;
    
    if (node->previous) node->previous->next = node->next;
    if (node->next)     node->next->previous = node->previous;
}

void OrderedLRU::attach(ListNode *node)
{
    node->previous  = nullptr;
    node->next      = m_front;
    
    if (m_front) m_front->previous = node;
    
    if (m_size == 1) m_back = node;
    
    m_front = node;
}


int OrderedLRU::get_space()
{
    return 8 * int(m_map.bucket_count()) + (8 + 8) * int(m_size) + m_capacity * 8 * 4 + int(m_tree.size()) * 8 * 4;
}


std::string OrderedLRU::get_name()
{
    return "OLRU";
}


std::string OrderedLRU::to_string()
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
