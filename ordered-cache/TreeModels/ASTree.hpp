#ifndef ASTree_hpp
#define ASTree_hpp

#include "DoubleRotationBase.hpp"

class ASTree : public DoubleRotationBase
{
public:
    ASTree(int capacity);
        
    std::string get_name();
    
protected:
    void attach_heuristic(TreeNode* node);
};

#endif /* ASTree_hpp */
