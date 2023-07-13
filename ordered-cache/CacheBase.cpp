#include "CacheBase.hpp"

CacheBase::CacheBase(int capacity)
:   m_capacity(capacity)
{
    m_size = 0;
    m_tracking = false;
    
    t_chrono        = std::chrono::microseconds(0);
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

void CacheBase::test(const std::string& path, const std::string& filename, bool prefix, int prefix_offset)
{
    const int   LINE_BUFFER_SIZE = 1 << 10;
    char        line_buffer[LINE_BUFFER_SIZE];
    std::FILE* fp = std::fopen((path + filename).c_str(), "r");
    
    TimerMeasure START = Timer::now();
    while (std::fgets(line_buffer, sizeof(line_buffer), fp))
    {
        std::string key(line_buffer);
        if (key.back() == '\n') key.pop_back();
        
        this->insert(key);
        
        if (prefix)
        {
            int s = (int)key.size();
            while (s > 1 && key[s-1] != '/') s--;
            if (s == 1) s = (int)key.size();

            this->prefix(key.substr(0, s + prefix_offset));
        }
    }
    TimerMeasure END = Timer::now();
        
    t_chrono = END - START;
    std::fclose(fp);
}

void CacheBase::get_statistics(std::string dataset_name, int w)
{
    std::cout << std::setw(w) << dataset_name;
    std::cout << std::setw(w) << get_name();
    std::cout << std::setw(w) << m_capacity;
    std::cout << std::setw(w) << get_space();
    std::cout << std::setw(w) << (t_age_full > 0 ? double(t_hits)/double(t_age_full) : 0);
    std::cout << std::setw(w) << std::setw(w) << (t_age_full > 0 ? std::chrono::duration<double, std::micro>(t_chrono).count() / double(t_age_full) : 0);
    std::cout << std::setw(w) << std::setw(w) << (t_ranges > 0 ? double(t_range_hits) / double(t_ranges) : 0);
    std::cout << std::endl;
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
