#include "odesolver.hpp"
#include <limits>

using namespace std;

double analytic(double x)
{
    return 0.125 * exp(-x) * (x + x * x);
}

int main()
{

    auto g = [](double x) -> double
    { return 1 + x * x; };
    auto s = [](double x) -> double
    { return -1; };
    auto nullfunc = [](double x) -> double
    { return 0; };

    ODESolver harmonic(-1, 1, 0, 0, .01, g, s);
    printf("n = %lu\n", harmonic.get_vector_size() - 1);
    harmonic.relaxation = 1.9;
    ODESolver harmonic2 = harmonic;
    unsigned int it = harmonic.solve(1e-10, 4, 4);
    unsigned int it2 = harmonic2.solve(1e-10, iteration_type::gauss_seidel);

    
    cout << "Residuum ohne zwei Gitter: " << harmonic2.get_residual_norm() << "  Iterationen: " << it2 << endl;
    cout << "Residuum mit zwei Gitter: " << harmonic.get_residual_norm() << "  Iterationen: " << it << endl;

    vector<double> x = harmonic.get_x();

    vector<double> uwithg = harmonic.get_u();
    vector<double> uwithoutg = harmonic2.get_u();
    misc::printInFile("test.txt", 3, &x, &uwithg, &uwithoutg);
}