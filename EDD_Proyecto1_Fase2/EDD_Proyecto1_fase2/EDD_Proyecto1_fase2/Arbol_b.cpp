#include "Arbol_b.h"

using namespace std;

NodoB::NodoB(bool hoja) : numeroClaves(0), esHoja(hoja) {
    for (int i=0; i<6; ++i)
        arbolesAVL[i]=nullptr;

    for (int i =0; i<7; ++i)
        hijos[i]=nullptr;
}

void ArbolB::dividirHijo(NodoB* padre, int indice) {
    NodoB* hijo =padre->hijos[indice];
    NodoB* nuevoNodo=new NodoB(hijo->esHoja);

    int mitad = 3;

    nuevoNodo->numeroClaves=2;
    for (int i=0; i<2; ++i) {
        nuevoNodo->fechas[i] =hijo->fechas[i + mitad+1];
        nuevoNodo->arbolesAVL[i]=hijo->arbolesAVL[i +mitad+ 1];
    }
    if (!hijo->esHoja) {
        for (int i = 0; i <3; ++i) {
            nuevoNodo->hijos[i]=hijo->hijos[i + mitad + 1];
        }
    }

    hijo->numeroClaves = 3;

    for (int i = padre->numeroClaves; i>indice; --i) {
        padre->fechas[i]=padre->fechas[i- 1];
        padre->arbolesAVL[i]=padre->arbolesAVL[i - 1];
        padre->hijos[i + 1] =padre->hijos[i];
    }

    padre->fechas[indice] = hijo->fechas[mitad];
    padre->arbolesAVL[indice] = hijo->arbolesAVL[mitad];
    padre->hijos[indice + 1] = nuevoNodo;
    padre->numeroClaves++;
}

void ArbolB::insertarNoLleno(NodoB* nodo, const string& fecha, ArbolAVL* arbolAVL) {
    int i =nodo->numeroClaves-1;

    if (nodo->esHoja) {
        while (i >= 0 && fecha<nodo->fechas[i]) {
            nodo->fechas[i+1] = nodo->fechas[i];
            nodo->arbolesAVL[i+ 1]= nodo->arbolesAVL[i];
            i--;
        }
        nodo->fechas[i +1] = fecha;
        nodo->arbolesAVL[i+1]=arbolAVL;
        nodo->numeroClaves++;
    }
    else {
        while (i >= 0 && fecha < nodo->fechas[i]) i--;
        i++;
        if (nodo->hijos[i]->numeroClaves == 6) {
            dividirHijo(nodo, i);
            if (fecha > nodo->fechas[i]) i++;
        }
        insertarNoLleno(nodo->hijos[i], fecha, arbolAVL);
    }
}

ArbolAVL* ArbolB::buscarRecursivo(NodoB* nodo, const string& fecha) {
    if (nodo ==nullptr) 
        return nullptr;

    int i=0;
    while (i < nodo->numeroClaves && fecha > nodo->fechas[i])
        i++;

    if (i<nodo->numeroClaves && fecha== nodo->fechas[i]) {
        return nodo->arbolesAVL[i];
    }
    if (nodo->esHoja)
        return nullptr;

    return buscarRecursivo(nodo->hijos[i], fecha);
}

void ArbolB::recorridoInOrden(NodoB* nodo, vector<string>& fechas) {
    if (nodo == nullptr) 
        return;

    int i;
    for (i=0; i < nodo->numeroClaves; ++i) {
        if (!nodo->esHoja)
            recorridoInOrden(nodo->hijos[i], fechas);

        fechas.push_back(nodo->fechas[i]);
    }
    if (!nodo->esHoja) 
        recorridoInOrden(nodo->hijos[i], fechas);
}

void ArbolB::liberarArbol(NodoB* nodo) {
    if (nodo==nullptr) 
        return;

    if (!nodo->esHoja) {
        for (int i=0; i <= nodo->numeroClaves; ++i) {
            liberarArbol(nodo->hijos[i]);
        }
    }
    for (int i =0; i < nodo->numeroClaves; ++i) {
        delete nodo->arbolesAVL[i];
    }
    delete nodo;
}

ArbolB::ArbolB() : raiz(nullptr) {}

ArbolB::~ArbolB() {
    if (raiz) liberarArbol(raiz);
}

void ArbolB::insertarFecha(const string& fecha, ArbolAVL* arbolAVL) {
    if (raiz==nullptr) {
        raiz = new NodoB(true);
        raiz->fechas[0]=fecha;
        raiz->arbolesAVL[0]= arbolAVL;
        raiz->numeroClaves= 1;
        return;
    }
    if (raiz->numeroClaves==6) {
        NodoB* nuevaRaiz=new NodoB(false);
        nuevaRaiz->hijos[0]=raiz;
        dividirHijo(nuevaRaiz, 0);
        raiz =nuevaRaiz;
    }
    insertarNoLleno(raiz, fecha, arbolAVL);
}

ArbolAVL* ArbolB::obtenerArbolAVL(const string& fecha) {
    return buscarRecursivo(raiz, fecha);
}

vector<string> ArbolB::listarTodasLasFechas() {
    vector<string> fechas;
    if (raiz) recorridoInOrden(raiz, fechas);
        return fechas;
}