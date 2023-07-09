#ifndef LRU_hpp
#define LRU_hpp

#include "../CacheBase.hpp"

class LRU : public CacheBase
{
public:
    LRU(int capacity);
    ~LRU() = default;
    
    void        insert(const std::string& key);
    void        prefix(const std::string& prefix_key);
    
    int         get_space();
    std::string get_name();
    std::string to_string();
    
private:
    std::list<std::string>                                      m_queue;
    std::unordered_map<std::string, decltype(m_queue.begin())>  m_map;
    
private:
    void clean();
};

#endif /* LRU_hpp */
