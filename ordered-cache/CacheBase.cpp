#include "CacheBase.hpp"

CacheBase::CacheBase(int capacity)
:   m_capacity(capacity)
{
    m_size = 0;
    m_tracking = false;
    
    t_chrono        = std::chrono::microseconds(0);
    t_chrono_range  = std::chrono::microseconds(0);
    t_chrono_hit    = std::chrono::microseconds(0);
    t_age           = 0;
    t_age_full      = 0;
    t_hits          = 0;
    t_ranges        = 0;
    t_range_hits    = 0;
    t_deleted_ranks = std::vector<int>(101, 0);
}

void CacheBase::set_tracking(bool tracking)
{
    m_tracking = tracking;
}

void CacheBase::get_statistics(std::string dataset_name, int w)
{
    std::cout << std::setw(w) << dataset_name;
    std::cout << std::setw(w) << get_name();
    std::cout << std::setw(w) << m_capacity;
    std::cout << std::setw(w) << get_space();
    std::cout << std::setw(w) << (t_age_full > 0 ? double(t_hits)/double(t_age_full) : 0);
    std::cout << std::setw(w) << std::setw(w) << (t_age_full > 0 ? std::chrono::duration<double, std::micro>(t_chrono).count() / double(t_age_full) : 0);
    std::cout << std::setw(w) << std::setw(w) << (t_hits > 0 ? std::chrono::duration<double, std::micro>(t_chrono_hit).count() / double(t_hits) : 0);
    std::cout << std::setw(w) << std::setw(w) << (t_ranges > 0 ? double(t_range_hits) / double(t_ranges) : 0);
    std::cout << std::setw(w) << std::setw(w) << (t_ranges > 0 ? std::chrono::duration<double, std::micro>(t_chrono_range).count() / double(t_ranges) : 0) << std::endl;
}

void CacheBase::get_tracking(std::string dataset_name)
{
    std::cout << dataset_name << " ";
    std::cout << get_name() << " ";
    std::cout << m_capacity << " ";
    std::cout << "rank ";
    
    for (long count : t_deleted_ranks) std::cout << count << " ";
    std::cout << std::endl;
}
