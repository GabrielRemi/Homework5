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

vector<double> operator*(const double alpha, const vector<double> &v)
{
    vector<double> out;
    for (double elem : v)
        out.push_back(alpha * elem);

    return out;
}

vector<double> operator*(const vector<double> &v, const vector<double> &w)
{
    vector<double> out;
    for (unsigned int i = 0; i < v.size(); ++i)
        out.push_back(v[i] * w[i]);

    return out;
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

vector<double> misc::assignValuesMultithread(vector<double> &vec, func_type f)
{
    const unsigned int SIZE = vec.size();
    const unsigned int NUM_THREADS = thread::hardware_concurrency();
    const unsigned int ELEM_PER_THREAD = (int)(SIZE / NUM_THREADS);
    const unsigned int REST_ELEM = SIZE % NUM_THREADS;
    vector<double> output(SIZE);

    /*Lambda Ausdruck. Setzte jedes Element von <vec> iterativ ab Index <startindex> bis
    ausschließlich <endIndex> in die Funktion <f> ein.*/
    auto assignValues = [&](vector<double> vec, const unsigned int startIndex, const unsigned int endIndex, func_type f)
    {
        for (unsigned int i = startIndex; i < endIndex; ++i)
            output[i] = f(vec[i]);
    };

    // Erstelle die maximale Anzahl an threads mit einem smart-pointer
    unique_ptr<thread[]> threads(new thread[NUM_THREADS]);

    // WEISE JEDEM THREAD EINEN BLOCK DES VECTORS ZU, DEN ER BERECHNET
    for (unsigned int i = 0; i < NUM_THREADS; ++i)
    {
        unsigned int startIndex = i * ELEM_PER_THREAD;
        unsigned int endIndex = (i + 1) * ELEM_PER_THREAD;
        // Letzter Thread könnte mehr Elemente übrig haben
        if (i == NUM_THREADS - 1)
            endIndex += REST_ELEM;

        threads[i] = thread(assignValues, vec, startIndex, endIndex, f);
    }

    // BEENDE THREADS
    for (unsigned int i = 0; i < NUM_THREADS; ++i)
        threads[i].join();

    return output;
}