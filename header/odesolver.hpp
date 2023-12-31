#include <iostream>
#include <cstdio>
#include <vector>
#include "misc.hpp"

/* Klasse, die Differentialgleichungen der Form u''(x)+g(x)u(x)=s(x) löst mit homogenen Randbedingungen.
im vektor <x> werden die x-Stellen gespeichert, in <u> die Lösungswerte*/
class ODESolver
{
private:
    double stepwidth;          // Schrittweite
    unsigned long n;           // Höchster Index des gröbsten Giters (mit Randwerten)
    unsigned long vector_size; // Anzahl der Elemente OHNE Randwerten, also n - 1s

    // Beide Vektoren ohne Randwerte
    vector<double> x;
    vector<double> u;

    // Funktionen g(x) und s(x) ausgewertet an den entsprechenden Stellen und f(x) = g(x)u(x) - s(x)
    vector<double> gvec;
    vector<double> svec;

    // MULTIGRIDFUNKTIONEN

    // Berechnet den Residuenvektor für eine Näherungslösung des Fehlers in einem Grid-level. Das Residuum wird berechnet mit der Gleichung r = f - Av
    vector<double> residual(const vector<double> &gvec, const vector<double> &svec, const vector<double> &v, unsigned int level);

    // führt eine Restriktion auf das nächst gröbere Gitter durch vom momentanen level
    vector<double> restrict(const vector<double> &vec);

    // Interpoliert einen Vektor auf das nächste feinere level
    vector<double> interpolate(const vector<double> &vec);

    // Berechnet von Gleichung A*x = y den Vektor x mit dem Thomas-Algorithmus, wobei A die Matrix der Differenzengleichung der numerischen DGL Lösung ist mit Schrittweite <h>
    vector<double> thomas_algorithm(const vector<double> &y, const vector<double> &gvec, const double h);

    void gauss_seidel(vector<double> &u, vector<double> &gvec, vector<double> &svec, unsigned int level); // gauss-seidel-Verfahren
    void twolevel_method(vector<double> &u, vector<double> &gvec, vector<double> &svec, unsigned int pre_smooth,
                         unsigned int post_smooth, unsigned int level); // Löst die DGL mit dem zweigitter verfahren
public:
    // Randbedingungen
    double start_point;
    double end_point;

    double relaxation = 1;
    unsigned int max_level = 2; // Anzahl der Level im Multigrid
    bool thomas = false;        // besimmt, ob der tridiagonal-algorithmus auf dem gröbsten Gitter verwendet wird

    // Funktionen, die die DGL spezifizieren
    func_type g;
    func_type s;

    // Getter und Setter
    vector<double> get_x(bool boundaries = true); // gibt die Vektoren MIT oder OHNE randwerten wieder
    vector<double> get_u(bool boundaries = true);
    vector<double> get_gvec() { return gvec; }
    vector<double> get_svec() { return svec; }
    void set_u(vector<double> src); // u ohne Randwerte
    unsigned long get_vector_size() { return vector_size; }
    double get_stepwidth() { return stepwidth; }
    void set_stepwidth(unsigned long n);
    double get_residual_norm();         // Konvergenzkriterium, Berechnet momentanes Residuum

    void gauss_seidel(); // gauss Seidel -Verfahren speziell für Näherungslösung der DGL

    /* löst die DGL mit dem V-zyklus, u_norm speichert das skalar von u nach jeder Iteration, res_norm das skalare Residuum nach jeder Iteration (falls kein nullptr).
    pre_smooth und post_smooth bestimmen die Anzahl der Glättungsoperationen auf jedem level vor und nach der Korrektur. Falls s_relative = true wird das Abbruchkriterium relativ zu s(x) ausgewertet */
    unsigned long solve(double eps, vector<double> *u_norms = nullptr, vector<double> *res_norms = nullptr, unsigned int pre_smooth = 1, unsigned int post_smooth = 1);

    /* Konstruktor der Klasse. Muss immer die Randpunkte sowie den maximalen Index (muss Potenz von 2 sein) des gröbsten Gitters
    als Argumente annehmen sowie die Funktionen, die die dgl spezifizieren.
    <init_value> ist der Wert, der allen Werten im <u> vektor anfangs gegeben wird (standardmäßig 0)*/
    ODESolver(double _start_point, double _end_point, unsigned long n, func_type g, func_type s, double init_value = 0);
};