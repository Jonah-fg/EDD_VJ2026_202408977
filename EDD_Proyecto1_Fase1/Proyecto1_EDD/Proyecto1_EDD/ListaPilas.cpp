#include "ListaPilas.h"

ListaPilas::ListaPilas() : cabeza(nullptr) {}

ListaPilas::~ListaPilas() {
    while (cabeza) {
        // Libera la pila de entregas de esta finca
        while (cabeza->topeEntregas) {
            NodoEntrega* temp =cabeza->topeEntregas;
            cabeza->topeEntregas=cabeza->topeEntregas->abajo;
            delete temp;
        }
        NodoFinca* temp=cabeza;
        cabeza =cabeza->sig;
        delete temp;
    }
}

void ListaPilas::agregarFinca(string nombre) {
    if (existeFinca(nombre))
        return;
    NodoFinca* nuevo=new NodoFinca(nombre);
    nuevo->sig =cabeza;
    cabeza =nuevo;
}

bool ListaPilas::existeFinca(string nombre) {
    NodoFinca* actual =cabeza;
    while (actual){
        if (actual->nombreFinca == nombre)
            return true;
        actual=actual->sig;
    }
    return false;
}

void ListaPilas::registrarEntrega(string nombreFinca, Entrega e) {
    NodoFinca* actual =cabeza;
    while (actual) {
        if (actual->nombreFinca==nombreFinca) {
            NodoEntrega* nuevo= new NodoEntrega(e);
            nuevo->abajo = actual->topeEntregas;
            actual->topeEntregas=nuevo;
            return;
        }
        actual =actual->sig;
    }
}

void ListaPilas::mostrar() {
    NodoFinca* actual=cabeza;
    while (actual) {
        cout << "Finca: "<< actual->nombreFinca << " -> ";
        NodoEntrega* e=actual->topeEntregas;
        while (e) {
            cout <<"("<< e->dato.fecha <<", " << e->dato.cantidad << ") ";
            e=e->abajo;
        }
        cout <<endl;
        actual =actual->sig;
    }
}

void ListaPilas::generarReporte(const string& nombreArchivo) {
    ofstream dot(nombreArchivo + ".dot");
    dot <<"digraph G{\n rankdir=LR;\n node [shape=box];\n";
    NodoFinca* actual=cabeza;
    int idF =0;
    while (actual) {
        dot << "subgraph cluster_" << idF << " {\n label=\"" << actual->nombreFinca << "\";\n rankdir=TB;\n";
        NodoEntrega* e = actual->topeEntregas;
        int idEntrega =0;
        NodoEntrega* prev= nullptr;
        while (e){
            dot << "e" << idF << "_" << idEntrega << " [label=\"Fecha: " << e->dato.fecha << "\\nCantidad: " << e->dato.cantidad << "\"];\n";
            if (prev) {
                dot << "e" << idF << "_"<< idEntrega-1 << " -> e" << idF << "_" << idEntrega << ";\n";
            }
            prev=e;
            e =e->abajo;
            idEntrega++;
        }
        dot << "}\n";
        if (actual->sig) {
            dot << "finca" << idF << " -> finca" << idF + 1 << " [style=invis];\n";
        }
        actual= actual->sig;
        idF++;
    }
    dot <<"}\n";
    dot.close();
    system(("dot -Tpng " + nombreArchivo + ".dot -o " + nombreArchivo + ".png").c_str());
}
