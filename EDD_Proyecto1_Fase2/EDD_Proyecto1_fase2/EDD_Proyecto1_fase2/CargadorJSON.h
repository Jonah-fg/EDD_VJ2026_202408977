#ifndef CARGA_JSON_H
#define CARGA_JSON_H

#include <string>
#include "FincaManager.h"
#include "Arbol_b.h"

class CargaJSON {
public:
    static void cargar(const std::string& ruta, FincaManager& fincas, ArbolB& arbol);
};

#endif
