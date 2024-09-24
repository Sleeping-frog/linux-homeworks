
all: output

output: main.o Complex.o Sort.o
	g++ main.o Complex.o Sort.o -o output

main.o: main.cpp
	g++ -c main.cpp

Complex.o: Complex.cpp
	g++ -c Complex.cpp

Sort.o: Sort.cpp
	g++ -c Sort.cpp

clean:
	rm *.o output