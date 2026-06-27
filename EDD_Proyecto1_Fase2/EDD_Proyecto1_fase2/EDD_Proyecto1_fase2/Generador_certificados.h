#ifndef CERTIFICATE_GENERATOR_H
#define CERTIFICATE_GENERATOR_H

#include <string>
#include "ArbolAVL.h" 
#include "Arbol_merkle.h"

class GeneradorCertificados {
private:
    static const std::string CARPETA_SALIDA;
    static void asegurarCarpeta();

    // Construye el texto plano del certificado según el formato del enunciado
    static std::string construirContenido(const Lote& lote);

public:
    static std::string generarYGuardarCertificado(Lote& lote, ArbolMerkle& merkle);
    static std::string leerYDesencriptarCertificado(const std::string& nombreArchivo);
};

#endif
