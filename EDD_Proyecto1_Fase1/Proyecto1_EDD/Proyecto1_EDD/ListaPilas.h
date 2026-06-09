#ifndef LISTAPILAS_H
#define LISTAPILAS_H

#include "Entrega.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

struct NodoEntrega {
    Entrega dato;
    NodoEntrega* abajo;
    NodoEntrega(Entrega e) : dato(e), abajo(nullptr) {}
};

struct NodoFinca {
    string nombreFinca;
    NodoEntrega* topeEntregas;   
    NodoFinca* sig;              
    NodoFinca(string nombre) : nombreFinca(nombre), topeEntregas(nullptr), sig(nullptr) {}
};

class ListaPilas {
private:
    NodoFinca* cabeza;
public:
    ListaPilas();
    ~ListaPilas();
    void agregarFinca(string nombre);
    bool existeFinca(string nombre);
    void registrarEntrega(string nombreFinca, Entrega e);
    void mostrar();
    void generarReporte(const string& nombreArchivo);
};

#endif