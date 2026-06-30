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
#include "Utils.h"
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

string GeneradorCertificados::generarYGuardarCertificadoSinMerkle(Lote& lote) {
    asegurarCarpeta();

    string contenido = construirContenido(lote);
    string hashContenido =calcularHash(contenido);
    lote.hashCertificado =hashContenido;

    string hashNombre =calcularHash(lote.codigoLote);
    string contenidoEncriptado= Encryption::encriptar(contenido);
    string nombreArchivo =hashNombre + ".txt";
    string rutaCompleta=CARPETA_SALIDA + "/" + nombreArchivo;

    ofstream archivo(rutaCompleta, ios::binary);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo crear elarchivo: " + rutaCompleta);
    }
    archivo.write(contenidoEncriptado.data(), contenidoEncriptado.size());
    archivo.close();

    cout << "Certificado generao (sin Merkle): "<< nombreArchivo << "\n";
    return hashContenido;
}


void GeneradorCertificados::generarCertificadosMasivos(ArbolB& arbolB, const string& fecha, ArbolMerkle& merkle) {
    ArbolAVL* avl = arbolB.obtenerArbolAVL(fecha);
    if (!avl) {
        cout << "No hay enregas para la fecha " << fecha << endl;
        return;
    }

    vector<Lote> lotes=avl->listarTodosLosLotes();
    if (lotes.empty()) {
        cout << "No hay lotes en esa fecha." << endl;
        return;
    }

    // Generar certificado para cada lote (sin actualizar Merkle)
    for (auto& lote : lotes) {
        Lote* lotePtr = avl->buscarLote(lote.codigoLote);
        if (lotePtr) {
            // Avanzar estado a certificado_emitido si no lo está
            if (lotePtr->estadoActual != "certificado_emitido") {
                lotePtr->estadoActual = "certificado_emitido";
                RegistroEstado reg;
                reg.timestamp =obtenerTimestampActual();
                reg.estado = "certificado_emitido";
                lotePtr->historialEstados.push_back(reg);
            }
            generarYGuardarCertificadoSinMerkle(*lotePtr);
        }
    }
	// Reconstreuccion del Merkle una sola vez al final 
    reconstruirMerkleDesdeArbolB(arbolB, merkle);
    cout << "Certificaos masivos generados para la fecha " << fecha << endl;
}


void GeneradorCertificados::reconstruirMerkleDesdeArbolB(ArbolB& arbolB, ArbolMerkle& merkle) {
    vector<string> hashes;
    vector<string> fechas=arbolB.listarTodasLasFechas();
    for (const string& fecha : fechas) {
        ArbolAVL* avl=arbolB.obtenerArbolAVL(fecha);
        if (avl) {
            vector<Lote> lotes = avl->listarTodosLosLotes();
            for (const Lote& l : lotes) {
                if (!l.hashCertificado.empty()) {
                    hashes.push_back(l.hashCertificado);
                }
            }
        }
    }

    merkle.construir(hashes);
    cout << "Árbol de Merkle reconstruido con "<< hashes.size() <<" certificados." << endl;
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
