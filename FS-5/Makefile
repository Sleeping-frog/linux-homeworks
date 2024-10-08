all: manual-append

manual-append: manual-append.o
	g++ manual-append.o -o manual-append

manual-append.o: manual-append.cpp
	g++ -c manual-append.cpp -o manual-append.o

clean:
	rm *.o manual-append
