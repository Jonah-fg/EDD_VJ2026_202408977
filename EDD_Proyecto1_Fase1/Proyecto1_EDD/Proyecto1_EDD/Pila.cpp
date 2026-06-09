#include "Pila.h"

Pila::Pila() : tope(nullptr) {}

Pila::~Pila() {
    while (!vacia())
        pop();
}

bool Pila::vacia() {
    return tope==nullptr;
}

void Pila::push(Registro r) {
    NodoPila* nuevo=new NodoPila(r);
    nuevo->abajo =tope;  
    tope =nuevo;         
}

Registro Pila::pop() {
    if (vacia()) {
        Registro vacio;
        vacio.tipoAccion ="";
        return vacio;
    }
    NodoPila* temp= tope;
    Registro r=temp->dato;
    tope =tope->abajo;
    delete temp;
    return r;
}


void Pila::mostrar(){
    NodoPila* actual=tope;
    while (actual) {
        cout <<actual->dato.fechaHora << " | " << actual->dato.tipoAccion << " | " << actual->dato.detalles << endl;
        actual=actual->abajo;
    }
}

void Pila::generarReporte(const string& nombreArchivo) {
    ofstream dot(nombreArchivo + ".dot");
    dot<<"digraph G {\n rankdir=TB;\n node [shape=record];\n";
    NodoPila* actual=tope;
    int id =0;
    string color;
    while (actual) {
        if (actual->dato.tipoAccion =="Registro lote") color = "blue";
        else if (actual->dato.tipoAccion== "Registro entrega") color = "orange";
        else color ="green";  // Procesar lote
        dot << "node"<< id <<" [label=\"" << actual->dato.fechaHora<<"\\n" << actual->dato.tipoAccion << "\\n" << actual->dato.detalles<< "\" style=filled fillcolor=" << color << "];\n";
        if (actual->abajo) {
            dot <<"node" << id << " -> node" << id + 1 <<";\n";
        }
        actual=actual->abajo;
        id++;
    }
    dot<<"}\n";
    dot.close();
    string comando= "dot -Tpng " +nombreArchivo +".dot -o "+nombreArchivo + ".png";
    system(comando.c_str());
}