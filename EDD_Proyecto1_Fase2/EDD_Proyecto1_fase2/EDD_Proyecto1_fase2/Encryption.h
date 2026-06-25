#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

class Encryption {
public:
    // Aplicando XOR con una clave fija. 
    static std::string xorTransformar(const std::string& texto);

    static std::string encriptar(const std::string& texto) 
    { 
        return xorTransformar(texto); 
    }
    static std::string desencriptar(const std::string& texto) { 

        return xorTransformar(texto); 
    }
};

#endif
