#include <memory>

#include "CompositeModels/ARC.hpp"
#include "CompositeModels/ARC.cpp"
#include "CompositeModels/HLRU.hpp"
#include "CompositeModels/TLRU.hpp"

#include "HashModels/LRU.hpp"
#include "HashModels/OrderedLRU.hpp"
#include "HashModels/FixedCache.hpp"
#include "TreeModels/STree.hpp"
#include "TreeModels/MTRTree.hpp"
#include "TreeModels/SETree.hpp"
#include "TreeModels/LSTree.hpp"
#include "TreeModels/ASTree.hpp"

/*
 ./program <path> <dataset> <n> <model> <ranking> <prefix> <prefix_offset>
 */

int main(int argc, const char * argv[])
{
    if (argc > 8 || argc < 7)
    {
        std::cout << "INVALID ARGUMENT: Expected (6|7) arguments, " << argc-1 << " were given" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Set stream
    std::string path    = std::string(argv[1]);
    std::string dataset = std::string(argv[2]);
    
    // Set cache
    int n = std::atoi(argv[3]);
    std::unique_ptr<CacheBase> cache;
    
    if      (std::string(argv[4]) == "lru")     cache = std::make_unique<LRU>(n);
    else if (std::string(argv[4]) == "st")      cache = std::make_unique<STree>(n);
    else if (std::string(argv[4]) == "lst")     cache = std::make_unique<LSTree>(n);
    else if (std::string(argv[4]) == "ast")     cache = std::make_unique<ASTree>(n);
    else if (std::string(argv[4]) == "mtr")     cache = std::make_unique<MTRTree>(n);
    else if (std::string(argv[4]) == "se")      cache = std::make_unique<SETree>(n);
    else if (std::string(argv[4]) == "fc")      cache = std::make_unique<FixedCache>(n);
    else if (std::string(argv[4]) == "olru")    cache = std::make_unique<OrderedLRU>(n);
    else if (std::string(argv[4]) == "arc")     cache = std::make_unique<ARC<HLRU>>(n);
    else if (std::string(argv[4]) == "oarc")    cache = std::make_unique<ARC<TLRU>>(n);
    else
    {
        std::cout << "INVALID ARGUMENT: Unknown model " << argv[4] << std::endl;
        return EXIT_FAILURE;
    }
    
    // Set tracking
    bool tracking;
    if (std::atoi(argv[5]) == 0)    tracking = false;
    else                            tracking = true;
    
    cache->set_tracking(tracking);
    
    // Set prefix
    bool prefix = std::atoi(argv[6]);
    int prefix_offset = 0;
    if (argc == 8) prefix_offset = std::atoi(argv[7]);
        
    // Test cache
    cache->test(path, dataset, prefix, prefix_offset);
    
    // Output
    if (tracking)   cache->get_tracking(dataset);
    else            cache->get_statistics(dataset);
    
    return EXIT_SUCCESS;
}
