#include "odesolver.hpp"
#include <limits>
#include <sys/time.h>

#define TIME_MEASURING 1

using namespace std;

double gfunc(double const x)
{
    return (1. + x * x);
}

double sfunc(double const x)
{
    return (-1.);
}

int main()
{
#if TIME_MEASURING
    // zeigt nach Durchlauf die Ausführdauer an
    struct timeval start, end;
    gettimeofday(&start, NULL);
#endif

    ODESolver dgl(-1., 1., 256, gfunc, sfunc);
    dgl.max_level = 2;
    unsigned long iteration = dgl.solve(1e-10, 4, 4);
    fprintf(stdout, "Level: %d, Iterationen: %lu, Residuum: %.5e\n", dgl.max_level, iteration, dgl.get_residual_norm());

#if TIME_MEASURING
    gettimeofday(&end, NULL);
    // Berechne die vergangene Zeit in sekungden
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Dauer: %.3f sekunden\n", elapsed_time);
#endif
}