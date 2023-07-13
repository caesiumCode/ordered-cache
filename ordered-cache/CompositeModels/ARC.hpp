#ifndef ARC_hpp
#define ARC_hpp

#include "../CacheBase.hpp"

template<typename LruT>
class ARC : public CacheBase
{
public:
    ARC(int capacity);
    ~ARC() = default;
    
    void        insert(const std::string& key);
    void        prefix(const std::string& prefix_key);
    
    int         get_space();
    std::string get_name();
    std::string to_string();
    
private:
    LruT T1;
    LruT B1;
    LruT T2;
    LruT B2;
    
    uint64_t c;
    uint64_t p;
    
    //std::string tmp, tmp2;
    
private:
    void replace(bool is_in_B2);
};

#endif /* ARC_hpp */
