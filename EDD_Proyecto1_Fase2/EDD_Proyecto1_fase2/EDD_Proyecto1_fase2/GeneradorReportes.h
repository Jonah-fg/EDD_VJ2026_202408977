#ifndef GENERADOR_REPORTES_H
#define GENERADOR_REPORTES_H

#include <string>
#include "Arbol_b.h"
#include "ArbolAVL.h"
#include "Arbol_merkle.h"
#include "FincaManager.h"

class GeneradorReportes {
private:
    ArbolB& arbolB;
    ArbolMerkle& arbolMerkle;
    FincaManager& fincaManager;
    void generarNodoB(NodoB* nodo, std::ofstream& dot, int& id);
    void generarNodoAVL(NodoAVL* nodo, std::ofstream& dot, int& id);
    int generarNodoMerkle(NodoMerkle* nodo, std::ofstream& dot, int& id, bool esRaiz = false);

public:
    GeneradorReportes(ArbolB& b, ArbolMerkle& m, FincaManager& f);

    void reporteArbolB(const std::string& nombreArchivo= "arbolB");
    void reporteAVL(const std::string& fecha, const std::string& nombreArchivo="avl");
    void reporteMerkle(const std::string& nombreArchivo ="merkle");
    void reporteTrazabilidad(const std::string& codigoLote, const std::string& nombreArchivo="trazabilidad");
    void compilarDot(const std::string& archivoDot);
};

#endif

