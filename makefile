all: mergesort.cpp
	g++ -fopenmp -o mergesort mergesort.cpp

clean:
	mergesort
