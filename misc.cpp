#include "misc.hpp"

void misc::printInFile(std::string fileName, unsigned int n, ...)
{
    std::fstream file;
    file.open(fileName, std::ios_base::out);

    va_list args;
    va_start(args, n);

    // Pointer, auf das vektor array der Argumente
    std::vector<double> **arrayp = new vector<double> *[n * sizeof(double *)];

    for (unsigned int arg = 0; arg < n; ++arg)
    {
        arrayp[arg] = va_arg(args, std::vector<double> *);
    }

    // speichert alle Vektoren nebeneinander in einer Datei
    file.precision(PRECISION);
    for (unsigned int index = 0; index < arrayp[0]->size(); ++index)
    {
        file << std::scientific;
        for (unsigned int arg = 0; arg < n; ++arg)
        {
            file << arrayp[arg][0][index] << "  ";
        }
        file << std::endl;
    }

    va_end(args);
    file.close();
    delete[] arrayp;
}

std::vector<double> misc::assignValues(const std::vector<double> &values, func_type func)
{
    unsigned int size = values.size();
    std::vector<double> result(size);

    for (unsigned int i = 0; i < values.size(); ++i)
        result[i] = func(values[i]);

    return result;
}

vector<double> misc::equidistantValues(const double x0, const double xmax, double &h)
{
    unsigned int n = ceil((xmax - x0) / h) + 1;
    h = (xmax - x0) / (n - 1); // Passe Schrittweite an, damit out[n-1] = xmax
    vector<double> out(n);

    for (unsigned int i = 0; i < n; ++i)
        out[i] = x0 + i * h;

    return out;
}

double misc::norm(const vector<double> &y)
{
    double norm = 0;
    for (double elem : y)
        norm += elem * elem;
    norm = sqrt(norm);

    return norm;
}

