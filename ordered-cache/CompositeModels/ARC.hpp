#ifndef ARC_hpp
#define ARC_hpp

#include "../CacheBase.hpp"
#include "ALRU.hpp"

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
    ALRU T1;
    ALRU B1;
    ALRU T2;
    ALRU B2;
    
    int p;
    
private:
    void replace(bool is_in_B2);
};

#endif /* ARC_hpp */
