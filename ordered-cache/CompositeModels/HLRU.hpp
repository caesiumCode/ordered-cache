#ifndef HLRU_hpp
#define HLRU_hpp

#include "../CacheBase.hpp"

class HLRU
{
public:
    HLRU(uint32_t capacity);
    ~HLRU();
    
    static std::string signature() {return "";}
    bool empty();
    std::size_t size();
    std::size_t space();
    std::string print() {return "";}
    
    void insert_new(const std::string& key);
    bool find(const std::string& key);
    
    void move_last_hit_to_front();
    void move_last_hit_to_mru(HLRU& lru);
    void move_lru_to_mru(HLRU& lru);
    
    void delete_lru();
    
private:
    const std::size_t CAPACITY;
    
    ListNode* m_queue;
    ListNode* m_front;
    ListNode* m_back;
    std::size_t m_size;
    
    std::unordered_map<std::string, ListNode*> m_map;
    
private:
    ListNode* last_hit;
    
private:
    void detach(ListNode* node);
    void remove(ListNode* node);
    void attach(ListNode* node);    
};

#endif /* HLRU_hpp */
