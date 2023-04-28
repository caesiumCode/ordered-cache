#ifndef SETree_hpp
#define SETree_hpp

#include "SingleRotationBase.hpp"

class SETree : public SingleRotationBase
{
public:
    SETree(int capacity);
        
    std::string get_name();
    
protected:
    void move_heuristic(TreeNode* node);
};

#endif /* SETree_hpp */
