#include "ALRU.hpp"

ALRU::ALRU()
{
    
}

void ALRU::insert(const std::string &key)
{
    auto it = m_map.find(key);
    bool hit = it != m_map.end();
    
    if (hit)
    {
        m_queue.erase(it->second);
        m_queue.push_front(key);
        
        it->second = m_queue.begin();
    }
    else
    {
        m_queue.push_front(key);
        m_map.insert(make_pair(key, m_queue.begin()));
    }
}

map_it ALRU::find(const std::string &key)
{
    return m_map.find(key);
}

bool ALRU::contains(map_it it)
{
    return it != m_map.end();
}

void ALRU::move_to_front(map_it it)
{
    if (it != m_map.end())
    {
        m_queue.erase(it->second);
        m_queue.push_front(it->first);
        
        it->second = m_queue.begin();
    }
}

void ALRU::remove(map_it it)
{
    if (it != m_map.end())
    {
        m_queue.erase(it->second);
        m_map.erase(it);
    }
}

std::pair<bool, std::string> ALRU::remove_lru()
{
    if (m_queue.size() > 0)
    {
        std::string key = m_queue.back();
        
        m_queue.pop_back();
        m_map.erase(key);
        
        return std::make_pair(true, key);
    }
    else return std::make_pair(false, "");
}

int ALRU::get_size()
{
    return (int)m_queue.size();
}

int ALRU::get_space()
{
    return 8 * int(m_map.bucket_count()) + (8 + 8) * int(m_queue.size()) + int(m_queue.size()) * 8 * 4;
}
