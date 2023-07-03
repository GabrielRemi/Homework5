#include <iostream>
#include <cstdio>
#include <vector>
#include "misc.hpp"

enum iteration_type
{
    jacobi,
    gauss_seidel
};

/* Klasse, die Differentialgleichungen der Form u''(x)+g(x)u(x)=s(x) löst.
im vektor <x> werden die x-Stellen gespeichert, in <u> die Lösungswerte*/
class ODESolver
{
private:
    double stepwidth;          // Schrittweite
    unsigned long vector_size; // Anzahl der Elemente in den vektoren
    double residual_norm;      // Konvergenzkriterium

    vector<double> x;
    vector<double> u;

    // Funktionen g(x) und s(x) ausgewertet an den entsprechenden Stellen
    vector<double> gvec;
    vector<double> svec;

    // Funktional, welches bei richtiger Lösung gegen ihr Minimum strebt
    double functional();

    // MULTIGRIDFUNKTIONEN
    // Berechnet Residuenvektor für einen Näherungslösungsvektor <u>
    vector<double> residual(const vector<double> &u, unsigned int level);

    // führt eine Restriktion auf das nächst gröbere Gitter durch vom momentanen level
    vector<double> restrict(const vector<double> &vec);

    // Interpoliert einen Vektor auf das nächste feinere level
    vector<double> interpolate(const vector<double> &vec);

    // Berechnet von Gleichung A*x = y den Vektor x mit dem Thomas-Algorithmus, wobei A die Matrix der Differenzengleichung der numerischen DGL Lösung ist mit Schrittweite <h>
    vector<double> tridig(const vector<double> &y, const double h);

public:
    // Randbedingungen
    double start_point;
    double end_point;
    double start_value;
    double end_value;

    double relaxation = 1;

    // Funktionen, die die DGL spezifizieren
    func_type g;
    func_type s;

    // Getter und Setter
    vector<double> get_x() { return x; }
    vector<double> get_u() { return u; }
    unsigned long get_vector_size() { return vector_size; }
    double get_stepwidth() { return stepwidth; }
    void set_stepwidth(double h);
    double get_residual_norm() { return residual_norm; }

    void jacobi();                                                                      // jacobi-Verfahren
    void gauss_seidel();                                                                // gauss-seidel-Verfahren
    double twolevel_method(unsigned int pre_smooth, unsigned int post_smooth);          // Löst die DGL mit dem zweigitter verfahren
    unsigned long solve(double eps, iteration_type it);                                 // Löst die DGL mit gewünschter Zielgenauigkeit des Funktionals und iterationsmethode
    unsigned long solve(double eps, unsigned int pre_smooth, unsigned int post_smooth); // löst die DGL mit dem Zwei-Gitter-Zyklus

    void printInFile(string fileName);              // speichert <x> und <u> in einer Datei
    void printInFile(string fileName, func_type f); // speichert <x> und <u> sowie die analytische Lösung in einer Datei

    // Konstruktor der Klasse. Muss immer die Randbedingungen sowie die maximale Schrittweite und dgl spezifizieren
    // <init_value> ist der Wert, der allen Werten im <u> vektor anfangs gegeben wird
    ODESolver(double _start_point, double _end_point, double _start_value, double _end_value, double stepwidth, func_type g, func_type s, double init_value = 0);
};