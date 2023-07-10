#ifndef ALRU_hpp
#define ALRU_hpp

#include <utility>
#include <string>
#include <list>
#include <unordered_map>

using list_it = std::list<std::string>::reverse_iterator;
using map_it = std::unordered_map<std::string, std::list<std::string>::iterator>::iterator;

class ALRU
{
public:
    ALRU();
    
    void        insert(const std::string& key);
    void        prefix(const std::string& prefix_key);
    
    map_it      find(const std::string& key);
    bool        contains(map_it it);
    void        move_to_front(map_it it);
    void        remove(map_it it);
    std::pair<bool, std::string> remove_lru();
    int         get_size();
    
    int         get_space();
    
private:
    std::list<std::string> m_queue;
    std::unordered_map<std::string, std::list<std::string>::iterator> m_map;
};

#endif /* ALRU_hpp */
