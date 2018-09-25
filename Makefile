#Gavan Adrian George - 314CA

build: tema2

tema2: tema2.o
	g++ -Wall tema2.o -o tema2

tema2.o: tema2.cc
	g++ -Wall -c tema2.cc

clean:
	rm -f *.o tema2
