#include "ListaDoble.h"
#include <iostream>
#include <fstream>
using namespace std;

ListaDoble::ListaDoble() : cabeza(nullptr), cola(nullptr) {}

ListaDoble::~ListaDoble() {
    while (cabeza) {
        NodoLD* temp=cabeza;
        cabeza=cabeza->sig;
        delete temp;
    }
}

void ListaDoble::insertarOrdenado(Lote l) {
    NodoLD* nuevo=new NodoLD(l);
    if (!cabeza) {
        cabeza=cola= nuevo;
        return;
    }

    if (l.codigo< cabeza->dato.codigo) {
        nuevo->sig=cabeza;
        cabeza->ant= nuevo;
        cabeza=nuevo;
        return;
    }
    NodoLD* actual= cabeza;
    while (actual->sig && actual->sig->dato.codigo < l.codigo) {
        actual=actual->sig;
    }
    nuevo->sig =actual->sig;
    if (actual->sig){
        actual->sig->ant =nuevo;
    }
    else {
        cola=nuevo;
    }
    actual->sig=nuevo;
    nuevo->ant=actual;
}

bool ListaDoble::buscarPorCodigo(string codigo, Lote& encontrado) {
    NodoLD* actual=cabeza;
    while (actual) {
        if (actual->dato.codigo==codigo) {
            encontrado=actual->dato;
            return true;
        }
        actual =actual->sig;
    }
    return false;
}

bool ListaDoble::buscarPorFinca(string finca, Lote& encontrado) {
    NodoLD* actual = cabeza;
    while (actual) {
        if (actual->dato.finca == finca) {
            encontrado =actual->dato;
            return true;
        }
        actual =actual->sig;
    }
    return false;
}

void ListaDoble::actualizarCantidad(string codigo, int nuevaCantidad) {
    NodoLD* actual =cabeza;
    while (actual) {
        if (actual->dato.codigo == codigo) {
            actual->dato.cantidad = nuevaCantidad;
            break;
        }
        actual = actual->sig;
    }
}

void ListaDoble::mostrar() {
    NodoLD* actual=cabeza;
    while (actual) {
        cout << "Cod:" << actual->dato.codigo << ", Finca: " << actual->dato.finca<< ", Cant: " << actual->dato.cantidad << ", Rec: " << actual->dato.fechaRecepcion << ", Min: " << actual->dato.nivelMinimo;
        if (actual->dato.cantidad < actual->dato.nivelMinimo) {
            cout <<" [ALERTA: bajo stock]";
        }
        cout<<endl;
        actual =actual->sig; 
    }
}

void ListaDoble::generarReporte(const string& nombreArchivo) {
    ofstream dot(nombreArchivo + ".dot");
    dot << "digraph G {\n rankdir=LR;\n node [shape=box];\n";
    NodoLD* actual=cabeza;
    int id =0;
    while (actual) {
        string color=(actual->dato.cantidad < actual->dato.nivelMinimo) ? "red" : "green";
        dot << "node" << id << " [label=\"Código: " << actual->dato.codigo << "\\nFinca: " << actual->dato.finca
            << "\\nCantidad: " << actual->dato.cantidad << "\\nFecha: " << actual->dato.fechaRecepcion
            << "\" style=filled fillcolor=" << color << "];\n";
        if (actual->sig) {
            dot << "node" << id << " -> node" << id + 1 << " [dir=both];\n";
        }
        actual =actual->sig;
        id++;
    }
    dot << "}\n";
    dot.close();
    string comando = "dot -Tpng " + nombreArchivo + ".dot -o " + nombreArchivo + ".png";
    system(comando.c_str());
}

