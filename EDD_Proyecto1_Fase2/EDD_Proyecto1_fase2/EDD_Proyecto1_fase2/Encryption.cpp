#include "encryption.h"
using namespace std;

static const string CLAVE= "CoffeeTrack2026EDD_Secret!";

string Encryption::xorTransformar(const string& texto) {
    string resultado= texto;
    size_t claveLen=CLAVE.length();
    for (size_t i=0; i< resultado.size(); ++i) {
        resultado[i]=resultado[i] ^ CLAVE[i %claveLen];
    }
    return resultado;
}

