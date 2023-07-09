#include "LRU.hpp"

LRU::LRU(int capacity)
: CacheBase(capacity)
{
    m_map.clear();
    m_queue.clear();
}

void LRU::insert(const std::string &key)
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

void LRU::prefix(const std::string& prefix_key)
{
    // Tracking
    if (m_size == m_capacity) t_ranges++;
    
    TimerMeasure START = Timer::now();
    
    long counter = 0;
    auto it = m_map.begin();
            
    while (it != m_map.end())
    {
        std::string key = it->first;
        if (key.compare(0, prefix_key.size(), prefix_key) == 0)
        {
            counter++;
            
            m_queue.erase(it->second);
            m_map.erase(it++);
            
            m_queue.push_front(key);
            m_map.insert(make_pair(key, m_queue.begin()));
        }
        else it++;
    }
    
    TimerMeasure END = Timer::now();
    
    // Tracking
    if (m_size == m_capacity)
    {
        t_range_hits += counter;
        t_chrono_range += END - START;
    }
}

void LRU::clean()
{
    while(m_map.size() > m_capacity)
    {
        m_map.erase(*m_queue.rbegin());
        m_queue.pop_back();
    }
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
    
    for (std::string& key : m_queue)
    {
        str += key + " ";
    }
    
    return str;
}
