#include "Cola.h"
#include <iostream>
#include <fstream>
using namespace std;

Cola::Cola() : frente(nullptr), final(nullptr) {}
Cola::~Cola() { 
    while (!vacia())
        dequeue(); 
}

void Cola::enqueue(Lote l) {
    NodoCola* nuevo =new NodoCola(l);
    if (vacia()) {
        frente = nuevo;
        final =nuevo;
    }
    else {
        final->sig =nuevo;
        final=nuevo;
    }
}

Lote Cola::dequeue() {
    if (vacia()) {
        Lote vacio;
        vacio.codigo = ""; 
        return vacio; 
    }
    NodoCola* temp=frente;
    Lote l=temp->dato;
    frente =frente->sig;
    if (!frente)
        final=nullptr;
    delete temp;
    return l;
}

bool Cola::vacia() { 
    return frente== nullptr; 
}

void Cola::mostrar() {
    NodoCola* actual =frente;
    while (actual) {
        cout << "Lote: "<< actual->dato.codigo << ", Finca: " << actual->dato.finca<< ", Cant: " << actual->dato.cantidad << endl;
        actual =actual->sig;
    }
}

void Cola::generarReporte(const string& nombreArchivo) {
    ofstream dot(nombreArchivo + ".dot");
    dot << "digraph G {\n rankdir=LR;\n node [shape=circle];\n";
    NodoCola* actual=frente;
    int id = 0;
    while (actual) {
        dot << "node" << id << " [label=\"Lote: " << actual->dato.codigo << "\\nFinca: " << actual->dato.finca
            << "\\nCant: "<< actual->dato.cantidad << "\"];\n";
        if (actual->sig) {
            dot << "node"<<id<< " -> node" << id+1 << ";\n";
        }
        actual =actual->sig;
        id++;
    }
    dot << "}\n";
    dot.close();
    system(("dot -Tpng "+ nombreArchivo + ".dot -o " + nombreArchivo + ".png").c_str());
}
