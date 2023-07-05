#include "header/odesolver.hpp"

ODESolver::ODESolver(double xa, double xe, double h, func_type g, func_type s, double init_value)
{
    this->start_point = xa;
    this->end_point = xe;

    // initialisiere <x> und <u>
    set_stepwidth(h);

    x.resize(vector_size);
    for (unsigned long i = 0; i < vector_size; ++i)
        x[i] = xa + (i + 1) * stepwidth;

    u.resize(vector_size);
    for (unsigned long i = 0; i < vector_size; ++i)
    {
        u[i] = init_value;
    }

    // INITIALISIERE <g> und <s>
    this->g = g;
    this->s = s;

    gvec = misc::assignValues(x, g);
    svec = misc::assignValues(x, s);
    fvec.resize(vector_size);
}

void ODESolver::set_stepwidth(double h)
{
    // setze eine angemessene Schrittweite, sodass x[n-1] = end_point
    double xDelta = end_point - start_point;
    n = ceil(xDelta / h) + 1; // Stellt sicher, dass die Schrittweite h0 <= h

    // Stelle sicher, dass vector_size-1 eine Potenz von 2 ist
    double exponent = log(n - 1) / log(2);
    if (exponent != int(exponent))
        n = pow(2, ceil(exponent)) + 1;
    vector_size = n - 2;

    stepwidth = xDelta / (n - 1);
}

vector<double> ODESolver::get_x()
{
    vector<double> out(n);
    for (unsigned long i = 0; i < vector_size; ++i)
        out[i + 1] = x[i];
    out[0] = start_point;
    out[n - 1] = end_point;

    return out;
}

vector<double> ODESolver::get_u()
{
    vector<double> out(n);
    for (unsigned long i = 0; i < vector_size; ++i)
        out[i + 1] = u[i];
    out[0] = 0;
    out[n - 1] = 0;

    return out;
}

void ODESolver::calc_fvec(void)
{
    for (unsigned long i = 0; i < vector_size; ++i)
        fvec[i] = gvec[i] * u[i] - svec[i];
}

vector<double> ODESolver::residual()
{
    unsigned long size = u.size();

    const double step_squared = stepwidth * stepwidth;
    vector<double> residual(size);

    residual[0] = (-2 * u[0] + u[1]) / step_squared + gvec[0] * u[0] - svec[0];
    residual[size - 1] = (u[size - 2] - 2 * u[size - 1]) / step_squared + gvec[size - 1] * u[size - 1] - svec[size - 1];
    for (unsigned long i = 1; i < size - 1; ++i)
        residual[i] = (u[i - 1] - 2 * u[i] + u[i + 1]) / step_squared + gvec[i] * u[i] - svec[i];

    return residual;
}

vector<double> ODESolver::residual(const vector<double> &f, const vector<double> &v, unsigned int level)
{
    // Breche das Programm ab, wenn f ungleiche viele Elemente wie v hat
    unsigned long size = v.size();
    if (f.size() != size)
    {
        fprintf(stderr, "ODESolver::residual: vektoren besitzen unterschiedlich viele Elemente\n");
        exit(-1);
    }
    vector<double> residual(size);
    double h = pow(2, level - 1) * stepwidth; // Die Schritweite in der Matrix A hängt vom level ab
    double hh = h * h;

    residual[0] = (-2 * v[0] + v[1]) / hh + f[0];
    residual[size - 1] = (v[size - 2] - 2 * v[size - 1]) / hh + f[size - 1];
    for (unsigned long i = 1; i < size - 1; ++i)
        residual[i] = (v[i - 1] - 2 * v[i] + v[i + 1]) / hh + f[i];

    return residual;
}

double ODESolver::get_residual_norm()
{
    calc_fvec();
    vector<double> res = residual();

    return misc::norm(res);
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

vector<double> ODESolver::thomas_algorithm(const vector<double> &y, const double h)
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

/*Glätter nach dem Gauß-Seidel Verfahren für die Vektorgleichung A*x = f für den Fall, dass f nicht von x abhängt und schrittweite <h>.
Wird im Multigrid-Verfahren verwendet, um den Fehler auf einem Gitter zu bestimmen*/
void ODESolver::smoother(vector<double> &x, const vector<double> &f, const unsigned int level)
{
    unsigned long size = x.size();
    double h = pow(2, level - 1) * stepwidth; // Die Schritweite in der Matrix A hängt vom level ab
    double hh = h * h;

    x[0] = (1 - relaxation) * x[0] + relaxation * 0.5 * (x[1] + hh * f[0]);
    x[size - 1] = (1 - relaxation) * x[size - 1] + relaxation * 0.5 * (x[size - 2] + hh * f[size - 1]);
    for (unsigned long i = 1; i < size - 1; ++i)
        x[i] = (1 - relaxation) * x[i] + relaxation * 0.5 * (x[i - 1] + x[i + 1] + hh * f[i]);
}

void ODESolver::gauss_seidel()
{
    double step_squared = stepwidth * stepwidth;

    u[0] = (1 - relaxation) * u[0] + relaxation * (u[1] - step_squared * svec[0]) / (2 - step_squared * gvec[0]);
    u[vector_size - 1] = (1 - relaxation) * u[vector_size - 1] + relaxation * (u[vector_size - 2] - step_squared * svec[vector_size - 1]) / (2 - step_squared * gvec[vector_size - 1]);

    for (unsigned long i = 1; i < vector_size - 1; ++i)
        u[i] = (1 - relaxation) * u[i] + relaxation * (u[i + 1] + u[i - 1] - step_squared * svec[i]) / (2 - step_squared * gvec[i]);
}

double ODESolver::twolevel_method(vector<double> &u, vector<double> &f, unsigned int pre_smooth, unsigned int post_smooth, unsigned int level)
{
    /* In allen Schritten werden die Residuen und Fehler an den Randpunkten nicht beachtet, da diese immer 0 sind. Dies führt zu einer Verschiebung der Indizes im Vergleich
    zu den mathematischen Formeln*/
    // GLÄTTUNG
    for (unsigned int i = 0; i < pre_smooth; ++i)
    {
        // Da auf level 1 f von u abhängt, muss dieses nach jeder Glättoperation angepasst werden
        if (level == 1)
            calc_fvec();
        smoother(u, f, level);
    }

    vector<double> residual_fine_grid = residual(f, u, level);
    vector<double> residual_coarse_grid = restrict(residual_fine_grid);
    unsigned long coarse_grid_size = residual_coarse_grid.size();

    // BERECHNE FEHLER AUF NIEDRIGSTEM LEVEL
    vector<double> error_coarse_grid(coarse_grid_size);
    if (level + 1 == max_level)
    {
        error_coarse_grid = thomas_algorithm(residual_coarse_grid, 2 * stepwidth);
    }
    else
    {
        vector<double> f(coarse_grid_size);
        for (unsigned long i = 0; i < coarse_grid_size; ++i)
            f[i] = residual_coarse_grid[i];
        twolevel_method(error_coarse_grid, f, pre_smooth, post_smooth, level + 1);
    }
        
    vector<double> error_fine_grid = interpolate(error_coarse_grid);
    // error_fine_grid = thomas_algorithm(residual_fine_grid, stepwidth);

    // ADDIERE FEHLER ZU <u>
    for (unsigned long i = 0; i < coarse_grid_size; ++i)
        u[i] += error_fine_grid[i];

    // NACH-GLÄTTUNG
    for (unsigned int i = 0; i < post_smooth; ++i)
    {
        if (level == 1)
            calc_fvec();
        smoother(u, f, level);
    }

    if (level == 1)
        return get_residual_norm();
    else return 0;
}

double ODESolver::vcycle(unsigned int pre_smooth, unsigned int post_smooth, unsigned int maxlevel)
{
    // Glättung von <u>
    for (unsigned int i = 0; i < pre_smooth; ++i)
        gauss_seidel();

    vector<double> residual_fine_grid = residual();

    return 0;
}

unsigned long ODESolver::solve(double eps)
{
    double current_eps = 2 * eps; // Anfangswerte muss größer als eps sein

    unsigned long iteration = 0;
    unsigned long iteration_max = 10000000;
    while (current_eps > eps && iteration < iteration_max)
    {
        ++iteration;
        calc_fvec();
        smoother(u, fvec, 1);

        current_eps = get_residual_norm();
        // printf("old functional: %.5e, new functional: %.5e, eps: %.5e", old_functional, new_functional, current_eps);
    }

    return iteration;
}

unsigned long ODESolver::solve(double eps, unsigned int pre_smooth, unsigned int post_smooth)
{
    unsigned int iteration = 0;
    const unsigned int iteration_max = 10000000;

    double eps_iter = 2 * eps; // willkürlicher Wert, hauptsache größer eps

    while (eps_iter > eps && iteration < iteration_max)
    {
        eps_iter = twolevel_method(u, fvec, pre_smooth, post_smooth, 1);
        iteration += pre_smooth + post_smooth;
    }

    if (iteration == iteration_max)
        fprintf(stderr, "Warnung: Konvergenz nicht eingetrofen\n");

    return iteration;
}

void ODESolver::printInFile(string fileName)
{
    vector<double> x = get_x();
    vector<double> u = get_u();
    misc::printInFile(fileName, 2, &x, &u);
}

void ODESolver::printInFile(string fileName, func_type f)
{
    vector<double> x = get_x();
    vector<double> u = get_u();
    vector<double> analytic = misc::assignValuesMultithread(x, f);

    misc::printInFile(fileName, 3, &x, &u, &analytic);
}