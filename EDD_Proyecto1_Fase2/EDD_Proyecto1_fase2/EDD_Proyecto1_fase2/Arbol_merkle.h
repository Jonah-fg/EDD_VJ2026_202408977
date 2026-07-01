#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <string>
#include <vector>

struct NodoMerkle {
    std::string hash;
    NodoMerkle* izquierdo;
    NodoMerkle* derecho;
    NodoMerkle(const std::string& h);
};

class ArbolMerkle {
private:
    NodoMerkle* raiz;
    std::vector<std::string> hojas; 

    NodoMerkle* construirArbol(const std::vector<std::string>& hashes, int inicio, int fin);
    void liberarNodos(NodoMerkle* nodo);

public:
    ArbolMerkle();
    ~ArbolMerkle();

    // Construye el árbol desde una lista de hashes 
    void construir(const std::vector<std::string>& hashes);
    void agregarHoja(const std::string& hash);
    bool verificarIntegridad(const std::string& hashHoja) const;
    std::string obtenerHashRaiz() const;

    // Devuelve la cantidad de hojas
    size_t cantidadHojas() const {
        return hojas.size();
    }
    NodoMerkle* getRaiz() const {return raiz;}
    int obtenerPosicionHoja(const std::string& hash) const;
    int obtenerNumeroHojas() const { return hojas.size();}
};

#endif


