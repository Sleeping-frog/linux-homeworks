All: do-command

do-command: do-command.o
	g++ do-command.o -o do-command

do-command.o: do-command.cpp
	g++ -c do-command.cpp -o do-command.o

clean:
	rm *.o do-command
