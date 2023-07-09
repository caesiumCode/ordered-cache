#ifndef OrderedLRU_hpp
#define OrderedLRU_hpp

#include "../CacheBase.hpp"

class OrderedLRU : public CacheBase
{
public:
    OrderedLRU(int capacity);
    ~OrderedLRU() = default;
    
    void        insert(const std::string& key);
    void        prefix(const std::string& prefix_key);
    
    int         get_space();
    std::string get_name();
    std::string to_string();
    
private:
    std::list<std::string>                                      m_queue;
    std::unordered_map<std::string, decltype(m_queue.begin())>  m_map;
    std::map<std::string, void*, std::less<>>                   m_tree;
    
private:
    void clean();
};

#endif /* OrderedLRU_hpp */
