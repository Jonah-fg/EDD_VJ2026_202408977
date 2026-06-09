#ifndef PILA_H
#define PILA_H

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

struct Registro {
    string fechaHora;
    string tipoAccion;   
    string detalles;
};

struct NodoPila {
    Registro dato;
    NodoPila* abajo;    
    NodoPila(Registro r) : dato(r), abajo(nullptr) {}
};

class Pila {
private:
    NodoPila* tope;      
public:
    Pila();
    ~Pila();
    void push(Registro r);
    Registro pop();
    bool vacia();
    void mostrar();
    void generarReporte(const string& nombreArchivo);
};

#endif
