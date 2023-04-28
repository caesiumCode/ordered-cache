#include <iostream>
#include <string>
#include <unordered_set>
#include <iomanip>

/*
 ./program <path> <dataset>
 */

int main(int argc, const char * argv[])
{
    // Set stream
    std::string path    = std::string(argv[1]);
    std::string dataset = std::string(argv[2]);
    
    const int   LINE_BUFFER_SIZE = 1 << 10;
    char        line_buffer[LINE_BUFFER_SIZE];
    std::FILE* fp = std::fopen((path + dataset).c_str(), "r");
    
    // Set trackers
    long stream_size = 0;
    long key_size = 0;
    std::unordered_set<std::string> key_set;
    
    // Read dataset
    while (std::fgets(line_buffer, sizeof(line_buffer), fp))
    {
        std::string key(line_buffer);
        if (key.back() == '\n') key.pop_back();
             
        key_set.insert(key);
        
        stream_size++;
        key_size += key.size();
    }
        
    std::fclose(fp);
    
    // Output results
    int w = 14;
    std::cout << std::setw(w) << dataset;
    std::cout << std::setw(w) << key_set.size();
    std::cout << std::setw(w) << stream_size;
    std::cout << std::setw(w) << double(key_size) / double(stream_size) << std::endl;
    
    return EXIT_SUCCESS;
}
