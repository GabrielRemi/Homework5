#include "header/odesolver.hpp"

ODESolver::ODESolver(double xa, double xe, double ua, double ue, double h, func_type g, func_type s, double init_value)
{
    this->start_point = xa;
    this->end_point = xe;
    this->start_value = ua;
    this->end_value = ue;

    // initialisiere <x> und <u>
    this->stepwidth = h;

    x = misc::equidistantValues(start_point, end_point, stepwidth);
    vector_size = x.size();
    u.resize(vector_size);
    for (unsigned long i = 0; i < vector_size; ++i)
        u[i] = init_value;
    u[0] = start_value;

    // INITIALISIERE <g> und <s>
    // Falls eine Funktion ein nullptr ist, soll die Funktion den Wert 0 wiedergeben
    auto nullfunc = [](double x) -> double
    { return 0; };
    if (g != nullptr)
        this->g = g;
    else
        this->g = nullfunc;
    if (s != nullptr)
        this->s = s;
    else
        this->s = nullfunc;
    gvec = misc::assignValues(x, g);
    svec = misc::assignValues(x, s);
}

void ODESolver::set_stepwidth(double h)
{
    // setze eine angemessene Schrittweite, sodass x[n-1] = end_point
    double xDelta = end_point - start_point;
    vector_size = ceil(xDelta / h) + 1; // Stellt sicher, dass die Schrittweite h0 <= h
    stepwidth = xDelta / (vector_size - 1);
}

void ODESolver::jacobi()
{
    vector<double> new_u(vector_size);
    new_u[0] = u[0];
    new_u[vector_size - 1] = u[vector_size - 1];
    double step_squared = stepwidth * stepwidth;

    for (unsigned long i = 1; i < vector_size - 1; ++i)
        new_u[i] = (u[i + 1] + u[i - 1] - step_squared * svec[i]) / (2 - step_squared * gvec[i]);

    u = new_u;
}

double ODESolver::functional()
{
    // BERECHNET ABLEITUNGSARRAY
    vector<double> u_derivative(vector_size);
    u_derivative[0] = 1 / stepwidth * (u[1] - u[0]);
    u_derivative[vector_size - 1] = 1 / stepwidth * (u[vector_size - 1] - u[vector_size - 2]);
    for (int i = 1; i < vector_size - 1; ++i)
        u_derivative[i] = 1 / (2 * stepwidth) * (u[i + 1] - u[i - 1]);

    // BERECHNET INTEGRANDEN
    vector<double> integrand(vector_size);
    for (int i = 0; i < vector_size; ++i)
        integrand[i] = 0.5 * u_derivative[i] * u_derivative[i] + svec[i] * u[i] - 0.5 * gvec[i] * u[i] * u[i];

    double functional = misc::trapez(integrand, stepwidth);

    return functional;
}

unsigned long ODESolver::solve(double eps)
{
    double current_eps = 2 * eps; // Anfangswerte muss größer als eps sein
    double old_functional;
    double new_functional;

    jacobi();
    new_functional = functional();

    unsigned long iteration = 0;
    unsigned long iteration_max = 100000;
    while (current_eps > eps && iteration < iteration_max)
    {
        ++iteration;

        jacobi();
        old_functional = new_functional;
        new_functional = functional();

        current_eps = fabs((new_functional - old_functional) / (new_functional + old_functional));
        // printf("old functional: %.5e, new functional: %.5e, eps: %.5e", old_functional, new_functional, current_eps);
    }

    return iteration;
}

void ODESolver::printInFile(string fileName)
{
    misc::printInFile(fileName, 2, &x, &u);
}

void ODESolver::printInFile(string fileName, func_type f)
{
    vector<double> analytic = misc::assignValuesMultithread(x, f);

    misc::printInFile(fileName, 3, &x, &u, &analytic);
}