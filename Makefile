all: shell1.o shell2.o
	g++ -std=gnu++11 -o shell1 shell1.o
	g++ -std=gnu++11 -o shell2 shell2.o
shell1.o: shell1.cpp
	g++ -std=gnu++11 -c shell1.cpp
shell2.o: shell2.cpp
	g++ -std=gnu++11 -c shell2.cpp
clean:
	rm *.o