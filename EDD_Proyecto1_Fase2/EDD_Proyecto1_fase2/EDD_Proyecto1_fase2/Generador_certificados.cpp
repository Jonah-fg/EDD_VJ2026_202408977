#include "Generador_certificados.h"
#include "hash_utils.h"
#include "Encryption.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#endif
using namespace std;

const string GeneradorCertificados::CARPETA_SALIDA = "certificados";
void GeneradorCertificados::asegurarCarpeta() {
    struct stat info;
    if (stat(CARPETA_SALIDA.c_str(), &info) != 0) {
		// No existe y se crea la carpeta
#ifdef _WIN32
        _mkdir(CARPETA_SALIDA.c_str());
#else
        mkdir(CARPETA_SALIDA.c_str(), 0755);
#endif
        cout << "[INFO] Carpeta'"<< CARPETA_SALIDA << "/' creada.\n";
    }
}

string GeneradorCertificados::construirContenido(const Lote& lote) {
    stringstream ss;
    ss << "CERTIFICADO DE ENTREGA - EDD COFFEETRACK\n";
    ss << "=============================================\n";
    ss << "Fecha : "<< lote.fechaHoraEntrega.substr(0, 10) << "\n";
    ss << "Codigo lote : "<< lote.codigoLote << "\n";
    ss << "Finca : " << lote.nombreFinca << "\n";
    ss << "Region : " <<"No disponible" << "\n";
    ss << "Tipo de cafe : "<< lote.tipoCafe << "\n";
    ss << "Sacos : " <<lote.sacos << "\n";
    ss << "Ruta tomada : "<<lote.rutaTomada<< "\n";
    ss << "Distancia : " <<static_cast<int>(lote.distanciaKm)<< " km\n";
    ss << "Estado final : "<<lote.estadoActual<< "\n";
    ss << "=============================================\n";
    ss << "Hash de contenido: "<<calcularHash(ss.str()) << "\n";
    return ss.str();
}

string GeneradorCertificados::generarYGuardarCertificado(Lote& lote, ArbolMerkle& merkle) {
    asegurarCarpeta();

    string contenido=construirContenido(lote);
    string hashContenido =calcularHash(contenido);
    lote.hashCertificado = hashContenido;

    string hashNombre=calcularHash(lote.codigoLote);
    string contenidoEncriptado=Encryption::encriptar(contenido);
    string nombreArchivo =hashNombre +".txt";
    string rutaCompleta =CARPETA_SALIDA + "/"+ nombreArchivo;

    ofstream archivo(rutaCompleta, ios::binary);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo crear el archivo: " +rutaCompleta);
    }
    archivo.write(contenidoEncriptado.data(), contenidoEncriptado.size());
    archivo.close();

    merkle.agregarHoja(hashContenido);
    cout <<"Certificado generado:" << nombreArchivo << "\n";
    return hashContenido;
}

string GeneradorCertificados::leerYDesencriptarCertificado(const string& nombreArchivo) {
    string rutaCompleta=CARPETA_SALIDA +"/" + nombreArchivo;
    ifstream archivo(rutaCompleta, ios::binary);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo: " + rutaCompleta);
    }
    string contenidoEncriptado((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
    archivo.close();
    return Encryption::desencriptar(contenidoEncriptado);
}
