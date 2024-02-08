#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// #define PRINT_OUTPUT


double find_root(double root_of){
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
    printf("iters: %d, root of: %lf root: %lf, reeal_root: %lf, is_close: %d\n", iterations, root_of, x_next, real_root, is_close);
#endif
    return is_close;
}


int main(int argc, char *argv[])
{
    int rc, rank, size, iters;
    double start_time, stop_time;

    if ((rc = MPI_Init(&argc, &argv))){
        MPI_Abort(MPI_COMM_WORLD, rc);
        return -1;
    }
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(time(NULL) + rank);
    if (rank == 0){
        iters = atoi(argv[1]);
        iters = iters / size + 1;
    }
    MPI_Bcast(&iters, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0){
        start_time = MPI_Wtime();
    }

    double root_of;
    
    int range = 100 / size;
    int start_from = range * rank;
    double delta = (double)range / (double)iters;

    for (int i = 0; i < iters; i++){
        root_of = (i + 1) * delta + start_from;
        find_root(root_of);
    }

    if (rank == 0){
        stop_time = MPI_Wtime();
        FILE* f = fopen("./results/res_mpi_c.txt", "a");
        fprintf(f, "%d\t%lf\n", size, stop_time - start_time);
    }
    MPI_Finalize();
}