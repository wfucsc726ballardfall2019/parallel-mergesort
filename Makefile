all: mergesort
	g++ -fopenmp -O3 -o mergesort mergesort.cpp

clean:
	mergesort
