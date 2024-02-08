#include <math.h>
#include <stdlib.h>
#include "functions.h"

// x = x^3 + 1
double func_1(double x){
    return cbrt(x - 1);
}

// x^2 - exp(-x) = 0
double func_2(double x){
    return exp(-1 * x / 2);
}

// arcsin(2x + 1) - x^2 = 0
double func_3(double x){
    return 0.5 * (sin(x*x) - 1);
}

// x^3 + 5x - 1
double func_4(double x){
    return (1 - pow(x, 3)) * 0.2;
}

// x^2 - 5 = 0
double func_5(double x){
    return (x*x + 5) / (2*x);
}
double func(double x){
    return 0;
}


double real_roots[] = {-1.325, 0.7030, -0.415, 0.1984, 2.2361};
double starts[] =     {-2.000, 1.0000, -0.000, 0.0000, 5.0000};
Function funcs[] =    {func_1, func_2, func_3, func_4, func_5};

Equation get_equation(int rank){

    Equation eq = {
                    .func = funcs[rank],
                    .real_root = real_roots[rank],
                    .start = starts[rank]
        };
    return eq;
}


