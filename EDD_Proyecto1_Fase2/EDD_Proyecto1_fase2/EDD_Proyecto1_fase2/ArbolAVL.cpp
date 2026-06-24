#include <algorithm>
#include <iostream>
#include "ArbolAVL.h"

using namespace std;

NodoAVL::NodoAVL(const Lote& l) : lote(l), hijoIzquierdo(nullptr), hijoDerecho(nullptr), altura(1) {}


int ArbolAVL::obtenerAltura(NodoAVL* nodo) {
    return (nodo ==nullptr) ? 0 : nodo->altura;
}

int ArbolAVL::obtenerFactorBalance(NodoAVL* nodo) {
    if (nodo==nullptr)
        return 0;

    return obtenerAltura(nodo->hijoIzquierdo)-obtenerAltura(nodo->hijoDerecho);
}

void ArbolAVL::actualizarAltura(NodoAVL* nodo) {
    if (nodo !=nullptr) {
        nodo->altura= 1+max(obtenerAltura(nodo->hijoIzquierdo),
            obtenerAltura(nodo->hijoDerecho));
    }
}

NodoAVL* ArbolAVL::rotarDerecha(NodoAVL* y) {
    NodoAVL* x =y->hijoIzquierdo;
    NodoAVL* T2=x->hijoDerecho;

    x->hijoDerecho = y;
    y->hijoIzquierdo =T2;

    actualizarAltura(y);
    actualizarAltura(x);
    return x;
}

NodoAVL* ArbolAVL::rotarIzquierda(NodoAVL* x) {
    NodoAVL* y=x->hijoDerecho;
    NodoAVL* T2=y->hijoIzquierdo;

    y->hijoIzquierdo =x;
    x->hijoDerecho =T2;

    actualizarAltura(x);
    actualizarAltura(y);
    return y;
}

NodoAVL* ArbolAVL::insertarRecursivo(NodoAVL* nodo, const Lote& lote) {
    if (nodo==nullptr) {
        return new NodoAVL(lote);
    }

    if (lote.codigoLote< nodo->lote.codigoLote) {
        nodo->hijoIzquierdo=insertarRecursivo(nodo->hijoIzquierdo, lote);
    }
    else if (lote.codigoLote > nodo->lote.codigoLote) {
        nodo->hijoDerecho =insertarRecursivo(nodo->hijoDerecho, lote);
    }
    else {
        return nodo;
    }

    actualizarAltura(nodo);
    int balance=obtenerFactorBalance(nodo);

    // Caso LL
    if (balance>1 && lote.codigoLote < nodo->hijoIzquierdo->lote.codigoLote) {
        return rotarDerecha(nodo);
    }
    // Caso RR
    if (balance < -1 && lote.codigoLote> nodo->hijoDerecho->lote.codigoLote) {
        return rotarIzquierda(nodo);
    }
    // Caso LR
    if (balance > 1 && lote.codigoLote > nodo->hijoIzquierdo->lote.codigoLote) {
        nodo->hijoIzquierdo=rotarIzquierda(nodo->hijoIzquierdo);
        return rotarDerecha(nodo);
    }
    // Caso RL
    if (balance < -1 && lote.codigoLote < nodo->hijoDerecho->lote.codigoLote) {
        nodo->hijoDerecho= rotarDerecha(nodo->hijoDerecho);
        return rotarIzquierda(nodo);
    }
    return nodo;
}


NodoAVL* ArbolAVL::buscarRecursivo(NodoAVL* nodo, const string& codigoLote) {
    if (nodo==nullptr || nodo->lote.codigoLote==codigoLote) {
        return nodo;
    }
    if (codigoLote < nodo->lote.codigoLote) {
        return buscarRecursivo(nodo->hijoIzquierdo, codigoLote);
    }
    else {
        return buscarRecursivo(nodo->hijoDerecho, codigoLote);
    }
}

void ArbolAVL::recorridoInOrden(NodoAVL* nodo, vector<Lote>& resultado) const {
    if (nodo ==nullptr) 
        return;

    recorridoInOrden(nodo->hijoIzquierdo, resultado);
    resultado.push_back(nodo->lote);
    recorridoInOrden(nodo->hijoDerecho, resultado);
}

void ArbolAVL::liberarArbol(NodoAVL* nodo) {
    if (nodo ==nullptr) 
        return;

    liberarArbol(nodo->hijoIzquierdo);
    liberarArbol(nodo->hijoDerecho);
    delete nodo;
}


ArbolAVL::ArbolAVL() : raiz(nullptr) {}

ArbolAVL::~ArbolAVL() {
    liberarArbol(raiz);
}

void ArbolAVL::insertarLote(const Lote& lote) {
    raiz =insertarRecursivo(raiz, lote);
}

Lote* ArbolAVL::buscarLote(const string& codigoLote) {
    NodoAVL* nodo =buscarRecursivo(raiz, codigoLote);
    return (nodo==nullptr) ? nullptr : &(nodo->lote);
}

vector<Lote> ArbolAVL::listarTodosLosLotes() const {
    vector<Lote> resultado;
    recorridoInOrden(raiz, resultado);
    return resultado;
}

bool ArbolAVL::avanzarEstado(const string& codigoLote, const string& nuevoEstado, const string& timestamp) {
    NodoAVL* nodo =buscarRecursivo(raiz, codigoLote);
    if (nodo==nullptr) 
        return false;

    static const vector<string> secuenciaEstados ={"recibido", "en_cola", "procesado", "en_bodega", "certificado_emitido"};

    int indiceActual =-1;
    int indiceNuevo =-1;
    for (size_t i =0; i<secuenciaEstados.size(); ++i) {
        if (secuenciaEstados[i]==nodo->lote.estadoActual)
            indiceActual= i;

        if (secuenciaEstados[i]==nuevoEstado) 
            indiceNuevo=i;
    }
    if (indiceActual ==-1|| indiceNuevo ==-1 || indiceNuevo<=indiceActual) {
        return false;
    }

    nodo->lote.estadoActual =nuevoEstado;
    RegistroEstado registro;
    registro.timestamp=timestamp;
    registro.estado=nuevoEstado;
    nodo->lote.historialEstados.push_back(registro);
    return true;
}


