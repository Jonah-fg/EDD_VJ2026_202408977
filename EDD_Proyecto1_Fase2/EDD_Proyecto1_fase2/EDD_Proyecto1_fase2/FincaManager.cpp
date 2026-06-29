#include "FincaManager.h"

bool FincaManager::agregarFinca(const Finca& f) {
    if (existeFinca(f.codigo))
        return false;

    fincas.push_back(f);
    return true;
}

bool FincaManager::existeFinca(const std::string& codigo) const {
    for (const auto& f : fincas)
        if (f.codigo==codigo)
            return true;

    return false;
}

Finca* FincaManager::buscarFinca(const std::string& codigo) {
    for (auto& f : fincas)
        if (f.codigo==codigo)
            return &f;

    return nullptr;
}

std::vector<Finca> FincaManager::listarTodas() const {
    return fincas;
}
