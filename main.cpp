#include "odesolver.hpp"
#include <limits>
#include <sys/time.h>

#define TIME_MEASURING 1

using namespace std;

double g(double const x)
{
    return -1;
}

double s(double const x)
{
    return -0.5 * x * exp(-x);
}

double analytic(double x)
{
    return 0.125 * x * (1 + x) * exp(-x);
}

int main()
{
#if TIME_MEASURING
    // zeigt nach Durchlauf die Ausführdauer an
    struct timeval start, end;
    gettimeofday(&start, NULL);
#endif

    ODESolver dgl(0, 20., 1000, g, s);
    dgl.max_level = 5;
    unsigned long iteration = dgl.solve(1e-10, 4, 4);
    fprintf(stdout, "Level: %d Gitter %lu, Iterationen: %lu, Residuum: %.5e\n", dgl.max_level, dgl.get_vector_size() + 1, iteration, dgl.get_residual_norm());
    dgl.printInFile("test.txt", analytic);

#if TIME_MEASURING
    gettimeofday(&end, NULL);
    // Berechne die vergangene Zeit in sekungden
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Dauer: %.3f sekunden\n", elapsed_time);
#endif
}