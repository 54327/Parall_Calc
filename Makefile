SHELL := /bin/bash
root := ./src
iterations := 1000000
max_proc := 28
results := ./results


mpi_c: 
	mpicc -Wall $(root)/mpi_c_realization.c  -lm -o $(root)/out_mpi_c
	proc=1 ; while [[ $$proc -le $(max_proc) ]] ; do \
		mpirun -np $$proc $(root)/out_mpi_c $(iterations); \
		((proc = proc + 2)) ; \
	done


mpi_py:
	proc=1 ; while [[ $$proc -le $(max_proc) ]] ; do \
		mpiexec -n $$proc python3.10 $(root)/mpi_py_realization.py $(iterations); \
		((proc = proc + 2)) ; \
	done


pthread: 
	gcc -Wall -lpthread $(root)/pthread_realization.c -lm -o $(root)/out_pthread
	threads=1 ; while [[ $$threads -le $(max_proc) ]] ; do \
		$(root)/out_pthread $$threads $(iterations); \
		((threads = threads + 2)) ; \
	done


omp: 
	gcc -Wall -fopenmp $(root)/omp_realization.c -lm -o $(root)/out_omp
	threads=1 ; while [[ $$threads -le $(max_proc) ]] ; do \
		$(root)/out_omp $$threads $(iterations); \
		((threads = threads + 2)) ; \
	done
	

clear:
	rm $(root)/functions.o $(root)/out_mpi_c $(root)/out_omp $(root)/out_pthread $(results)/*
