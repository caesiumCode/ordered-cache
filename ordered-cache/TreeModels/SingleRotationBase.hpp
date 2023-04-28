#ifndef SingleRotationBase_hpp
#define SingleRotationBase_hpp

#include "LeafQueueBase.hpp"

class SingleRotationBase : public LeafQueueBase
{
public:
    SingleRotationBase(int capacity);
        
    virtual std::string get_name() = 0;
    
protected:
    void attach_heuristic(TreeNode* node);
    
    virtual void move_heuristic(TreeNode* node) = 0;
    void rotate_up(TreeNode* node);
};

#endif /* SingleRotationBase_hpp */
