#ifndef CacheBase_hpp
#define CacheBase_hpp

#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <map>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <set>
#include <cmath>
#include <functional>
#include <list>

using Timer         = std::chrono::high_resolution_clock;
using TimerMeasure  = std::chrono::time_point<Timer>;

struct ListNode
{
    std::string key;
    void*       value;
    
    ListNode* previous;
    ListNode* next;
};


class CacheBase
{
public:
    CacheBase(int capacity);
    virtual ~CacheBase() = default;
    
    void set_tracking(bool tracking);
    
    void test(const std::string& path, const std::string& filename, bool prefix, int prefix_offset);
    
    virtual void        insert(const std::string& key)  = 0;
    virtual void        prefix(const std::string& prefix_key) = 0;
    
    virtual std::string to_string() = 0;
    virtual int         get_space() = 0;
    virtual std::string get_name()  = 0;
    
    void get_statistics(std::string dataset_name, int w = 14);
    virtual void get_tracking(std::string dataset_name);
    
protected:
    const int   m_capacity;
    int         m_size;
    bool        m_tracking;
    
    // Tracking
    std::chrono::duration<double, std::micro> t_chrono;
    int  t_age;
    int  t_age_full;
    int  t_hits;
    int  t_ranges;
    long t_range_hits;
    std::vector<int> t_deleted_ranks;
};

#endif /* CacheBase_hpp */
