#ifndef DoubleRotationBase_hpp
#define DoubleRotationBase_hpp

#include "LeafQueueBase.hpp"

class DoubleRotationBase : public LeafQueueBase
{
public:
    DoubleRotationBase(int capacity);
        
    virtual std::string get_name() = 0;
    
protected:
    virtual void attach_heuristic(TreeNode* node) = 0;
    
    void move_heuristic(TreeNode* node);
    void rotate_up(TreeNode* node);
};

#endif /* DoubleRotationBase_hpp */
