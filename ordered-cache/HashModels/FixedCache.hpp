#ifndef FixedCache_hpp
#define FixedCache_hpp

#include "../CacheBase.hpp"

class FixedCache : public CacheBase
{
public:
    FixedCache(int capacity);
    
    void        insert(const std::string& key);
    void        prefix(const std::string& prefix_key);
    
    int         get_space();
    std::string get_name();
    std::string to_string();
    
private:
    std::unordered_map<std::string, void*> m_map;
};

#endif /* FixedCache_hpp */
