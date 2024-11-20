#include <iostream>
#include <pthread.h>
#include <time.h>

class Integer {
private:
  pthread_spinlock_t lock;
  int inner_value = 0;
public:
  Integer(int value) : inner_value(value) { pthread_spin_init(&lock, PTHREAD_PROCESS_SHARED); }
  ~Integer() { pthread_spin_destroy(&lock); }
  void inc() {
    pthread_spin_lock(&lock);
    inner_value++;
    pthread_spin_unlock(&lock);
  }
  int get_inner_value() {
    return inner_value;
  }
};

void* increment(void* arg) {
  Integer* num = (Integer*)arg;
  for (int i = 0; i < 10000; ++i) {
    num->inc();
  }
  return nullptr;
}

int main(int argc, char** argv) {
  Integer* num = new Integer(0);
  int thread_count = 50;
  pthread_t* tids = new pthread_t[thread_count];

  time_t start = clock();

  for (int i = 0; i < thread_count; ++i) {
    pthread_create(&tids[i], NULL, increment, num);  
  }

  for (int i = 0; i < thread_count; ++i) {
    pthread_join(tids[i], NULL);
  }

  time_t end = clock();

  std::cout << "Final value is " << num->get_inner_value() << std::endl;
  std::cout << "Time: " << ((float)(end - start)) / CLOCKS_PER_SEC << " seconds\n";
  
  delete[] tids;
  delete num;
  return 0;
}
