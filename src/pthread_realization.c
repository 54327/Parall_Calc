#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <bits/pthreadtypes.h>

// #define PRINT_OUTPUT

typedef struct{
    int iters;
    int thread_id;
    int num_threads;
} ThreadData;

int find_root(double root_of, int thread_id){
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
    printf("th_id: %d, iterations: %d, root of: %lf root: %lf, reeal_root: %lf, is_close: %d\n",thread_id, iterations, root_of, x_next, real_root, is_close);
    fflush(stdout);
#endif
    return is_close;
}


void* run_n_iters(void* thread_data){
    ThreadData* data = (ThreadData*)thread_data;
    int iters = data->iters;
    int thread_id = data->thread_id;
    int num_threads = data->num_threads;
    
    double root_of;
    
    int range = 100 / num_threads;
    int start_from = range * thread_id;
    double delta = (double)range / (double)iters;

    for (int i = 0; i < iters; i++){
        root_of = (i + 1) * delta + start_from;
        find_root(root_of, thread_id);
    }
    fflush(stdout);
    return NULL;
}


int main(int argc, char *argv[])
{
    struct timeval tval_before, tval_after, tval_result;
    int iters, num_threads;

    num_threads = atoi(argv[1]);
    iters = atoi(argv[2]);
    iters = iters / num_threads + 1;
    
    pthread_t* threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
    ThreadData* thread_data = (ThreadData*)malloc(num_threads * sizeof(ThreadData));
    
    gettimeofday(&tval_before, NULL);
    for (int thread_id = 0; thread_id < num_threads; thread_id++) {
        thread_data[thread_id].iters = iters;
        thread_data[thread_id].thread_id = thread_id;
        thread_data[thread_id].num_threads = num_threads;
        pthread_create(&threads[thread_id], NULL, run_n_iters, (void *)(&(thread_data[thread_id])));
    }
    for (int thread_id = 0; thread_id < num_threads; thread_id++){
        pthread_join(threads[thread_id], NULL);
    }
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);

    free(threads);
    free(thread_data);

    FILE* f = fopen("./results/res_pthreads.txt", "a");
    fprintf(f, "%d\t%ld.%06ld\n", num_threads, (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
}