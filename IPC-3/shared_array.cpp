#include "shared_array.h"
#define SHM_NAME "Shared_arrays"
#define SHM_SIZE 1024

shared_array::shared_array(const std::string& name, std::size_t size) {
    if (free_space == nullptr) {
        int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1) {
            perror("shm_open");
            exit(-1);
        }
        if (ftruncate(shm_fd, SHM_SIZE) == -1) {
            perror("ftruncate");
            exit(-1);
        }
        void* shm_ptr = mmap(nullptr, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (shm_ptr == MAP_FAILED) {
            perror("mmap");
            exit(-1);
        }
    }
    ++count;
    auto it = arrays.find(std::pair<std::string, std::size_t>(name, size));
    auto [it, inserted] = arrays.insert({{name, size}, nullptr});
}
