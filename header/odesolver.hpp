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
    unsigned long n;           // Anzahl der Elemente mit Randwerten
    unsigned long vector_size; // Anzahl der Elemente OHNE Randwerten, also n - 2

    // Beide Vektoren ohne Randwerte
    vector<double> x;
    vector<double> u;

    // Funktionen g(x) und s(x) ausgewertet an den entsprechenden Stellen und f(x) = g(x)u(x) - s(x)
    vector<double> gvec;
    vector<double> svec;
    vector<double> fvec;
    void calc_fvec(void);

    // MULTIGRIDFUNKTIONEN
    // Berechnet Residuenvektor für einen Näherungslösungsvektor <u>
    vector<double> residual();

    // Berechnet den Residuenvektor für eine Näherungslösung des Fehlers in einem Grid-level. Das Residuum wird berechnet mit der Gleichung r = f - Av
    vector<double> residual(const vector<double> &f, const vector<double> &v, unsigned int level);

    // führt eine Restriktion auf das nächst gröbere Gitter durch vom momentanen level
    vector<double> restrict(const vector<double> &vec);

    // Interpoliert einen Vektor auf das nächste feinere level
    vector<double> interpolate(const vector<double> &vec);

    /*Glätter nach dem Gauß-Seidel Verfahren für die Vektorgleichung A*x = f für den Fall, dass f nicht von x abhängt.
    Wird im Multigrid-Verfahren verwendet, um den Fehler auf einem Gitter eines levels zu bestimmen*/
    void smoother(vector<double> &x, const vector<double> &f, const unsigned int level);

    // Berechnet von Gleichung A*x = y den Vektor x mit dem Thomas-Algorithmus, wobei A die Matrix der Differenzengleichung der numerischen DGL Lösung ist mit Schrittweite <h>
    vector<double> thomas_algorithm(const vector<double> &y, const double h);

public:
    // Randbedingungen
    double start_point;
    double end_point;

    double relaxation = 1;
    unsigned int max_level = 2; // Anzahl der Level im Multigrid

    // Funktionen, die die DGL spezifizieren
    func_type g;
    func_type s;

    // Getter und Setter
    vector<double> get_x(); // gibt die Vektoren MIT randwerten wieder
    vector<double> get_u();
    unsigned long get_vector_size() { return vector_size; }
    double get_stepwidth() { return stepwidth; }
    void set_stepwidth(double h);
    double get_residual_norm(); // Konvergenzkriterium, Berechnet momentanes Residuum

    void gauss_seidel(); // gauss-seidel-Verfahren
    double twolevel_method(vector<double> &u, vector<double> &f, unsigned int pre_smooth,
                           unsigned int post_smooth, unsigned int level);                    // Löst die DGL mit dem zweigitter verfahren
    double vcycle(unsigned int pre_smooth, unsigned int post_smooth, unsigned int maxlevel); // Löst die DGL mit dem V-Zyklus des Multigrid-Verfahrens
    unsigned long solve(double eps);                                                         // Löst die DGL mit gewünschter Zielgenauigkeit des Residuuma
    unsigned long solve(double eps, unsigned int pre_smooth, unsigned int post_smooth);      // löst die DGL mit dem Zwei-Gitter-Zyklus

    void printInFile(string fileName);              // speichert <x> und <u> in einer Datei
    void printInFile(string fileName, func_type f); // speichert <x> und <u> sowie die analytische Lösung in einer Datei

    // Konstruktor der Klasse. Muss immer die Randbedingungen sowie die maximale Schrittweite und dgl spezifizieren
    // <init_value> ist der Wert, der allen Werten im <u> vektor anfangs gegeben wird
    ODESolver(double _start_point, double _end_point, double stepwidth, func_type g, func_type s, double init_value = 0);
};