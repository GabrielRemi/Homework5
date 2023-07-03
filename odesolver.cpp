#include "header/odesolver.hpp"

ODESolver::ODESolver(double xa, double xe, double ua, double ue, double h, func_type g, func_type s, double init_value)
{
    this->start_point = xa;
    this->end_point = xe;
    this->start_value = ua;
    this->end_value = ue;

    // initialisiere <x> und <u>
    set_stepwidth(h);

    x.resize(vector_size);
    for (unsigned long i = 0; i < vector_size; ++i)
        x[i] = xa + i * stepwidth;

    u.resize(vector_size);
    for (unsigned long i = 0; i < vector_size; ++i)
        u[i] = init_value;
    u[0] = start_value;
    u[vector_size - 1] = end_value;

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
    this->vector_size = ceil(xDelta / h) + 1; // Stellt sicher, dass die Schrittweite h0 <= h

    // Stelle sicher, dass vector_size-1 eine Potenz von 2 ist
    double exponent = log(vector_size - 1) / log(2);
    if (exponent != int(exponent))
        this->vector_size = pow(2, ceil(exponent)) + 1;

    this->stepwidth = xDelta / (vector_size - 1);
}

vector<double> ODESolver::residual(const vector<double> &u, unsigned int level)
{
    unsigned long base_size = vector_size - 2; // Größe des feinsten Gitters
    unsigned long size;
    if (level == 1)
        size = base_size;
    const double step_squared = stepwidth * stepwidth;
    vector<double> residual(size);
    for (unsigned long i = 0; i < vector_size - 2; ++i)
        residual[i] = 1 / step_squared * (u[i + 2] + u[i] - 2 * u[i + 1]) + gvec[i + 1] * u[i + 1] - svec[i + 1];

    return residual;
}

vector<double> ODESolver::restrict(const vector<double> &vec)
{
    // BESTIMME VEKTOR-GRÖẞE
    unsigned long base_size = vec.size();
    // unsigned long size = (base_size + 1) / pow(2, level) - 1;
    unsigned long size = (base_size - 1) / 2;

    // BERECHNE VEKTOR AUF GROBEN GITTER
    vector<double> restricted_vec(size);
    for (unsigned long i = 0; i < size; ++i)
        restricted_vec[i] = 0.25 * (vec[2 * i] + 2 * vec[2 * i + 1] + vec[2 * i + 2]);

    return restricted_vec;
}

vector<double> ODESolver::interpolate(const vector<double> &vec)
{
    // BESTIMME VEKTOR-GRÖẞE
    unsigned long base_size = vec.size();
    // unsigned long size = (base_size + 1) / pow(2, level - 1) - 1;
    unsigned long size = 2 * base_size + 1;

    // BERECHNE VEKTOR AUF FEINEREM GITTER
    vector<double> interpolated_vec(size);
    interpolated_vec[0] = 0.5 * vec[0];
    interpolated_vec[size - 1] = 0.5 * vec[base_size - 1];
    for (unsigned long i = 0; i < size; ++i)
    {
        if (i % 2 == 1)
            interpolated_vec[i] = vec[(i - 1) / 2];
        if (i % 2 == 0)
            interpolated_vec[i] = .5 * (vec[i / 2] + vec[i / 2 - 1]);
    }

    return interpolated_vec;
}

vector<double> ODESolver::tridig(const vector<double> &y, const double h)
{
    unsigned long y_size = y.size();
    vector<double> d(y_size);
    vector<double> c(y_size);
    double hh = h * h;

    // STARTWERTE
    d[0] = hh * y[0] / 2;
    c[0] = -0.5;

    for (unsigned long i = 1; i < y_size; ++i)
    {
        d[i] = (hh * y[i] + d[i - 1]) / (2 + c[i - 1]);
        c[i] = -1 / (2 + c[i - 1]);
    }

    vector<double> x(y_size);
    x[y_size - 1] = d[y_size - 1];
    for (int i = y_size - 2; i >= 0; --i)
        x[i] = d[i] - c[i] * x[i + 1];

    return x;
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

void ODESolver::gauss_seidel()
{
    double step_squared = stepwidth * stepwidth;
    for (unsigned long i = 1; i < vector_size - 1; ++i)
        u[i] = (1 - relaxation) * u[i] + relaxation * (u[i + 1] + u[i - 1] - step_squared * svec[i]) / (2 - step_squared * gvec[i]);

    // Berechne Residuum
    vector<double> res = residual(u, 1);
    residual_norm = misc::norm(res);
}

double ODESolver::functional()
{
    // BERECHNET ABLEITUNGSARRAY
    vector<double> u_derivative(vector_size);
    u_derivative[0] = 1 / stepwidth * (u[1] - u[0]);
    u_derivative[vector_size - 1] = 1 / stepwidth * (u[vector_size - 1] - u[vector_size - 2]);
    for (unsigned long i = 1; i < vector_size - 1; ++i)
        u_derivative[i] = 1 / (2 * stepwidth) * (u[i + 1] - u[i - 1]);

    // BERECHNET INTEGRANDEN
    vector<double> integrand(vector_size);
    for (unsigned long i = 0; i < vector_size; ++i)
        integrand[i] = 0.5 * u_derivative[i] * u_derivative[i] + svec[i] * u[i] - 0.5 * gvec[i] * u[i] * u[i];

    double functional = misc::trapez(integrand, stepwidth);

    return functional;
}

double ODESolver::twolevel_method(unsigned int pre_smooth, unsigned int post_smooth)
{
    /* In allen Schritten werden die Residuen und Fehler an den Randpunkten nicht beachtet, da diese immer 0 sind. Dies führt zu einer Verschiebung der Indizes im Vergleich
    zu den mathematischen Formeln*/
    // GLÄTTUNG
    for (unsigned long i = 0; i < pre_smooth; ++i)
        gauss_seidel();

    vector<double> residual_fine_grid = residual(u, 1);
    vector<double> residual_coarse_grid = restrict(residual_fine_grid);

    vector<double> error_coarse_grid = tridig(residual_coarse_grid, 2 * stepwidth);
    vector<double> error_fine_grid = interpolate(error_coarse_grid);

    // ADDIERE FEHLER ZU <u>
    for (unsigned long i = 1; i < vector_size - 1; ++i)
        u[i] += error_fine_grid[i - 1];

    // NACH-GLÄTTUNG
    for (unsigned long i = 0; i < post_smooth; ++i)
        gauss_seidel();

    residual_fine_grid = residual(u, 1);
    return misc::norm(residual_fine_grid);
}

unsigned long ODESolver::solve(double eps, iteration_type it)
{
    // SETZE DIE ITERATIONSMETHODE ÜBER LAMBDA-AUSDRÜCKE
    function<void()> iteration_function;
    if (it == iteration_type::jacobi)
        iteration_function = [this](void) -> void
        { jacobi(); };
    else if (it == iteration_type::gauss_seidel)
        iteration_function = [this](void) -> void
        { gauss_seidel(); };

    double current_eps = 2 * eps; // Anfangswerte muss größer als eps sein

    unsigned long iteration = 0;
    unsigned long iteration_max = 10000000;
    while (current_eps > eps && iteration < iteration_max)
    {
        ++iteration;
        iteration_function();
        current_eps = residual_norm;
        // printf("old functional: %.5e, new functional: %.5e, eps: %.5e", old_functional, new_functional, current_eps);
    }

    return iteration;
}

unsigned long ODESolver::solve(double eps, unsigned int pre_smooth, unsigned int post_smooth)
{
    unsigned int iteration = 0;
    const unsigned int iteration_max = 5000;

    double eps_iter = 2 * eps; // willkürlicher Wert, hauptsache größer eps

    while(eps_iter > eps && iteration < iteration_max)
    {
        eps_iter = twolevel_method(pre_smooth, post_smooth);
        iteration += pre_smooth + post_smooth;
    }

    if (iteration == iteration_max)
        fprintf(stderr, "Warnung: Konvergenz nicht eingetrofen\n");

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