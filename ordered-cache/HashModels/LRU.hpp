#ifndef LRU_hpp
#define LRU_hpp

#include "../CacheBase.hpp"

class LRU : public CacheBase
{
public:
    LRU(int capacity);
    ~LRU();
    
    void        insert(const std::string& key);
    void        prefix(const std::string& prefix_key);
    
    int         get_space();
    std::string get_name();
    std::string to_string();
    
private:
    ListNode* m_block;
    ListNode* m_front;
    ListNode* m_back;
    
    std::unordered_map<std::string, ListNode*> m_map;
    
private:
    void detach(ListNode* node);
    void attach(ListNode* node);
};

#endif /* LRU_hpp */
