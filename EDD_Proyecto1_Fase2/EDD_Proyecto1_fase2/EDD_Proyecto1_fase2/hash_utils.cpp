#include "hash_utils.h"
#include <sstream>
#include <iomanip>

using namespace std;

// Función de avalancha (tomada del auxiliar)
static uint64_t mezclarBits(uint64_t hash) {
    hash ^= hash>>33;
    hash *= 0xff51afd7ed558ccdULL;
    hash ^= hash >> 33;
    hash *= 0xc4ceb9fe1a85ec53ULL;
    hash ^= hash >> 33;
    return hash;
}

// Algoritmo FNV-1a de 64 bits con avalancha al final
static uint64_t fnv1a_64_avalancha(const string& entrada) {
    uint64_t hash=14695981039346656037ULL;
    const uint64_t primo= 1099511628211ULL;
    for (char c : entrada) {
        hash^= static_cast<uint64_t>(c);
        hash*=primo;
    }
    return mezclarBits(hash);
}

string calcularHash(const string& entrada) {
    uint64_t parte1= fnv1a_64_avalancha(entrada);
    uint64_t parte2=fnv1a_64_avalancha(entrada + "EDD_CoffeeTrack_Salt_2026");

    stringstream resultado;
    resultado << hex << setw(16) << setfill('0') << parte1<< hex << setw(16) << setfill('0') << parte2;
    return resultado.str(); // 32 caracteres hexadecimales
}