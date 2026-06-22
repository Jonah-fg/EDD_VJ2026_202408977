#ifndef ArbolAVL_H
#define ArbolAVL_H

#include <string>
#include <vector>

struct Lote {
    std::string codigoLote;
    std::string codigoFinca;
    std::string nombreFinca;
    int sacos;
    std::string tipoCafe;
    std::string rutaTomada;
    float distanciaKm;
    std::string estadoActual;
    std::vector<std::string> historialEstados;
};

struct NodoAVL {
    Lote lote;
    NodoAVL* hijoIzquierdo;
    NodoAVL* hijoDerecho;
    int altura;

    NodoAVL(const Lote& lote);
};

class ArbolAVL {
private:
    NodoAVL* raiz;

    int obtenerAltura(NodoAVL* nodo);
    int obtenerFactorBalance(NodoAVL* nodo);
    void actualizarAltura(NodoAVL* nodo);

    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);

    NodoAVL* insertarRecursivo(NodoAVL* nodo, const Lote& lote);
    NodoAVL* buscarRecursivo(NodoAVL* nodo, const std::string& codigoLote);

    void recorridoInOrden(NodoAVL* nodo, std::vector<Lote>& resultado) const;
    void liberarArbol(NodoAVL* nodo);

public:
    ArbolAVL();
    ~ArbolAVL();

    void insertarLote(const Lote& lote);
    Lote* buscarLote(const std::string& codigoLote);
    std::vector<Lote> listarTodosLosLotes() const;

    bool avanzarEstado(const std::string& codigoLote,
        const std::string& nuevoEstado,
        const std::string& timestamp);
};

#endif