#ifndef TLRU_hpp
#define TLRU_hpp

#include "../TreeModels/LeafQueueBase.hpp"

class TLRU
{
public:
    TLRU(uint32_t capacity);
    ~TLRU();
    
    static std::string signature() {return "O";}
    bool empty();
    std::size_t size();
    std::size_t space();
    std::string print() {return m_root ? m_root->to_string("", true) : "";}
    
    void insert_new(const std::string& key);
    bool find(const std::string& key);
    
    void move_last_hit_to_front();
    void move_last_hit_to_mru(TLRU& lru);
    void move_lru_to_mru(TLRU& lru);
    
    void delete_lru();
    
private:
    const std::size_t CAPACITY;
    
    TreeNode* m_block;
    
    TreeNode* m_root;
    TreeNode* m_leaf_queue_front;
    TreeNode* m_leaf_queue_back;
    std::size_t m_size;
        
private:
    TreeNode* last_hit;
    
private:
    // Update rule
    void rotate_up(TreeNode* node);
    void rotate_up_single(TreeNode* node);
    void move_heuristic(TreeNode* node);
    void attach_heuristic(TreeNode* node);
    
    // Remove node
    TreeNode* find_loc(const std::string& key);
    TreeNode* find_largest_node(TreeNode* node);
    void remove_internal_node(TreeNode* node);
    void remove(TreeNode* node);
    
    // Leaf Queue interface
    void        push_leaf_queue(TreeNode* node);
    TreeNode*   pop_leaf_queue();
    void        remove_leaf_queue(TreeNode* node);
};

#endif /* TLRU_hpp */
