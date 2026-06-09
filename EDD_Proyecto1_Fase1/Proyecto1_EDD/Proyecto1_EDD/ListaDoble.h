#ifndef LISTADOBLE_H
#define LISTADOBLE_H
#include "Lote.h"

struct NodoLD {
    Lote dato;
    NodoLD* sig;
    NodoLD* ant;
    NodoLD(Lote lote) : dato(lote), sig(nullptr), ant(nullptr) {}
};

class ListaDoble {
private:
    NodoLD* cabeza;
    NodoLD* cola;
public:
    ListaDoble();
    ~ListaDoble();
    void insertarOrdenado(Lote l);
    bool buscarPorCodigo(string codigo, Lote& encontrado);
    bool buscarPorFinca(string finca, Lote& encontrado);
    void actualizarCantidad(string codigo, int nuevaCantidad);
    void mostrar(); 
    void generarReporte(const string& nombreArchivo);
    NodoLD* getCabeza() { return cabeza; }
};
#endif