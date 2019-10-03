#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <ctime>
#include <omp.h>
using namespace std;

void mergesort(int* a, int* tmp, int n);
int medianofunion(int* a, int n, int* b, int m);
void recmerge(int* a, int n, int* b, int m, int* tmp);

int main(int argc, char** argv) {

    // read inputs
    if(argc != 4) {
        cout << "Usage: ./a.out seed length nthreads" << endl;
        return 1;
    }
    srand(atoi(argv[1]));
    int length = atoi(argv[2]);
    omp_set_num_threads(atoi(argv[3]));

    // allocate memory
    int* v = new int[length]; // array to be sorted
    int* t = new int[length]; // temporary workspace
    int* v2 = new int[length]; // copy for checking

    // initialize input randomly
    for (int i = 0; i < length; i++) {
        v[i] = v2[i] = rand() % length;
    }

    // sort array using mergesort (and time it)
    double start_mergeSort = omp_get_wtime();
    #pragma omp parallel shared(v,t)
    {
      #pragma omp single nowait
      {
        mergesort(v, t, length);
      }
    }
    double elapsed_mergeSort = omp_get_wtime() - start_mergeSort;

    // sort array using STL (and time it)
    double start_sort = omp_get_wtime();
    sort(v2,v2+length);
    double elapsed_sort = omp_get_wtime() - start_sort;

    // check outputs, make sure they match
    for(int i = 0; i < length; i++) {
        assert(v[i] == v2[i]);
    }

    cout << "time: " << elapsed_mergeSort << '\t' << elapsed_sort << endl;

    delete [] v2;
    delete [] t;
delete [] v;

}

// sorts array a of length n, tmp is workspace of length n
void mergesort(int* a, int* tmp, int n)
{
    if(n <= 1000) {
        sort(a, a+n);
    }
    else {
        // sort left and right recursively
        int mid = n / 2;
        #pragma omp task shared(a,tmp)
        mergesort(a, tmp, mid);
        mergesort(a + mid, tmp + mid, n - mid);
        #pragma omp taskwait

        // merge left and right into tmp and copy back into a (using STL)
        // merge(a, a+mid, a+mid, a+n, tmp);
        recmerge(a, mid, a+mid, n-mid, tmp);
        //copy(tmp,tmp+n,a);
        #pragma omp parallel for shared(a,tmp)
        for(int i = 0; i < n; i++) {
            a[i] = tmp[i];
        }
    }
}
    
// computes median of union of array a of length n and array b of length m
// assuming elements of a and b are already internally sorted
//   if m+n is even it returns one of the two middle values
int medianofunion(int *a, int n, int *b, int m) {
    int min_index = 0, max_index = n, i, j;
    while (min_index <= max_index) {
        i = (min_index + max_index) / 2;
        j = ((n + m + 1) / 2) - i;
        // Searching on right
        if (i < n && j > 0 && b[j - 1] > a[i])
            min_index = i + 1;
        // Searching on left
        else if (i > 0 && j < m && b[j] < a[i - 1])
            max_index = i - 1;
        // Found desired halves
        else {
            // no elements in 1st half of a
            if (i == 0) return b[j - 1];
            // no elements in 1st half of b
            else if (j == 0) return a[i - 1];
            else return max(a[i - 1], b[j - 1]);
        }
    }
}

void recmerge(int* a, int n, int* b, int m, int* tmp) {
    if(n==0){
        copy(b,b+m,tmp);
        return;
    }
    if(m==0){
        copy(a,a+n,tmp);
        return;
    }

    cout << n << " " << m << endl;
    
    int Ms = medianofunion(a, n, b, m);
    int i = (int)(lower_bound(a, a+n, Ms) - a);
    int j = (int)(lower_bound(b, b+m, Ms) - b);

    #pragma omp task shared(a,b,tmp)
    recmerge(a, i, b, j, tmp);
    recmerge(a+i, n-i, b+j, m-j, tmp+i+j);
    #pragma omp taskwait
    
}
