#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <fstream>
#include <thread>
#include <cmath>
#include <functional>
#include <memory>

#define STR_FORMAT "%.16e" // Formatierung einer Gleitkommazahl
#define PRECISION 15       // Genauigkeit von Gleitkommazahlen
// Relative Differenz zwischen zwei Zahlen
#define REL_DIF(X, Y) fabs((X - Y) / (X + Y) * 2)

using namespace std;

typedef function<double(double)> func_type;

// Überlade den * Operator, damit man jedes Element eines vectors mit einer double Zahl multiplizieren kann
vector<double> operator*(const double alpha, const vector<double> &v);

// Überlade den * Operator, damit man jedes Element eines vector mit jedem elemen einen zweiten Vectors multiplizieren kann
vector<double> operator*(const vector<double> &v, const vector<double> &w);

// Speicher die Funktionen in einem namespace, damit diese von anderen unterschieden werden können
namespace misc
{

    /*Speichert eine beliebige Anzahl von Vektoren nebeneinander in einer Datei ab.
    n steht für die Anzahl der Vektoren*/
    void printInFile(std::string fileName, unsigned int n, ...);

    // ##################### SEKANTEN-VERFAHREN #########################################
    /* Berechnet die Nullstelle einer Funktion mit dem Sekantenverfahren. spezifiziere keinen Datentypen
    für die Funktion f, damit auch Funktoren und Lambda Ausdrücke erlaubt sind. <function> ist eine Klasse,
    die mit dem header <functional> eingebunden wird. Diese Verallgemeinerung ist für diesen Code nötig*/
    double sekant(func_type f, const double xa, const double xe, double eps);

    /*Berechne im gegebenen Interval die Nullstelle einer Funktion <f> mit dem Bisektionsverfahren. <eps> ist die Zielgenauigkeit*/
    double bisection(func_type f, double xa, double xe, const double eps);


    /*Berechnet einen vector mit äquidistanten elementen, wobei <h> die maximale Schrittweite ist*/
    vector<double> equidistantValues(const double x0, const double xmax, double &h);
    
    // entferne aus einer vector<double> Datenstruktur alle Elemente, die den Wert NAN haben
    vector<double> eraseNAN(vector<double> &vec);

    // suche nach lokalem Maxima eines Arrays und gebe die Indizes als vector wieder
    // Der Wert <minimum> muss erreicht werden, damit ein Wert als Maximum gewertet wird
    vector<int> localMaximaIndices(const vector<double> &vec, double minimum);

    // findet für einen double vector den größten Wert
    double maximum(const std::vector<double>);

    /*Nimmt einen vector und berechnet die Differenzen zwischen den Elementen*/
    vector<double> calculateDifferences(vector<double> vec);

    /*Berechne die Fläche unter einer Funktion mithilfe eines gegebenen Vectors, welche Funktionswerte von äquidistanten Stellen darstellen mit der Trapezregel. */
    double trapez(const vector<double> &vec, double h);

    // eine Funktion, die die datenstruktur <values> auf die function <func> abbildet und in einem vektor abspeichert
    std::vector<double> assignValues(const std::vector<double> &values, func_type func);
    
    /*Setzte jedes Element von <vec> iterativ in die Funktion <f> ein und speichere die Ergebnisse in einem neuen vector ab.
    Nutze dabei multithreading um den Prozessor vollständig auszunutzen.*/
    vector<double> assignValuesMultithread(vector<double> &vec, func_type f);

}
