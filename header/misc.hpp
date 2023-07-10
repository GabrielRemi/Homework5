#pragma once
#include <iostream>
#include <vector>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <fstream>
#include <cmath>
#include <functional>
#include <memory>

#define STR_FORMAT "%.16e" // Formatierung einer Gleitkommazahl
#define PRECISION 15       // Genauigkeit von Gleitkommazahlen

using namespace std;

typedef function<double(double)> func_type;

// Speicher die Funktionen in einem namespace, damit diese von anderen unterschieden werden können
namespace misc
{
    /*Speichert eine beliebige Anzahl von Vektoren nebeneinander in einer Datei ab.
    n steht für die Anzahl der Vektoren*/
    void printInFile(std::string fileName, unsigned int n, ...);

    // Berechne die euklidische Norm eines vektors
    double norm(const vector<double> &y);

    /*Berechnet einen vector mit äquidistanten elementen, wobei <h> die maximale Schrittweite ist*/
    vector<double> equidistantValues(const double x0, const double xmax, double &h);

    // eine Funktion, die die datenstruktur <values> auf die function <func> abbildet und in einem vektor abspeichert
    std::vector<double> assignValues(const std::vector<double> &values, func_type func);
}