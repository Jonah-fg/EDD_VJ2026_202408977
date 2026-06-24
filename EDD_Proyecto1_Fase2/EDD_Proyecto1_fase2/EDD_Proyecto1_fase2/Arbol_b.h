#ifndef ARBOL_B_H
#define ARBOL_B_H

#include <string>
#include <vector>
#include "ArbolAVL.h"

struct NodoB {
    int numeroClaves;
    std::string fechas[6];
    ArbolAVL* arbolesAVL[6];
    NodoB* hijos[7];
    bool esHoja;

    NodoB(bool hoja);
};

class ArbolB {
private:
    NodoB* raiz;
    void dividirHijo(NodoB* padre, int indice);
    void insertarNoLleno(NodoB* nodo, const std::string& fecha, ArbolAVL* arbolAVL);
    ArbolAVL* buscarRecursivo(NodoB* nodo, const std::string& fecha);
    void recorridoInOrden(NodoB* nodo, std::vector<std::string>& fechas);
    void liberarArbol(NodoB* nodo);

public:
    ArbolB();
    ~ArbolB();

    void insertarFecha(const std::string& fecha, ArbolAVL* arbolAVL);
    ArbolAVL* obtenerArbolAVL(const std::string& fecha);
    std::vector<std::string> listarTodasLasFechas();

    void generarDot(const std::string& nombreArchivo) const; // pendiente
};

#endif