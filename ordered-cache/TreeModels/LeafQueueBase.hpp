#ifndef LeafQueueBase_hpp
#define LeafQueueBase_hpp

#include "../CacheBase.hpp"

struct TreeNode
{
    std::string key;
    void*       value;
    
    bool leaf;
    
    TreeNode* parent;
    TreeNode* left;
    TreeNode* right;
    
    // Tracking
    int timestamp;
    mutable int rank;
    
    // Display
    std::string to_string(std::string indent, bool last)
    {
        std::string node_string = "+-" + key + "(" + std::to_string(rank) + ")\n";
        
        if (!leaf)
        {
            std::string new_indent = indent + (last ? "   " : "|  ");
            
            if      (left != nullptr && right == nullptr)   return indent + node_string + left->to_string(new_indent, true);
            else if (left == nullptr && right != nullptr)   return indent + node_string + right->to_string(new_indent, true);
            else if (left != nullptr && right != nullptr)   return indent + node_string + left->to_string(new_indent, false) + right->to_string(new_indent, true);
        }
        
        return indent + node_string;
    }
};




class LeafQueueBase : public CacheBase
{
public:
    LeafQueueBase(int capacity);
    ~LeafQueueBase();
    
    void insert(const std::string& key);
    void prefix(const std::string& prefix_key);
    
    virtual std::string get_name() = 0;
    int         get_space();
    std::string to_string();
    
    void get_tracking(std::string dataset_name);
    
protected:
    TreeNode* m_root;
    
    // Rule
    virtual void move_heuristic(TreeNode* node) = 0;
    virtual void attach_heuristic(TreeNode* node) = 0;
    
    // Leaf Queue interface
    void        push_leaf_queue(TreeNode* node);
    TreeNode*   pop_leaf_queue();
    void        remove_leaf_queue(TreeNode* node);
    
private:
    TreeNode* m_block;
    TreeNode* m_leaf_queue_front;
    TreeNode* m_leaf_queue_back;
    
    // Tracking
    std::set<TreeNode*, std::function<bool(TreeNode*, TreeNode*)>> t_timestamp_heap;
    std::vector<int> t_leafset_sizes;
    int t_leafset_size;
    
    void update_leafset(int ds);
    
    // BST operations
    TreeNode*   find(const std::string& key);
    
    void        insert_hit(const std::string& key, TreeNode* node);
    void        insert_miss(const std::string& key, TreeNode* node);
    
    // Leaf Queue update
    void move_to_back(TreeNode* node);
};

#endif /* LeafQueueBase_hpp */
