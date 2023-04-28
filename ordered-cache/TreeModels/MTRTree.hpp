#ifndef MTRTree_hpp
#define MTRTree_hpp

#include "SingleRotationBase.hpp"

class MTRTree : public SingleRotationBase
{
public:
    MTRTree(int capacity);
        
    std::string get_name();
    
protected:
    void move_heuristic(TreeNode* node);
};

#endif /* MTRTree_hpp */
