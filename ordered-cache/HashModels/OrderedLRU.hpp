#ifndef OrderedLRU_hpp
#define OrderedLRU_hpp

#include "../CacheBase.hpp"

class OrderedLRU : public CacheBase
{
public:
    OrderedLRU(int capacity);
    ~OrderedLRU();
    
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
    std::map<std::string, ListNode*, std::less<>> m_tree;
    
private:
    void detach(ListNode* node);
    void attach(ListNode* node);
};

#endif /* OrderedLRU_hpp */
