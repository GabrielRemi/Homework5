#include "misc.hpp"

void misc::printInFile(std::string fileName, unsigned int n, ...)
{
    std::fstream file;
    file.open(fileName, std::ios_base::out);

    va_list args;
    va_start(args, n);

    // Pointer, auf das vektor array der Argumente
    std::vector<double> **arrayp = new vector<double>*[n * sizeof(double *)];

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

double misc::maximum(std::vector<double> vec)
{
    double max = vec[0];
    for (double elem : vec)
        if (elem > max)
            max = elem;

    return max;
}

/*Nimmt einen vector und berechnet die Differenzen zwischen den Elementen*/
vector<double> misc::calculateDifferences(vector<double> vec)
{
    vector<double> out;
    for (unsigned int i = 1; i < vec.size(); ++i)
        out.push_back(vec[i] - vec[i - 1]);

    return out;
}

double misc::sekant(func_type f, const double xa, const double xe, double eps)
{
    if (REL_DIF(xa, xe) < eps)
    {
        printf("Startintervall zu klein");
        return NAN;
    }
    double old_x0;
    double x1 = xa; // Stützstellen des Iterationsverfahrens
    double x2 = xe;
    double x0 = x2; // Nullstelle

    unsigned int iteration = 0;
    unsigned int iteration_max = 100;

    while (REL_DIF(x1, x2) > eps && iteration < iteration_max)
    {
        iteration++;
        old_x0 = x0;
        // Iterationsformel des Sekantenverfahrens
        x0 = (x1 * f(x2) - x2 * f(x1)) / (f(x2) - f(x1));

        // verhindere, dass durch null geteilt wird.
        if (old_x0 == 0 && x0 == 0)
            break;

#define SHOW_DETAILS 0
#if SHOW_DETAILS == 1
        cout.precision(10);
        cout << "[Sekant]  ";
        cout << scientific << x1 << " " << x2 << " " << f(x1) << " " << f(x2) << " " << x0 << endl;
#endif

        x1 = x2;
        x2 = x0;
    };

    if (iteration == iteration_max)
        printf("Keine Konvergenz eingetroffen\n");

    // Damit bei der 0 als Nullstelle das Minuszeichen nicht angezeigt wird
    if (x0 == 0)
        x0 = fabs(x0);

    return x0;
}

double misc::norm(const vector<double> &y)
{
    double norm = 0;
    for (double elem : y)
        norm += elem*elem;
    norm = sqrt(norm);

    return norm;
}

double misc::bisection(func_type f, double xa, double xe, const double eps)
{
    // FALLS DAS INTERVALL FLASCH GEWÄHLT, BRECHE AB
    if (xa > xe)
    {
        cout << "Fehler: xe muss größer sein als xa" << endl;
        return NAN;
    }

    double delta = fabs(xe - xa);
    // FALLS ANFANGSINTERVALL ZU KLEIN, BRECHE AB
    if (delta < eps)
    {
        cout << "Fehler: Intervall zu klein gewählt. Vergrößere das Intervall oder verkleinere die Zielgenauigkeit" << endl;
        return NAN;
    }

    double x0 = 0;
    double fa = f(xa);
    double fe = f(xe);

    // UNTERSUCHE DEN RAND AUF NULLSTELLEN
    if (!(fa * fe <= 0))
        return NAN;
    else if (fa == 0)
        return xa;
    else if (fe == 0)
        return xe;

    // HALBIERE DAS INTERVALL
    delta /= 2;
    double xMiddle = xa + delta;
    double fMiddle = f(xMiddle);
    x0 = xMiddle;

    // HALBIERE INTERVALL, BIS GENAUIGKEIT ERREICHT
    while (delta > eps)
    {
        if (fMiddle == 0)
            return xMiddle;

        // UTNERSUCHE BEIDE INTERVALLE AUF VORZEICHENWECHSEL
        delta /= 2;
        if (fa * fMiddle < 0)
        {
            xe = xMiddle;
            fe = fMiddle;
            xMiddle = xa + delta;
            fMiddle = f(xMiddle);
        }
        else if (fe * fMiddle < 0)
        {
            xa = xMiddle;
            fa = fMiddle;
            xMiddle = xe - delta;
            fMiddle = f(xMiddle);
        }

        x0 = xMiddle;
    }

    x0 = xa + delta;

    return x0;
}

vector<double> misc::eraseNAN(vector<double> &vec)
{
    unsigned int size = vec.size();
    vector<double> out;

    for (unsigned int i = 0; i < size; ++i)
        if (!isnan(vec[i]))
            out.push_back(vec[i]);

    return out;
}

vector<int> misc::localMaximaIndices(const vector<double> &vec, double minimum)
{
    vector<int> out;

    for (unsigned int i = 1; i < vec.size() - 1; ++i)
        if ((vec[i] >= minimum) && (vec[i] > vec[i - 1]) && (vec[i] > vec[i + 1]))
            out.push_back(i);

    return out;
}

double misc::trapez(const vector<double> &vec, double h)
{
    unsigned int size = vec.size();
    double output = 0.;
    output += 0.5 * (vec[0] + vec[size - 1]);

    for (unsigned int i = 1; i < size - 1; ++i)
        output += vec[i];

    output *= h;

    return output;
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