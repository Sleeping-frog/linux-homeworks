#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <pthread.h>
#include <vector>

void* sum_func(void* args) {
	int* sum = new int;
	int* arr = ((int**)args)[0];
	int start = *(int*)&((void**)args)[1];
	int end = *(int*)&((void**)args)[2];
	for (int i = start; i < end; ++i)
		*sum += arr[i];
	return sum;
}

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("wrong arguments\n");
		exit(-1);
	}
	int N = atoi(argv[1]);
	int M = atoi(argv[2]);

	int* array = new int[N];
	srand(time(NULL));
	for (int i = 0; i < N; ++i)
		array[i] = rand();
	
	int sum = 0;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; ++i) {
		sum += array[i];
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	printf("Time spent without threads: %ld microseconds\n", duration.count());
	
	sum = 0;
	pthread_t* tids = new pthread_t[M];
	void*** args = new void**[M];
	for (int i = 0; i < M; ++i) {
		args[i] = new void*[3];
		args[i][0] = array;
		args[i][1] = reinterpret_cast<void*>(i == 0 ? 0 : *(int*)&args[i-1][1] + N/M);
		args[i][2] = reinterpret_cast<void*>(i == 0 ? N/M : *(int*)&args[i-1][2] + N/M);
	}
	args[M-1][2] = reinterpret_cast<void*>(*(int*)&args[M-1][2] + N%M);
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < M; ++i) {
		pthread_create(&tids[i], 0, sum_func, (void*)args[i]);
		args[0] += N/M;
	}
	for (int i = 0; i < M; ++i) {
		int* tmp;
		pthread_join(tids[i], (void**)&tmp);
		sum += *tmp;
		delete tmp;
	}
	stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        printf("Time spent with %d threads: %ld microseconds\n", M, duration.count());

	delete[] args;
	
	delete[] tids;
	delete[] array;
}
