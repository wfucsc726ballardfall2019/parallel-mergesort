# parallel-mergesort

The starter code implements sequential mergesort, using the C++ STL's merge and copy algorithms to do the merge.
It also sorts using the STL algorithm to check correctness and compare timings.

Our goal is to implement the PRAM parallel mergesort algorithm as a class, so we'll all share the same repo.
Below are the (parallel) tasks.  Once your group completes a task, commit and push to the repo.
  
1. **spawn** Implement a new base case to avoid deep recursion overhead in mergesort 
    * call STL's sort when the problem size is sufficiently small
2. **spawn** Parallelize mergesort's recursive calls using OpenMP task parallelism
    * see https://openmp.org/wp-content/uploads/sc13.tasking.ruud.pdf 
3. **spawn** Write recursive merge routine with specified signature
    * use medianofunion function provided
4. **sync** Parallelize merge's recursive calls
    * let's do this one together after we've completed (we can use same techniques as mergesort)
