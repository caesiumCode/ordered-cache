#include "ARC.hpp"

template<typename LruT>
ARC<LruT>::ARC(int capacity)
: CacheBase(capacity)
, c(capacity/2)
, T1(capacity/2)
, B1(capacity/2)
, T2(2*(capacity/2))
, B2(2*(capacity/2))
{
    p = 0;
}

template<typename LruT>
void ARC<LruT>::insert(const std::string &key)
{
    // Tracking
    t_age++;
    uint64_t full_size = T1.size() + B1.size() + T2.size() + B2.size();
    if (full_size >= m_capacity) t_age_full++;
    bool hit;
    
    
    if (T1.find(key))
    {
        T1.move_last_hit_to_mru(T2);
        
        hit = true;
    }
    else if (T2.find(key))
    {
        T2.move_last_hit_to_front();
        
        hit = true;
    }
    else if (B1.find(key))
    {
        uint64_t B1_size = B1.size();
        uint64_t B2_size = B2.size();
        uint64_t delta = B1_size >= B2_size ? 1 : (B1_size == 0 ? c : B2_size / B1_size);
        p += delta;
        if (p > c) p = c;
        
        replace(false);
        B1.move_last_hit_to_mru(T2);
        
        hit = true;
    }
    else if (B2.find(key))
    {
        uint64_t B1_size = B1.size();
        uint64_t B2_size = B2.size();
        uint64_t delta = B2_size >= B1_size ? 1 : (B2_size == 0 ? c : B1_size / B2_size);
        if (p >= delta) p -= delta;
        else            p = 0;
        
        replace(true);
        B2.move_last_hit_to_mru(T2);
        
        hit = true;
    }
    else if (T1.size() + B1.size() == c)
    {
        if (T1.size() < c)
        {
            if (!B1.empty()) B1.delete_lru();
            replace(false);
        }
        else if (!T1.empty()) T1.delete_lru();
        
        T1.insert_new(key);
        
        hit = false;
    }
    else
    {
        if (full_size >= c)
        {
            if (full_size >= 2*c && !B2.empty()) B2.delete_lru();
            replace(false);
        }
        
        T1.insert_new(key);

        hit = false;
    }
    
    // Tracking
    if (full_size >= m_capacity) t_hits += hit;
}

template<typename LruT>
void ARC<LruT>::prefix(const std::string &prefix_key)
{
    
}


template<typename LruT>
void ARC<LruT>::replace(bool is_in_B2)
{
    uint64_t T1_size = T1.size();
    
    if (!T1.empty() && (T1_size > p || (is_in_B2 && T1_size == p))) T1.move_lru_to_mru(B1);
    else if (!T2.empty())                                           T2.move_lru_to_mru(B2);
}


template<typename LruT>
int ARC<LruT>::get_space()
{
    return int(T1.space()) + int(B1.space()) + int(T2.space()) + int(B2.space());
}

template<typename LruT>
std::string ARC<LruT>::get_name()
{
    return LruT::signature() + "ARC";
}

template<typename LruT>
std::string ARC<LruT>::to_string()
{
    return "[ARC]";
}
