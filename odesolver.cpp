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
    if (g == nullptr)
        g = [](double x)
        { return 0; };
    if (s == nullptr)
        s = [](double x)
        { return 0; };

    this->g = g;
    this->s = s;

    gvec = misc::assignValues(x, g);
    svec = misc::assignValues(x, s);
}

void ODESolver::set_stepwidth(unsigned long n)
{
    double xDelta = (double)end_point - start_point;

    // Kontrolliert, ob n eine Potenz von 2 ist
    double exponent = log(n) / log(2);
    if (exponent != int(exponent))
        this->n = pow(2, ceil(exponent));
    else
        this->n = n;
    vector_size = this->n - 1;

    this->stepwidth = xDelta / this->n;
}

vector<double> ODESolver::get_x(bool boundaries)
{
    if (boundaries)
    {
        vector<double> out(n + 1);
        out[0] = start_point;
        out[n] = end_point;
        for (unsigned long i = 1; i < n; ++i)
            out[i] = x[i - 1];

        return out;
    }
    return x;
}

vector<double> ODESolver::get_u(bool boundaries)
{
    if (boundaries)
    {
        vector<double> out(n + 1);
        out[0] = 0;
        out[n] = 0;
        for (unsigned long i = 1; i < n; ++i)
            out[i] = u[i - 1];

        return out;
    }
    return u;
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

vector<double> ODESolver::thomas_algorithm(const vector<double> &y, const vector<double> &gvec, const double h)
{
    /*Löst die Gleichung A*u = f, wobei auf die Werte der Hauptdiagonalen 1/h^2(2 - h^2 g_i) sind und -1 auf den Nebendiagonalen*/
    unsigned long y_size = y.size();
    vector<double> d(y_size);
    vector<double> c(y_size);
    double hh = h * h;

    // STARTWERTE
    d[0] = hh * y[0] / (2 - hh * gvec[0]);
    c[0] = 1 / (hh * gvec[0] - 2);

    for (unsigned long i = 1; i < y_size; ++i)
    {
        d[i] = (hh * y[i] + d[i - 1]) / (2 - hh * gvec[i] + c[i - 1]);
        c[i] = -1 / (2 - hh * gvec[i] + c[i - 1]);
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

void ODESolver::gauss_seidel()
{
    gauss_seidel(u, gvec, svec, 1);
}

void ODESolver::twolevel_method(vector<double> &u, vector<double> &gvec, vector<double> &svec, unsigned int pre_smooth, unsigned int post_smooth, unsigned int level)
{
    // BENUTZE AUF GRÖBSTEN GITTER DEN THOMAS-ALGORITHMUS
    if (thomas == true)
    {
        // falls nur ein level, führe nur eine gauss-seidel iteration durch
        if (max_level == 1)
        {
            gauss_seidel();
            return;
        }

        vector<double> error_fine_grid;
        if (level < max_level)
        {
            // VOR-GLÄTTUNG
            for (unsigned int i = 0; i < pre_smooth; ++i)
                gauss_seidel(u, gvec, svec, level);

            // RESIDUENVEKTOREN UND FEHLER
            vector<double> residual_fine_grid = residual(gvec, svec, u, level);
            vector<double> residual_coarse_grid = restrict(residual_fine_grid);

            unsigned long coarse_grid_size = residual_coarse_grid.size();

            // BERECHNE FEHLER AUF NIEDRIGSTEM LEVEL
            vector<double> error_coarse_grid(coarse_grid_size);

            vector<double> gvec_coarse_grid(coarse_grid_size);
            vector<double> svec_coarse_grid(coarse_grid_size);
            for (unsigned long i = 0; i < coarse_grid_size; ++i)
            {
                svec_coarse_grid[i] = -residual_coarse_grid[i];
                gvec_coarse_grid[i] = gvec[2 * i + 1];
            }

            // REKURSION DER ZWEIGITTER-VERFAHRENS
            twolevel_method(error_coarse_grid, gvec_coarse_grid, svec_coarse_grid, pre_smooth, post_smooth, level + 1);
            error_fine_grid = interpolate(error_coarse_grid);

            // ADDIERE FEHLER ZUR NÄHERUNG
            for (unsigned long i = 0; i < u.size(); ++i)
                u[i] += error_fine_grid[i];

            // NACH-GLÄTTUNG
            for (unsigned int i = 0; i < post_smooth; ++i)
                gauss_seidel(u, gvec, svec, level);
        }
        else
        {
            // BERECHNE AUF GRÖBSTEN GITTER DEN FEHLER EXAKT
            double h = pow(2, level - 1) * stepwidth;
            error_fine_grid = thomas_algorithm(svec, gvec, h);

            // ADDIERE FEHLER ZUR NÄHERUNG
            for (unsigned long i = 0; i < u.size(); ++i)
                u[i] += error_fine_grid[i];
        }
    }
    // MULTIGRID OHNE THOMAS-ALGORITHMUS
    else 
    {
        // GLÄTTUNG
        for (unsigned int i = 0; i < pre_smooth; ++i)
            gauss_seidel(u, gvec, svec, level);

        if (level < max_level)
        {
            vector<double> residual_fine_grid = residual(gvec, svec, u, level);
            vector<double> residual_coarse_grid = restrict(residual_fine_grid);

            unsigned long fine_grid_size = residual_fine_grid.size();
            unsigned long coarse_grid_size = residual_coarse_grid.size();

            vector<double> error_coarse_grid(coarse_grid_size);

            vector<double> gvec_coarse_grid(coarse_grid_size);
            vector<double> svec_coarse_grid(coarse_grid_size);
            for (unsigned int i = 0; i < coarse_grid_size; ++i)
            {
                svec_coarse_grid[i] = -residual_coarse_grid[i];
                gvec_coarse_grid[i] = gvec[2 * i + 1];
            }

            // Berechne Fehler auf nächstem gitter
            twolevel_method(error_coarse_grid, gvec_coarse_grid, svec_coarse_grid, pre_smooth, post_smooth, level + 1);
            vector<double> error_fine_grid = interpolate(error_coarse_grid);

            // ADDIERE FEHLER ZU <u>
            for (unsigned long i = 0; i < fine_grid_size; ++i)
                u[i] += error_fine_grid[i];
        }

        // NACH-GLÄTTUNG
        for (unsigned int i = 0; i < post_smooth; ++i)
            gauss_seidel(u, gvec, svec, level);
        }
}

unsigned long ODESolver::solve(double eps, vector<double> *u_norms, vector<double> *res_norms, unsigned int pre_smooth, unsigned int post_smooth)
{
    if (u_norms != nullptr)
        (*u_norms).resize(0);
    if (res_norms != nullptr)
        (*res_norms).resize(0);

    unsigned int iteration = 0;
    const unsigned int iteration_max = 150000;


    double eps_iter = 2 * eps; // willkürlicher Wert, hauptsache größer eps

    while (eps_iter > eps && iteration < iteration_max)
    {
        twolevel_method(u, gvec, svec, pre_smooth, post_smooth, 1);

        eps_iter = get_residual_norm();
       
        ++iteration;

        if (u_norms != nullptr)
            (*u_norms).push_back(misc::norm(u));
        if (res_norms != nullptr)
            (*res_norms).push_back(eps_iter);
    }

    if (iteration == iteration_max)
        fprintf(stderr, "Warnung: Konvergenz nicht eingetrofen\n");

    if (u_norms != nullptr)
        (*u_norms).shrink_to_fit();
    if (res_norms != nullptr)
        (*res_norms).shrink_to_fit();

    return iteration;
}


void ODESolver::set_u(vector<double> src)
{
    if (src.size() == vector_size)
        this->u = src;
    else
        fprintf(stderr, "Warnung set_u(): u konnte nicht gesetzt werden, da die vektorlänge nicht übereinstimmt\n");
}
