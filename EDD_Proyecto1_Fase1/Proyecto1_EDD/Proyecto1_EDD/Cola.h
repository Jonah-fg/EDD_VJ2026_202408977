#ifndef COLA_H
#define COLA_H
#include "Lote.h"

struct NodoCola {
    Lote dato;
    NodoCola* sig;
    NodoCola(Lote l) : dato(l), sig(nullptr) {}
};

class Cola {
private:
    NodoCola* frente;
    NodoCola* final;
public:
    Cola();
    ~Cola();
    void enqueue(Lote l);
    Lote dequeue();
    bool vacia();
    void mostrar();
    void generarReporte(const string& nombreArchivo);
};
#endif
