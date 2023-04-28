#include <memory>

#include "LRU.hpp"
#include "FixedCache.hpp"
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
    
    const int   LINE_BUFFER_SIZE = 1 << 10;
    char        line_buffer[LINE_BUFFER_SIZE];
    std::FILE* fp = std::fopen((path + dataset).c_str(), "r");
    
    // Set cache
    int n = std::atoi(argv[3]);
    std::unique_ptr<CacheBase> cache;
    
    if      (std::string(argv[4]) == "lru") cache = std::make_unique<LRU>(n);
    else if (std::string(argv[4]) == "st")  cache = std::make_unique<STree>(n);
    else if (std::string(argv[4]) == "lst") cache = std::make_unique<LSTree>(n);
    else if (std::string(argv[4]) == "ast") cache = std::make_unique<ASTree>(n);
    else if (std::string(argv[4]) == "mtr") cache = std::make_unique<MTRTree>(n);
    else if (std::string(argv[4]) == "se")  cache = std::make_unique<SETree>(n);
    else if (std::string(argv[4]) == "fc")  cache = std::make_unique<FixedCache>(n);
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
    while (std::fgets(line_buffer, sizeof(line_buffer), fp))
    {
        std::string key(line_buffer);
        if (key.back() == '\n') key.pop_back();
        
        cache->insert(key);
        
        if (prefix)
        {
            int s = (int)key.size();
            while (s > 1 && key[s-1] != '/') s--;
            if (s == 1) s = (int)key.size();

            cache->prefix(key.substr(0, s+prefix_offset));
        }
    }
        
    std::fclose(fp);
    
    // Output
    if (tracking)   cache->get_tracking(dataset);
    else            cache->get_statistics(dataset);
    
    return EXIT_SUCCESS;
}
