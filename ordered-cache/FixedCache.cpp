#include "FixedCache.hpp"

FixedCache::FixedCache(int capacity)
: CacheBase(capacity)
{
    m_map.clear();
}

void FixedCache::insert(const std::string &key)
{
    // Tracking
    t_age++;
    if (m_size == m_capacity) t_age_full++;
    
    TimerMeasure START = Timer::now();
    
    bool hit = m_map.contains(key);
        
    if (!hit)
    {
        if (m_size < m_capacity)
        {
            m_map[key] = nullptr;
            m_size++;
        }
    }
    else if (m_size == m_capacity) t_hits++;
    
    TimerMeasure END = Timer::now();
    
    // Tracking
    if (m_size == m_capacity)
    {
        t_chrono += END - START;
        
        if (hit) t_chrono_hit += END - START;
    }
}

void FixedCache::prefix(const std::string& prefix_key)
{
    // Tracking
    if (m_size == m_capacity) t_ranges++;
    
    TimerMeasure START = Timer::now();
    
    long counter = 0;
        
    for (auto& it : m_map)
    {
        if (it.first.compare(0, prefix_key.size(), prefix_key) == 0) counter++;
    }
    
    TimerMeasure END = Timer::now();
    
    // Tracking
    if (m_size == m_capacity)
    {
        t_range_hits += counter;
        t_chrono_range += END - START;
    }
}

int FixedCache::get_space()
{
    return 8 * (int)m_map.bucket_count() + (8 + 8) * m_size + m_capacity * 8 * 3;
}

std::string FixedCache::get_name()
{
    return "Fixed Cache";
}

std::string FixedCache::to_string()
{
    std::string str;
    
    for (const auto& [key, value] : m_map) str += key + " ";
    
    return str;
}
