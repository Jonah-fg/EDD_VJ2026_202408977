#ifndef FINCA_MANAGER_H
#define FINCA_MANAGER_H

#include <string>
#include <vector>

struct Finca {
    std::string codigo;
    std::string nombre;
    std::string region;
    std::string propietario;
};

class FincaManager {
private:
    std::vector<Finca> fincas;

public:
    bool agregarFinca(const Finca& f);
    bool existeFinca(const std::string& codigo) const;
    Finca* buscarFinca(const std::string& codigo);
    std::vector<Finca> listarTodas() const;
};

#endif