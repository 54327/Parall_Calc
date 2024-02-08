#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>


// #define PRINT_OUTPUT


int find_root(double root_of){
    int is_close;
    double x_prev;
    double real_root = sqrt(root_of);
    double x_next = root_of;
    int iterations = 0;
    double eps = 1e-6;
    
    do {
        x_prev = x_next;
        x_next = ((x_prev * x_prev) + root_of) / (x_prev * 2);
        iterations++;
    } while (fabs(x_next - x_prev) > eps);

    is_close = fabs(real_root - x_next) < eps;
#ifdef PRINT_OUTPUT
    printf("iterations: %d, root of: %lf root: %lf, reeal_root: %lf, is_close: %d\n", iterations, root_of, x_next, real_root, is_close);
    fflush(stdout);
#endif
    return is_close;
}

int main(int argc, char *argv[])
{
    struct timeval tval_before, tval_after, tval_result;
    int iters, num_threads;
    double root_of;

    num_threads = atoi(argv[1]);
    iters = atoi(argv[2]);
    iters = iters / num_threads + 1;
    
    gettimeofday(&tval_before, NULL);
    #pragma omp parallel for
    for (int i = 0; i < iters; i++){
        root_of = (double)i / (double)iters * 100.0;
        find_root(root_of);
    }
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);

    FILE* f = fopen("./results/res_omp.txt", "a");
    fprintf(f, "%d\t%ld.%06ld\n", num_threads, (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
}