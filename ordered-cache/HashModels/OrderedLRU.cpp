#include "OrderedLRU.hpp"

OrderedLRU::OrderedLRU(int capacity)
: CacheBase(capacity)
{
    m_queue.clear();
    m_map.clear();
    m_tree.clear();
}


void OrderedLRU::insert(const std::string &key)
{
    // Tracking
    t_age++;
    if (m_size == m_capacity) t_age_full++;
    
    TimerMeasure START = Timer::now();
    
    auto it = m_map.find(key);
    bool hit = (it != m_map.end());
    
    if (hit)
    {
        m_queue.erase(it->second);
        m_map.erase(it);
        
        // Tracking
        if (m_size == m_capacity) t_hits++;
    }
    else m_tree.insert(make_pair(key, nullptr));
    
    m_queue.push_front(key);
    m_map.insert(make_pair(key, m_queue.begin()));
    
    clean();
    
    TimerMeasure END = Timer::now();
    
    // Tracking
    m_size = (int)m_map.size();
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
        auto map_it = m_map.find(it->first);
        m_queue.erase(map_it->second);
        m_map.erase(map_it);
        
        m_queue.push_front(it->first);
        m_map.insert(make_pair(it->first, m_queue.begin()));
        
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


int OrderedLRU::get_space()
{
    return (int) 8 * m_map.bucket_count() + (8 + 8) * m_size + m_capacity * 8 * 4 + (int)m_tree.size() * 8 * 4;
}


std::string OrderedLRU::get_name()
{
    return "OLRU";
}


std::string OrderedLRU::to_string()
{
    std::string str;
    
    for (std::string& key : m_queue)
    {
        str += key + " ";
    }
    
    return str;
}


void OrderedLRU::clean()
{
    while(m_map.size() > m_capacity)
    {
        m_map.erase(*m_queue.rbegin());
        m_tree.erase(*m_queue.rbegin());
        m_queue.pop_back();
    }
}
