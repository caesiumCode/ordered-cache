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
        
    bool hit = m_map.contains(key);
        
    if (!hit)
    {
        if (m_size < m_capacity)
        {
            m_map[key] = nullptr;
            m_size++;
        }
    }
        
    // Tracking
    if (m_size == m_capacity) t_hits += hit;
}

void FixedCache::prefix(const std::string& prefix_key)
{
    // Tracking
    if (m_size == m_capacity) t_ranges++;
        
    long counter = 0;
        
    for (auto& it : m_map) if (it.first.compare(0, prefix_key.size(), prefix_key) == 0) counter++;
        
    // Tracking
    if (m_size == m_capacity) t_range_hits += counter;
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
