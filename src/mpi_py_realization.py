from mpi4py import MPI
import random
import math
import time
import sys

PRINT_INFO = 0

def find_root(root_of):
    real_root = math.sqrt(root_of)
    x_next = root_of
    iterations = 0
    eps = 1e-6
    x_prev = x_next + 1
    while (abs(x_prev - x_next) > eps):
        x_prev = x_next
        x_next = ((x_prev * x_prev) + root_of) / (x_prev * 2)
        iterations += 1

    is_close = abs(real_root - x_next) < eps
    if PRINT_INFO:
        print(f"iters: {iterations}, root of: {root_of:.6f}, root: {x_next:.6f}, real_root: {real_root:.6f}, is_close: {is_close}")

    return is_close


if __name__ == "__main__":
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()
    iters = -1
    if (rank == 0):
        iters = int(sys.argv[1])
        iters = int(iters / size) + 1
    
    iters = comm.bcast(iters, 0)
    start = time.time()

    range_ = int(100 / size)
    start_from = range_ * rank
    delta = range_ / iters

    for iter_ in range(0, iters):
        root_of = (iter_ + 1) * delta + start_from
        find_root(root_of)

    stop = time.time()
    if rank == 0:
        with open("results/res_mpi_py.txt", "a") as f:
            txt = f"{size}\t{stop - start:.2f}\n"
            f.write(txt)
    