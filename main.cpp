#include "odesolver.hpp"
#include <limits>
#include <sys/time.h>

#define TIME_MEASURING 1

using namespace std;

// Berechnet für Aufgabe 1 für das angebene maximale Level den Iterationsverlauf der Norm der Näherungslösung
void calc_u_norm(unsigned int level)
{
    unsigned int exponent = 7;
    unsigned long n = pow(2, exponent);
    // Bedingungen der Anfangslösung
    for (unsigned int wn = 1; wn < n - 1; wn += 10)
    {
        unsigned int wave_number = wn;
        char fileName[50];
        sprintf(fileName, "results/unorm_aufgabe1_level%d_k%d", level, wave_number);

        ODESolver dgl(0, M_PI, n, nullptr, nullptr, 0);

        double h = dgl.get_stepwidth();
        double eps = 0.1 * h * h;

        // initiiere Anfangslösung
        vector<double> x = dgl.get_x(false);
        vector<double> u = misc::assignValues(x, [&](double x)
                                              { return sin(wave_number * x); });
        dgl.set_u(u);

        vector<double> u_norms;
        dgl.max_level = level;
        unsigned int iterations = dgl.solve(eps, &u_norms, 1, 1);
        fprintf(stdout, "[level] %u, [Wellenzahl] %u, Iterationen: %u, Genauigkeit: %.5e\n", level, wave_number, dgl.get_residual_norm());

        vector<double> iter_values(iterations);
        for (unsigned int i = 0; i < iterations; ++i)
            iter_values[i] = (double)(i + 1);
        misc::printInFile(fileName, 2, &iter_values, &u_norms);
    }
}

void aufgabe_1()
{
    for (int i = 1; i <= 5; ++i)
        calc_u_norm(i);
}

void aufgabe_2()
{
}

int main()
{
#if TIME_MEASURING
    // zeigt nach Durchlauf die Ausführdauer an
    struct timeval start, end;
    gettimeofday(&start, NULL);
#endif

    aufgabe_1();
    aufgabe_2();

#if TIME_MEASURING
    gettimeofday(&end, NULL);
    // Berechne die vergangene Zeit in sekungden
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Dauer: %.3f sekunden\n", elapsed_time);
#endif
}