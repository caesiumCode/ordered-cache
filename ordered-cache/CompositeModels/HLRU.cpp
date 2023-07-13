#include "HLRU.hpp"

HLRU::HLRU(uint32_t capacity)
: CAPACITY(capacity)
{
    m_queue = new ListNode[CAPACITY];
    m_front = nullptr;
    m_back = nullptr;
    m_size = 0;
    for (std::size_t i = 0; i < CAPACITY; i++)
    {
        m_queue[i].previous = nullptr;
        m_queue[i].next     = nullptr;
    }
    
    m_map.clear();
}

HLRU::~HLRU()
{
    delete [] m_queue;
}

bool HLRU::empty()
{
    return m_size == 0;
}

std::size_t HLRU::size()
{
    return m_size;
}

std::size_t HLRU::space()
{
    return 8 * m_map.bucket_count() + (8 + 8) * m_map.size() + m_size * 8 * 4;
}

void HLRU::insert_new(const std::string& key)
{
    ListNode* node = m_queue + m_size;
    node->key      = key;
    
    m_map[key] = node;
    m_size++;
    
    attach(node);
}

bool HLRU::find(const std::string& key)
{
    auto it = m_map.find(key);
    bool hit = (it != m_map.end());
    
    if (hit) last_hit = it->second;
    
    return hit;
}

void HLRU::move_last_hit_to_front()
{
    detach(last_hit);
    attach(last_hit);
}

void HLRU::move_last_hit_to_mru(HLRU& lru)
{
    lru.insert_new(last_hit->key);
    remove(last_hit);
}

void HLRU::move_lru_to_mru(HLRU& lru)
{
    lru.insert_new(m_back->key);
    remove(m_back);
}

void HLRU::delete_lru()
{
    remove(m_back);
}

void HLRU::detach(ListNode *node)
{
    if (node == m_back)  m_back  = node->previous;
    if (node == m_front) m_front = node->next;
    
    if (node->previous) node->previous->next = node->next;
    if (node->next)     node->next->previous = node->previous;
}

void HLRU::attach(ListNode *node)
{
    node->previous  = nullptr;
    node->next      = m_front;
    
    if (m_front) m_front->previous = node;
    
    if (m_size == 1) m_back = node;
    
    m_front = node;
}

void HLRU::remove(ListNode *node)
{
    detach(node);
    m_map.erase(node->key);
    
    ListNode* to_swap = m_queue + m_size - 1;
    if (node != to_swap)
    {
        node->key       = to_swap->key;
        node->previous  = to_swap->previous;
        node->next      = to_swap->next;
        
        if (m_back == to_swap)  m_back = node;
        if (m_front == to_swap) m_front = node;
        
        if (to_swap->previous)  to_swap->previous->next = node;
        if (to_swap->next)      to_swap->next->previous = node;
        
        m_map[node->key] = node;
    }
    
    m_size--;
}
