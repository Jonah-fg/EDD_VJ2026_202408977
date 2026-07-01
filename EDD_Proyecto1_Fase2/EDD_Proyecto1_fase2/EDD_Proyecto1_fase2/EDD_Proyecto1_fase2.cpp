#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include <cstdlib>
#include "Arbol_b.h"
#include "ArbolAVL.h"
#include "Arbol_merkle.h"
#include "Generador_certificados.h"
#include "GeneradorReportes.h"
#include "FincaManager.h"
#include "CargadorJSON.h"      
#include "hash_utils.h"
#include "utils.h"
using namespace std;

// Variables globales
ArbolB arbolB;
ArbolMerkle arbolMerkle;
FincaManager fincaManager;
GeneradorReportes generadorReportes(arbolB, arbolMerkle, fincaManager);

// Prototipos de funciones del menú
void menuGestionDatos();
void menuConsultas();
void menuRutas();
void menuCertificados();
void menuMerkle();
void menuReportes();

void registrarEntregaManual();
void buscarPorFecha();
void buscarLote();
void listarFechas();
void trazabilidadLote();
void avanzarEstadoLote();
void registrarFinca();
void listarFincas();
void generarCertificadoIndividual();
void generarCertificadosMasivos();
void mostrarCertificado();
void construirMerkle();
void verificarCertificado();
void mostrarHashRaiz();

// Reportes Graphviz 
void reporteArbolB();
void reporteAVL();
void reporteMerkle();
void reporteTrazabilidad();


int main() {
    int opcion;

    do {
        cout << "\n===== EDD COFFEETRACK – FASE 2 =====\n";
        cout << "MENÚ PRINCIPAL – ADMINISTRADOR\n";
        cout << "1. Gestión de Datos\n";
        cout << "2. Consltas y Trazabilidad\n";
        cout << "3. Rutas\n";
        cout << "4. Certificados\n";
        cout << "5. Árbol de Merkle\n";
        cout << "6. Reportes Graphviz\n";
        cout << "0. Salir\n";
        cout << "Seleccione: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
        case 1: menuGestionDatos();
            break;

        case 2: menuConsultas(); 
            break;

        case 3: menuRutas();
            break;

        case 4: menuCertificados();
            break;

        case 5: menuMerkle();
            break;

        case 6: menuReportes();
            break;

        case 0:
            cout << "Saliendo del programa :)\n";
            break;
        default:
            cout << "Opción inválida, introduzca uno valido.\n";
        }
    } while (opcion!= 0);

    return 0;
}


// Submenus
void menuGestionDatos() {
    int sub;
    cout << "\n--- Gestión de Datos ---\n";
    cout << "1. Carga masiva desde JSON\n";
    cout << "2. Registrar entrega manualmente\n";
    cout << "Seleccione: ";
    cin >> sub;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (sub==1) {
        string archivo;
        cout << "Ingrese ruta del archivo JSON: ";
        getline(cin, archivo);
        CargaJSON::cargar(archivo, fincaManager, arbolB);
    }
    else if (sub==2) {
        registrarEntregaManual();
    }
    else {
        cout << "Opción inválida.\n";
    }
}

void menuConsultas() {
    int sub;
    cout << "\n--__- Consultas y Trazabilidad --__-\n";
    cout << "2.1 Buscar entregas por fecha\n";
    cout << "2.2 Buscar lote específico por código\n";
    cout << "2.3 Listar todas las fechas registradas\n";
    cout << "2.4 Trazabilidad completa de un lote\n";
    cout << "2.5 Avanzar estado de un lote\n";
    cout << "Seleccione: ";
    cin >> sub;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (sub) {
    case 1: buscarPorFecha();
        break;

    case 2: buscarLote();
        break;

    case 3: listarFechas();
        break;

    case 4: trazabilidadLote();
        break;

    case 5: avanzarEstadoLote();
        break;

    default: cout << "Opción inválida.\n";
    }
}

void menuRutas() {
    int sub;
    cout << "\n--__- Rutas -__--\n";
    cout << "3.1 Registrar nueva finca y sus conexiones\n";
    cout << "3.2 Calcular ruta óptima desde una finca al beneficio (Dijkstra)\n";
    cout << "3.3 Ver todas las fincas y conexiones registradas\n";
    cout << "Seleccione: ";
    cin >> sub;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (sub) {
    case 1: registrarFinca(); 
        break;

    case 2:
        cout <<"Opción no implementada (Dijkstra omitido).\n";
        break;
    case 3: listarFincas(); 
        break;

    default: cout<< "Opción inválida.\n";
    }
}

void menuCertificados() {
    int sub;
    cout << "\n--- Certificados ---\n";
    cout << "4.1 Generar certificado encriptado de un lote\n";
    cout << "4.2 Generar certificados masivos de una fecha completa\n";
    cout << "4.3 Mostrar un certificado desencriptado en específico\n";
    cout << "Seleccione: ";
    cin >> sub;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (sub) {
    case 1: generarCertificadoIndividual();
        break;

    case 2: generarCertificadosMasivos();
        break;

    case 3: mostrarCertificado(); 
        break;

    default: cout << "Opción inválida.\n";
    }
}

void menuMerkle() {
    int sub;
    cout << "\n--- Árbol de Merkle ---\n";
    cout << "5.1 Construir / actualizar árbol con certificados actuales\n";
    cout << "5.2 Verificar si un certificado fue alterado\n";
    cout << "5.3 Mostrar hash raíz actual\n";
    cout << "Seleccione: ";
    cin >> sub;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (sub) {
    case 1: construirMerkle();
        break;

    case 2: verificarCertificado();
        break;

    case 3: mostrarHashRaiz(); 
        break;

    default: cout << "Opción inválida.\n";
    }
}

void menuReportes() {
    int sub;
    cout << "\n----- Reportes Graphviz -----\n";
    cout << "6.1 Graficar Árbol B\n";
    cout << "6.2 Graficar AVL de una fecha específica\n";
    cout << "6.3 Graficar grao de rutas (no implementado)\n";
    cout << "6.4 Graficar Árbol de Merkle\n";
    cout << "6.5 Graficar cadena de trazabilidad de un lote\n";
    cout << "Seleccione: ";
    cin >> sub;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (sub) {
    case 1: reporteArbolB(); 
        break;

    case 2: reporteAVL();
        break;

    case 3: cout << "No implementado (grafo de rutas omitido).\n";
        break;

    case 4: reporteMerkle();
        break;

    case 5: reporteTrazabilidad(); 
        break;

    default: cout << "Opción inválida.\n";
    }
}


// Funciones del menu
// ------------- 1.2 Registrar entrega manual ----------
void registrarEntregaManual() {
    string fecha, codigoLote, fincaCod, tipoCafe;
    int sacos;

    cout <<"Fecha (YYYY-MM-DD HH:MM:SS): ";
    getline(cin, fecha);
    cout<< "Código de lote: ";
    getline(cin, codigoLote);
    cout <<"Código de finca: ";
    getline(cin, fincaCod);
    cout <<"Sacos: ";
    cin >> sacos;
    cin.ignore();
    cout << "Tipo de café: ";
    getline(cin, tipoCafe);

    if (!fincaManager.existeFinca(fincaCod)) {
        cout << "Error: finca no registrada.\n";
        return;
    }

    Finca* f=fincaManager.buscarFinca(fincaCod);
    Lote lote;
    lote.codigoLote=codigoLote;
    lote.codigoFinca= fincaCod;
    lote.nombreFinca =f ? f->nombre : "";
    lote.sacos=sacos;
    lote.tipoCafe=tipoCafe;
    lote.estadoActual = "recibido";
    lote.rutaTomada ="N/A";
    lote.distanciaKm= 0.0;
    lote.fechaHoraEntrega = fecha;

    RegistroEstado reg;
    reg.timestamp=fecha;
    reg.estado ="recibido";
    lote.historialEstados.push_back(reg);

    string fechaSolo=fecha.substr(0, 10);
    ArbolAVL* avl=arbolB.obtenerArbolAVL(fechaSolo);
    if (!avl) {
        avl = new ArbolAVL();
        avl->insertarLote(lote);
        arbolB.insertarFecha(fechaSolo, avl);
    }
    else {
        avl->insertarLote(lote);
    }
    cout << "Entrega registrada exitoamente.\n";
}

// ------------- 2.1 Buscar por fecha --------------
void buscarPorFecha() {
    string fecha;
    cout << "Inrese fecha (YYYY-MM-DD): ";
    getline(cin, fecha);
    ArbolAVL* avl =arbolB.obtenerArbolAVL(fecha);
    if (!avl) {
        cout << "No hay enregas en esa fecha.\n";
        return;
    }
    vector<Lote> lotes=avl->listarTodosLosLotes();
    cout << "Lotes del " << fecha << ":\n";
    for (const auto& l : lotes) {
        cout << " Código: "<< l.codigoLote << ", Finca: "<< l.nombreFinca<< ", Sacos: "<< l.sacos<< ", Estado: " << l.estadoActual << "\n";
    }
}

// ---------------- 2.2 Buscar lote por código --------------
void buscarLote() {
    string codigo;
    cout <<"Ingrese código de lote: ";
    getline(cin, codigo);
    vector<string> fechas=arbolB.listarTodasLasFechas();
    for (const string& f : fechas){
        ArbolAVL* avl= arbolB.obtenerArbolAVL(f);
        if (avl) {
            Lote* l=avl->buscarLote(codigo);
            if (l){
                cout <<"Lote encontrado en fecha " << f << "\n";
                cout <<"  Finca: "<< l->nombreFinca << ", Sacos: " << l->sacos<< ", Estado: " << l->estadoActual << "\n";
                return;
            }
        }
    }
    cout << "Lte no encontrado.\n";
}

// --------------2.3 Listar fechas -------------
void listarFechas() {
    vector<string> fechas=arbolB.listarTodasLasFechas();
    if (fechas.empty()){
        cout<<"No hay fechas registradas.\n";
        return;
    }
    cout <<"Fechas registradas:\n";
    for (const auto& f: fechas) {
        cout<<"  "<< f << "\n";
    }
}

// ------------ 2.4 Trazabilidad completa -----------
void trazabilidadLote() {
    string codigo;
    cout << "Ingrese cdigo de lote: ";
    getline(cin, codigo);
    vector<string> fechas =arbolB.listarTodasLasFechas();
    for (const string& f : fechas) {
        ArbolAVL* avl=arbolB.obtenerArbolAVL(f);
        if (avl) {
            Lote* l = avl->buscarLote(codigo);
            if (l) {
                cout << "\n=== TRAZABILIDAD – LOTE "<< codigo << "===\n";
                cout << "Finca de origen: "<< l->nombreFinca << "("<< l->codigoFinca << ")\n";
                cout <<"Tipo de café: " <<l->tipoCafe<< "\n";
                cout << "Sacos: " << l->sacos <<"\n";
                cout << "Historial de estados:\n";
                for (const auto& reg : l->historialEstados) {
                    cout<<"  [" << reg.timestamp << "] "<< reg.estado<< "\n";
                }

                // Información del certificado y Merkl
                if (!l->hashCertificado.empty()) {
                    cout<< "Certificado: "<< l->hashCertificado <<".txt\n";

                    // Obtener posición en Merkle
                    int pos=arbolMerkle.obtenerPosicionHoja(l->hashCertificado);
                    string raiz= arbolMerkle.obtenerHashRaiz();
                    if (pos != -1) {
                        cout << "Posición en Merkle: hoja #" <<(pos + 1)<< " de " << arbolMerkle.obtenerNumeroHojas() << " / Hash raíz: " << raiz << "\n";
                    }
                    else {
                        cout << "Advertecia: certificado no encontrado en el árbol de Mrkle.\n";
                    }
                }
                else {
                    cout << "Certificado: No emitido\n";
                }

                cout << "Ruta tomada: "<< l->rutaTomada << "\n";
                cout <<"Distancia: "<< l->distanciaKm << " km\n";
                return;
            }
        }
    }
    cout << "Loe no encontrado.\n";
}


// --------- 2.5 Avanzar estado -----------
void avanzarEstadoLote() {
    string codigo, nuevoEstado;
    cout << "Ingrese código de lote: ";
    getline(cin, codigo);
    cout << "Ingrese nuevo estado (recibido, en_cola, procesado, en_bodega, certificado_emitido): ";
    getline(cin, nuevoEstado);
    string timestamp= obtenerTimestampActual();
    vector<string> fechas= arbolB.listarTodasLasFechas();
    for (const string& f : fechas) {
        ArbolAVL* avl=arbolB.obtenerArbolAVL(f);
        if (avl){
            if (avl->avanzarEstado(codigo, nuevoEstado, timestamp)) {
                cout <<"Estado actualizado correctamente.\n";
                return;
            }
        }
    }
    cout << "No se pudo actualizar. Verifique código y secuencia de estados.\n";
}

// ------------ 3.1 Registrar finca (sin conexiones) -------------
void registrarFinca() {
    Finca f;
    cout << "Código de finca: ";
    getline(cin, f.codigo);
    if (fincaManager.existeFinca(f.codigo)) {
        cout <<"Error: finca ya existe.\n";
        return;
    }
    cout << "Nombre: ";
    getline(cin, f.nombre);
    cout << "Región: ";
    getline(cin, f.region);
    cout << "Propietario: ";
    getline(cin, f.propietario);
    fincaManager.agregarFinca(f);
    cout << "Finca registrada.\n";
}

// --------------- 3.3 Listar fincas --------------
void listarFincas() {
    vector<Finca> fincas=fincaManager.listarTodas();
    if (fincas.empty()) {
        cout <<"No hay fincas registradas.\n";
        return;
    }
    cout << "Fincas registradas:\n";
    for (const auto& f : fincas) {
        cout << "  " << f.codigo << " - " << f.nombre << " (" << f.region << ")\n";
    }
}

// ------------ 4.1 Generar certificado individual ----------------
void generarCertificadoIndividual() {
    string codigo;
    cout << "Ingrese código de lote: ";
    getline(cin, codigo);
    vector<string> fechas= arbolB.listarTodasLasFechas();
    for (const string& f : fechas) {
        ArbolAVL* avl=arbolB.obtenerArbolAVL(f);
        if (avl) {
            Lote* l = avl->buscarLote(codigo);
            if (l) {
                if (l->estadoActual != "certificado_emitido") {
                    l->estadoActual = "certificado_emitido";
                    RegistroEstado reg;
                    reg.timestamp = obtenerTimestampActual();
                    reg.estado = "certificado_emitido";
                    l->historialEstados.push_back(reg);
                }
                GeneradorCertificados::generarYGuardarCertificado(*l, arbolMerkle);
                cout << "Certificado generado.\n";
                return;
            }
        }
    }
    cout << "Lote no encontrado.\n";
}

// ------------ 4.2 Generar certificados masivos --------------
void generarCertificadosMasivos() {
    string fecha;
    cout << "Ingrese fecha (YYYY-MM-DD): ";
    getline(cin, fecha);
    GeneradorCertificados::generarCertificadosMasivos(arbolB, fecha, arbolMerkle);
}

// --- 4.3 Mostrar certificado desencriptado ---
void mostrarCertificado() {
    string nombre;
    cout <<"Ingrese nombre del archivo (ej. a3f9c2d1.txt): ";
    getline(cin, nombre);
    try {
        string contenido = GeneradorCertificados::leerYDesencriptarCertificado(nombre);
        cout << "\n=== CERTIFICADO DESENCRIPTADO ===\n";
        cout << contenido << "\n";
    }
    catch (const exception& e) {
        cout <<"Error: " <<e.what()<< "\n";
    }
}

// --------------- 5.1 Reconstruir Merkle ----------------
void construirMerkle(){
    GeneradorCertificados::reconstruirMerkleDesdeArbolB(arbolB, arbolMerkle);
    cout << "Árbol de Merkle actualizado.\n";
}

// ---------------- 5.2 Verificar integridad de certificado --------------
void verificarCertificado() {
    string nombreArchivo;
    cout << "Ingrese nombre del archivo de certificado (ej. hash.txt): ";
    getline(cin, nombreArchivo);
    try {
        string contenido = GeneradorCertificados::leerYDesencriptarCertificado(nombreArchivo);
        string hashContenido = calcularHash(contenido);
        if (arbolMerkle.verificarIntegridad(hashContenido)) {
            cout << "El certificado NO ha sido alterado (hash válido en el Merkle).\n";
        }
        else {
            cout << "ADVERTENCIA: El certificado ha sido alterado o no está registrado.\n";
        }
    }
    catch (const exception& e) {
        cout << "Error: " <<e.what() << "\n";
    }
}

// -------------- 5.3 Mostrar hash raíz --------------------
void mostrarHashRaiz() {
    string raiz = arbolMerkle.obtenerHashRaiz();
    if (raiz.empty()) {
        cout << "El árbol de Merkle esta vacío.\n";
    }
    else {
        cout <<"Hash raíz actual: " << raiz << "\n";
    }
}

//Reportes
void reporteArbolB() {
    generadorReportes.reporteArbolB("arbolB");
}

void reporteAVL() {
    string fecha;
    cout << "Ingrese fecha (YYYY-MM-DD): ";
    getline(cin, fecha);
    generadorReportes.reporteAVL(fecha, "avl");
}

void reporteMerkle() {
    generadorReportes.reporteMerkle("merkle");
}

void reporteTrazabilidad() {
    string codigo;
    cout << "Ingrese cdigo de lote: ";
    getline(cin, codigo);
    generadorReportes.reporteTrazabilidad(codigo, "trazabilidad");
}
