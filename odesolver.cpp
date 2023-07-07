#include "header/odesolver.hpp"

ODESolver::ODESolver(double xa, double xe, unsigned long n, func_type g, func_type s, double init_value)
{
    this->start_point = xa;
    this->end_point = xe;

    // initialisiere <x> und <u>
    set_stepwidth(n);

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

void ODESolver::set_stepwidth(unsigned long n)
{
    double xDelta = (double)end_point - start_point;

    // Kontrolliert, ob n eine Potenz von 2 ist
    double exponent = log(n) / log(2);
    if (exponent != int(exponent))
        n = pow(2, ceil(exponent));
    vector_size = n - 1;

    this->stepwidth = xDelta / n;
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

vector<double> ODESolver::residual(const vector<double> &gvec, const vector<double> &svec, const vector<double> &v, unsigned int level)
{
    // Breche das Programm ab, wenn f ungleiche viele Elemente wie v hat
    unsigned long size = v.size();
    if (!(gvec.size() == size && svec.size() == size))
    {
        fprintf(stderr, "ODESolver::residual: vektoren besitzen unterschiedlich viele Elemente\n");
        exit(-1);
    }
    vector<double> residual(size);
    double h = pow(2, level - 1) * stepwidth; // Die Schritweite in der Matrix A hängt vom level ab
    double hh = h * h;

    residual[0] = (-2 * v[0] + v[1]) / hh + gvec[0] * v[0] - svec[0];
    residual[size - 1] = (v[size - 2] - 2 * v[size - 1]) / hh + gvec[size - 1] * v[size - 1] - svec[size - 1];
    for (unsigned long i = 1; i < size - 1; ++i)
        residual[i] = (v[i - 1] - 2 * v[i] + v[i + 1]) / hh + gvec[i] * v[i] - svec[i];

    return residual;
}

double ODESolver::get_residual_norm()
{
    vector<double> res = residual(this->gvec, this->svec, this->u, 1);

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

void ODESolver::gauss_seidel(vector<double> &u, vector<double> &gvec, vector<double> &svec, unsigned int level)
{
    unsigned long size = u.size();
    if (!(size == svec.size() && size == gvec.size()))
    {
        fprintf(stderr, "Fehler gauss_seidel: vektoren müssen gleich viele Elemente besitzen\n");
        exit(-1);
    }
    double h = pow(2, level - 1) * stepwidth; // Die Schritweite in der Matrix A hängt vom level ab
    double step_squared = h * h;

    u[0] = (1 - relaxation) * u[0] + relaxation * (u[1] - step_squared * svec[0]) / (2 - step_squared * gvec[0]);
    for (unsigned long i = 1; i < size - 1; ++i)
        u[i] = (1. - relaxation) * u[i] + relaxation * (u[i + 1] + u[i - 1] - step_squared * svec[i]) / (2. - step_squared * gvec[i]);

    u[size - 1] = (1 - relaxation) * u[size - 1] + relaxation * (u[size - 2] - step_squared * svec[size - 1]) / (2 - step_squared * gvec[size - 1]);
}

void ODESolver::twolevel_method(vector<double> &u, vector<double> &gvec, vector<double> &svec, unsigned int pre_smooth, unsigned int post_smooth, unsigned int level)
{
    /* In allen Schritten werden die Residuen und Fehler an den Randpunkten nicht beachtet, da diese immer 0 sind. Dies führt zu einer Verschiebung der Indizes im Vergleich
    zu den mathematischen Formeln*/
    // GLÄTTUNG
    for (unsigned int i = 0; i < pre_smooth; ++i)
        gauss_seidel(u, gvec, svec, level);

    if (level < max_level)
    {
        vector<double> residual_fine_grid = residual(gvec, svec, u, level);


        vector<double> residual_coarse_grid = restrict(residual_fine_grid);
        
        unsigned long fine_grid_size = residual_fine_grid.size();
        unsigned long coarse_grid_size = residual_coarse_grid.size();

        // BERECHNE FEHLER AUF NIEDRIGSTEM LEVEL
        vector<double> error_coarse_grid(coarse_grid_size);

        vector<double> gvec(coarse_grid_size);
        vector<double> svec(coarse_grid_size);
        for (unsigned long i = 0; i < coarse_grid_size; ++i)
        {
            svec[i] = -residual_coarse_grid[i];
            gvec[i] = 0;
        }
        twolevel_method(error_coarse_grid, gvec, svec, pre_smooth, post_smooth, level + 1);
        vector<double> error_fine_grid = interpolate(error_coarse_grid);
        // error_fine_grid = thomas_algorithm(residual_fine_grid, stepwidth);

        // ADDIERE FEHLER ZU <u>
        for (unsigned long i = 0; i < fine_grid_size; ++i)
            u[i] += error_fine_grid[i];
    }

    // NACH-GLÄTTUNG
    for (unsigned int i = 0; i < post_smooth; ++i)
        gauss_seidel(u, gvec, svec, level);
}

unsigned long ODESolver::solve(double eps)
{
    double current_eps = 2 * eps; // Anfangswerte muss größer als eps sein

    unsigned long iteration = 0;
    unsigned long iteration_max = 10000000;
    while (current_eps > eps && iteration < iteration_max)
    {
        ++iteration;
        gauss_seidel(u, gvec, svec, 1);

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
        twolevel_method(u, gvec, svec, pre_smooth, post_smooth, 1);
        eps_iter = get_residual_norm();
        // iteration += pre_smooth + post_smooth;
        ++iteration;
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