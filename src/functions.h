// #include <math.h>

typedef struct {
    double start;
    double real_root;
    double (*func)(double);
} Equation;

typedef double (*Function)(double);


double func_1(double x);
double func_2(double x);

Equation get_equation(int);
