#include "odesolver.hpp"
#include <limits>
#include <sys/time.h>

#define TIME_MEASURING 1

using namespace std;

double analytic(double x)
{
    return 0.125 * exp(-x) * (x + x * x);
}

int main()
{
#if TIME_MEASURING
    // zeigt nach Durchlauf die Ausführdauer an
    struct timeval start, end;
    gettimeofday(&start, NULL);
#endif

    auto g = [](double x) -> double
    { return 1 + x * x; };
    auto s = [](double x) -> double
    { return -1; };
    // auto g = [](double x) -> double
    // { return -1; };
    // auto s = [](double x) -> double
    // { return -0.5*x*exp(-x); };

    ODESolver dgl(-1, 1, .02, g, s);
    dgl.relaxation = 1;
    ODESolver twogrid = dgl;
    ODESolver multigrid = dgl;
    ODESolver multigrid2 = dgl;
    multigrid.max_level = 4;
    multigrid2.max_level = 6;

    double eps = 1e-10;
    unsigned int pre_smooth = 4;
    unsigned int post_smooth = 4;
    cout << "n = " << dgl.get_vector_size() + 1 << endl;
    cout << "Iterationen ohne Gitter: " << dgl.solve(eps) << " " << dgl.get_residual_norm() << endl;
    cout << "Iterationen mit 2 Gitter: " << twogrid.solve(eps, pre_smooth, post_smooth) << " " << twogrid.get_residual_norm() << endl;
    cout << "Iterationen mit 4 Gitter: " << multigrid.solve(eps, pre_smooth, post_smooth) << " " << multigrid.get_residual_norm() << endl;
    cout << "Iterationen mit 6 Gitter: " << multigrid2.solve(eps, pre_smooth, post_smooth) << " " << multigrid2.get_residual_norm() << endl;

    vector<double> x = dgl.get_x();
    vector<double> u1 = dgl.get_u();
    vector<double> u2 = multigrid.get_u();
    misc::printInFile("test.txt", 3, &x, &u1, &u2);

#if TIME_MEASURING
    gettimeofday(&end, NULL);
    // Berechne die vergangene Zeit in sekungden
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Dauer: %.3f sekunden\n", elapsed_time);
#endif
}