boardsweep: main.o
	gcc -o boardsweep main.o
main.o: main.cpp
	gcc -c main.cpp
clean:
	rm boardsweep; rm -f *.o

