#include <unordered_map>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

class shared_array {
public:
    shared_array(const std::string& name, std::size_t size);

private:
    static std::unordered_map<std::pair<std::string, std::size_t>, int*> arrays;
    static int* free_space;
    static int count;
    int* ptr;
    std::size_t size;
};