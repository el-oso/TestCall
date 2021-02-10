#include <julia.h>
JULIA_DEFINE_FAST_TLS() // only define this once, in an executable (not in a shared library) if you want fast code.

#include <vector>
#include <chrono>
#include <iostream>
using namespace std::chrono; 
using namespace std; 

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define N 150000
#define NCALLS 50

#define ALLOCATING 0 /* Use allocating code  */

int main(int argc, char *argv[])
{
    jl_init();
    jl_value_t* array_type = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);

    srand (time(NULL));
    double v[N];

    for(int i=0; i<N; i++)
        v[i] =  (double) rand() / RAND_MAX;

  
    jl_array_t *x = jl_ptr_to_array_1d(array_type, v, N, 0);

    // get a C function pointer to the Julia erf function compiled for
    jl_eval_string("import TestCall");
    jl_value_t* mod = (jl_value_t*) jl_eval_string("TestCall");

    #if ALLOCATING
    jl_array_t *out;
    jl_function_t* func = jl_get_function((jl_module_t*)mod, "erfv");
    #else 
    jl_function_t* func = jl_get_function((jl_module_t*)mod, "erfv2");
    jl_array_t *out = jl_ptr_to_array_1d(array_type, v, N, 0);
    #endif

    // Just being lazy and letting C++ determine the types for me
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    
    double *outData;

    jl_gc_collection_t gctype = JL_GC_FULL;

    // First Call 
    for(size_t n=0; n<=NCALLS; n++){

        jl_gc_enable(0);
        start = high_resolution_clock::now(); 

        #if ALLOCATING
        out = (jl_array_t*)jl_call1(func, (jl_value_t*)x);
        #else
        jl_call2(func, (jl_value_t*)out, (jl_value_t*)x);
        #endif

        outData = (double*)jl_array_data(out);

        stop = high_resolution_clock::now(); 
        duration = duration_cast<microseconds>(stop - start); 

        jl_gc_collect(JL_GC_FULL);
        jl_gc_enable(1);	

        cout << duration.count() << endl; 
    }

    // Print out the vector
    if (N <= 10){
        for(size_t i=0; i<jl_array_len(x); i++)
            std::cout << outData[i] << ' ';

        std::cout << "\n";
    }
  
    jl_atexit_hook(0);
    return 0;
}