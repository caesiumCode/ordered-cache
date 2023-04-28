#ifndef LSTree_hpp
#define LSTree_hpp

#include "../CacheBase.hpp"

struct LightTreeNode
{
    std::string key;
    void*       value;
    
    LightTreeNode* left;
    LightTreeNode* right;
    
    bool is_leaf()
    {
        return left == nullptr && right == nullptr;
    }
    
    // Tracking
    int timestamp;
    mutable int rank;
    
    // Display
    std::string to_string(std::string indent, bool last)
    {
        std::string node_string = "+-" + key + "(" + std::to_string(rank) + ")\n";
        
        if (!is_leaf())
        {
            std::string new_indent = indent + (last ? "   " : "|  ");
            
            if      (left != nullptr && right == nullptr)   return indent + node_string + left->to_string(new_indent, true);
            else if (left == nullptr && right != nullptr)   return indent + node_string + right->to_string(new_indent, true);
            else if (left != nullptr && right != nullptr)   return indent + node_string + left->to_string(new_indent, false) + right->to_string(new_indent, true);
        }
        
        return indent + node_string;
    }
};

class LSTree : public CacheBase
{
public:
    LSTree(int capacity);
    ~LSTree();
    
    void insert(const std::string& key);
    void prefix(const std::string& prefix_key);
    
    std::string get_name();
    int         get_space();
    std::string to_string();
    
private:
    LightTreeNode* m_block;
    LightTreeNode* m_root;
    
    std::vector<LightTreeNode*> path;
    
    // Heuristic
    void splay(LightTreeNode* node);
    
    // Tracking
    std::set<LightTreeNode*, std::function<bool(LightTreeNode*, LightTreeNode*)>> t_timestamp_heap;
    
    // BST operations
    LightTreeNode*  find(const std::string& key);
    void            insert_hit(const std::string& key, LightTreeNode* node);
    void            insert_miss(const std::string& key, LightTreeNode* node);
};

#endif /* LSTree_hpp */
