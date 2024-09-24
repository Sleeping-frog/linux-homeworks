#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Complex.h"
#include "Sort.h"

#define SIZE 10

int main() {
    Complex c1(5.2, 2);

    Complex c2(5);

    std::cout << ( 2.72 * (c1 + c2) ) - Complex(3, 1) << std::endl;

    Complex array[SIZE];

    unsigned int seed = time(NULL);
    srand(seed);

    for (int i = 0; i < SIZE; ++i) {
        array[i] = Complex(rand() % 100, rand() % 100);   
    }

    Sort(array, SIZE);

    for (int i = 0; i < SIZE; ++i) {
        std::cout << array[i] << std::endl;
    }

    return 0;
}
