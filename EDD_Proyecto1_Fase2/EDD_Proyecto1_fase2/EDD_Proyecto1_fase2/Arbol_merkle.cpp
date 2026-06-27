#include "Arbol_merkle.h"
#include "hash_utils.h"
#include <cmath>
#include <stdexcept>

using namespace std;

NodoMerkle::NodoMerkle(const string& h) : hash(h), izquierdo(nullptr), derecho(nullptr) {}

NodoMerkle* ArbolMerkle::construirArbol(const vector<string>& hashes, int inicio, int fin) {
    if (inicio>fin)
        return nullptr;

    if (inicio==fin) {
        return new NodoMerkle(hashes[inicio]);
    }

    int medio=(inicio + fin) / 2;
    NodoMerkle* izq =construirArbol(hashes, inicio, medio);
    NodoMerkle* der= construirArbol(hashes, medio + 1, fin);

    string hashCombinado=calcularHash(izq->hash+ der->hash);
    NodoMerkle* nodo =new NodoMerkle(hashCombinado);
    nodo->izquierdo = izq;
    nodo->derecho=der;
    return nodo;
}

void ArbolMerkle::liberarNodos(NodoMerkle* nodo) {
    if (nodo==nullptr) 
        return;

    liberarNodos(nodo->izquierdo);
    liberarNodos(nodo->derecho);
    delete nodo;
}

ArbolMerkle::ArbolMerkle() : raiz(nullptr) {}

ArbolMerkle::~ArbolMerkle() {
    liberarNodos(raiz);
}

void ArbolMerkle::construir(const vector<string>& hashes) {
    liberarNodos(raiz);
    hojas =hashes;

    if (hashes.empty()) {
        raiz=nullptr;
        return;
    }

	// duplicamos la ultima hoja si el número de hojas es impar
    vector<string> hojasProcesadas=hashes;
    if (hojasProcesadas.size()%2 != 0) {
        hojasProcesadas.push_back(hashes.back());
    }
    raiz = construirArbol(hojasProcesadas, 0, hojasProcesadas.size() - 1);
}

void ArbolMerkle::agregarHoja(const string& hash) {
    hojas.push_back(hash);
    construir(hojas); 
}

bool ArbolMerkle::verificarIntegridad(const string& hashHoja) const {
    for (const auto& h :hojas) {
        if (h==hashHoja)
            return true;
    }
    return false;
}

string ArbolMerkle::obtenerHashRaiz() const {
    if (raiz ==nullptr)
        return "";

    return raiz->hash;
}


