/*KOMPILIER- UND AUSFÜHRBEFEHL: make
AUSFÜHRDAUER: 5 sekunden*/
#include "odesolver.hpp"
#include <limits>
#include <sys/time.h>

#define TIME_MEASURING 1

using namespace std;

// ----------------------------------------------------------------------------------------
//                                AUFGABE 1
// ----------------------------------------------------------------------------------------
// Berechnet für Aufgabe 1 für das angebene maximale Level den Iterationsverlauf der Norm der Näherungslösung
void calc_u_norm(unsigned int level)
{
    unsigned int exponent = 7;
    unsigned long n = pow(2, exponent);
    // Bedingungen der Anfangslösung
    unsigned int wave_numbers[] = {1, 21, 41, 61, 81, 101, 121};
    unsigned int wave_size = sizeof(wave_numbers) / sizeof(unsigned int);
    for (unsigned int i = 0; i < wave_size; ++i)
    {
        unsigned int wave_number = wave_numbers[i];
        char fileName[50];
        sprintf(fileName, "results/unorm_aufgabe1_level%d_k%d", level, wave_number);

        ODESolver dgl(0, M_PI, n, nullptr, nullptr);

        double h = dgl.get_stepwidth();
        double eps = 0.1 * h * h;

        // initiiere Anfangslösung
        vector<double> x = dgl.get_x(false);
        vector<double> u = misc::assignValues(x, [&](double x)
                                              { return sin(wave_number * x); });
        dgl.set_u(u);

        vector<double> u_norms;
        dgl.max_level = level;
        unsigned int iterations = dgl.solve(eps, &u_norms, nullptr, 1, 1);
        fprintf(stdout, "[level] %u, [Wellenzahl] %u, Iterationen: %u, Genauigkeit: %.5e\n", level, wave_number, iterations, dgl.get_residual_norm());

        vector<double> iter_values(iterations);
        for (unsigned int i = 0; i < iterations; ++i)
            iter_values[i] = (double)(i + 1);
        misc::printInFile(fileName, 2, &iter_values, &u_norms);
    }
    fprintf(stdout, "\n");
}

void aufgabe_1()
{
    fprintf(stdout, "------[AUFGABE 1]------\n");
    for (int i = 1; i <= 5; ++i)
        calc_u_norm(i);
}

// ----------------------------------------------------------------------------------------
//                                AUFGABE 2
// ----------------------------------------------------------------------------------------

/* Berechne DGL Lösung für Aufgabe 2 mit feinstem Gitter mit 2^n Elementen. falls multigrid=true, so wird das multigrid-verfahren verwendet,
wobei das gröbste Gitter immer 8 Elemente besitzt.*/
void calc_res_norm(bool multigrid, unsigned int n)
{
    auto g = [](double x) -> double
    {
        return -1;
    };
    auto s = [](double x) -> double
    {
        return -0.5 * x * exp(-x);
    };

    ODESolver dgl(0, 20, pow(2, n), g, s);
    if (multigrid)
        dgl.max_level = n - 2;
    else
        dgl.max_level = 1;
    dgl.thomas = true;

    // Bestimme Genauigkeit in Abhängigkeit von der norm von s
    double h = dgl.get_stepwidth();
    double eps = 1e-3 * h * h;

    vector<double> residuum;
    unsigned int iterations = dgl.solve(eps, nullptr, &residuum, 1, 1);

    // in einer Datei speichern
    char fileName[50];
    sprintf(fileName, "results/res_norm_aufgabe2_multi=%d_N=%u", multigrid, n);
    vector<double> all_iterations(iterations);
    for (unsigned int i = 0; i < iterations; ++i)
        all_iterations[i] = i + 1;
    misc::printInFile(fileName, 2, all_iterations, residuum);

    // Ergebnisse im Terminal ausgeben
    fprintf(stdout, "[N] %u, [Level] %u, Iterationen: %u, Genauigkeit: %.5e\n", n, dgl.max_level, iterations, residuum[iterations - 1]);
}

void aufgabe_2()
{
    fprintf(stdout, "\n------[AUFGABE 2]------\n");
    unsigned int n[] = {4, 6, 8, 10, 12};
    unsigned int n_size = sizeof(n) / sizeof(int);
    for (int i = 0; i < n_size; ++i)
    {
        // Berechnet dgl einmal ohne und einmal mit multigrid
        calc_res_norm(false, n[i]);
        calc_res_norm(true, n[i]);
    }
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