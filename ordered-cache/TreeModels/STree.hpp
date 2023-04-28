#ifndef STree_hpp
#define STree_hpp

#include "DoubleRotationBase.hpp"

class STree : public DoubleRotationBase
{
public:
    STree(int capacity);
        
    std::string get_name();
    
protected:
    void attach_heuristic(TreeNode* node);
};

#endif /* STree_hpp */
