#include "ARC.hpp"

ARC::ARC(int capacity)
: CacheBase(capacity)
{
    p = 0;
}

void ARC::insert(const std::string &key)
{
    // Tracking
    t_age++;
    if (T1.get_size() + B1.get_size() + T2.get_size() + B2.get_size() >= 2*m_capacity) t_age_full++;
    bool hit;
    
    TimerMeasure START = Timer::now();
    
    
    map_it node_T1 = T1.find(key);
    map_it node_B1 = B1.find(key);
    map_it node_T2 = T2.find(key);
    map_it node_B2 = B2.find(key);
    
    if (T1.contains(node_T1))
    {
        T1.remove(node_T1);
        T2.insert(key);
        
        hit = true;
        t_hits++;
    }
    if (T2.contains(node_T2))
    {
        T2.move_to_front(node_T2);
        
        hit = true;
        t_hits++;
    }
    else if (B1.contains(node_B1))
    {
        int B1_size = B1.get_size();
        int B2_size = B2.get_size();
        int delta = B1_size >= B2_size ? 1 : (B1_size == 0 ? m_capacity - p : B2_size / B1_size);
        p = std::min(p + delta, m_capacity);
        
        replace(B2.contains(node_B2));
        B1.remove(node_B1);
        T2.insert(key);
        
        hit = true;
        t_hits++;
    }
    else if (B2.contains(node_B2))
    {
        int B1_size = B1.get_size();
        int B2_size = B2.get_size();
        int delta = B2_size >= B1_size ? 1 : (B2_size == 0 ? p : B1_size / B2_size);
        p = std::max(p - delta, 0);
        
        replace(B2.contains(node_B2));
        B2.remove(node_B2);
        T2.insert(key);
        
        hit = true;
        t_hits++;
    }
    else if (T1.get_size() + B1.get_size() == m_capacity)
    {
        if (T1.get_size() < m_capacity)
        {
            if (B1.get_size() > 0) B1.remove_lru();
            replace(false);
        }
        else T1.remove_lru();
        
        T1.insert(key);
        
        hit = false;
    }
    else
    {
        int size = T1.get_size() + B1.get_size() + T2.get_size() + B2.get_size();
        if (size >= m_capacity)
        {
            if (size >= 2 * m_capacity) B2.remove_lru();
            replace(false);
        }
        
        T1.insert(key);
        
        hit = false;
    }
    
    // Tracking
    TimerMeasure END = Timer::now();
    t_chrono += END - START;
    
    if (hit) t_chrono_hit += END - START;
}

void ARC::prefix(const std::string &prefix_key)
{
    
}


void ARC::replace(bool is_in_B2)
{
    int T1_size = T1.get_size();
    
    if (T1_size > 0 && (T1_size > p || (is_in_B2 && T1_size == p))) B1.insert(T1.remove_lru().second);
    else if (T2.get_size() > 0)                                     B2.insert(T2.remove_lru().second);
}


int ARC::get_space()
{
    return T1.get_space() + B1.get_space() + T2.get_space() + B2.get_space();
}

std::string ARC::get_name()
{
    return "ARC";
}

std::string ARC::to_string()
{
    return "[ARC]";
}
