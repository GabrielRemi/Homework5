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
    { return -1; };
    auto s = [](double x) -> double
    { return -0.5 * x * exp(-x); };
    auto nullfunc = [](double x) -> double
    { return 0; };

    ODESolver harmonic(0, 10, 0, 0, 0.1, g, s);
    cout << harmonic.solve(1e-8) << endl;
    harmonic.printInFile("test.txt", analytic);
}