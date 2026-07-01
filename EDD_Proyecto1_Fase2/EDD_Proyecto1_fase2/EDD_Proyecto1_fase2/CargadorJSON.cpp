#include "CargadorJSON.h"
#include "ArbolAVL.h"
#include "hash_utils.h" 
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

// Funciones auxiliares de parseo 
static string leerArchivo(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo JSON." << endl;
        return "";
    }
    stringstream buffer;
    buffer << archivo.rdbuf();
    archivo.close();
    return buffer.str();
}

static string obtenerValorString(const string& objeto, const string& clave) {
    string buscar = "\"" +clave+ "\"";
    size_t pos = objeto.find(buscar);
    if (pos == string::npos)
        return "";

    pos = objeto.find(":", pos);
    if (pos == string::npos)
        return "";

    pos = objeto.find("\"", pos);
    if (pos == string::npos)
        return "";

    size_t inicio = pos + 1;
    size_t fin = objeto.find("\"", inicio);
    if (fin == string::npos)
        return "";

    return objeto.substr(inicio, fin - inicio);
}

static int obtenerValorInt(const string& objeto, const string& clave) {
    string buscar = "\"" + clave + "\"";
    size_t pos = objeto.find(buscar);
    if (pos == string::npos)
        return 0;

    pos = objeto.find(":", pos);
    if (pos==string::npos)
        return 0;

    pos++;
    while (pos < objeto.size() && (objeto[pos] == ' ' || objeto[pos] == '\"'))
        pos++;

    string numero;
    while (pos < objeto.size() && objeto[pos] >= '0' && objeto[pos] <= '9') {
        numero += objeto[pos];
        pos++;
    }
    if (numero.empty())
        return 0;

    return stoi(numero);
}

static void cargarFincas(const string& contenido, FincaManager& fincas) {
    size_t posFincas = contenido.find("\"fincas\"");
    if (posFincas == string::npos) {
        cout << "No se encontro la seccion fincas." << endl;
        return;
    }
    size_t inicioArray = contenido.find("[", posFincas);
    size_t finArray = contenido.find("\"entregas\"");
    if (inicioArray == string::npos || finArray == string::npos) {
        cout << "No se pudo leer la secion fincas." << endl;
        return;
    }
    string seccion = contenido.substr(inicioArray, finArray - inicioArray);
    size_t pos = 0;
    int cargadas = 0;
    while (true) {
        size_t inicioObj = seccion.find("{", pos);
        if (inicioObj == string::npos)
            break;

        size_t finObj = seccion.find("}", inicioObj);
        if (finObj == string::npos)
            break;

        string objeto = seccion.substr(inicioObj, finObj - inicioObj + 1);
        string codigo = obtenerValorString(objeto, "codigo");
        string nombre = obtenerValorString(objeto, "nombre");
        string region = obtenerValorString(objeto, "region");
        string propietario = obtenerValorString(objeto, "propietario");
        if (!codigo.empty()) {
            Finca f;
            f.codigo = codigo;
            f.nombre = nombre;
            f.region = region;
            f.propietario = propietario;
            if (fincas.agregarFinca(f)) {
                cargadas++;
            }
        }
        pos = finObj + 1;
    }
    cout << "Fincas cargadas: " << cargadas << endl;
}

static void cargarEntregas(const string& contenido, FincaManager& fincas, ArbolB& arbol) {
    size_t posEntregas = contenido.find("\"entregas\"");
    if (posEntregas == string::npos) {
        cout << "No se encontro la seccion entregas." << endl;
        return;
    }
    size_t inicioArray = contenido.find("[", posEntregas);
    size_t finArray = contenido.find("]", inicioArray);
    if (inicioArray == string::npos || finArray == string::npos) {
        cout << "No se pudo leer la seccion entregas." << endl;
        return;
    }
    string seccion = contenido.substr(inicioArray, finArray - inicioArray + 1);
    size_t pos = 0;
    int cargadas=0;
    int ignoradas =0;
    while (true) {
        size_t inicioObj=seccion.find("{", pos);
        if (inicioObj == string::npos)
            break;

        size_t finObj = seccion.find("}", inicioObj);
        if (finObj == string::npos)
            break;

        string objeto = seccion.substr(inicioObj, finObj - inicioObj + 1);
        string fechaHora = obtenerValorString(objeto, "fecha");
        string fechaArbol = fechaHora.substr(0, 10); // YYYY-MM-DD
        int codigoLoteInt = obtenerValorInt(objeto, "codigo_lote");
        string codigoLote = to_string(codigoLoteInt);
        string codigoFinca = obtenerValorString(objeto, "finca");
        int sacos = obtenerValorInt(objeto, "sacos");
        string tipoCafe = obtenerValorString(objeto, "tipo_cafe");
        string estado = obtenerValorString(objeto, "estado");
        if (estado.empty())
            estado = "recibido";

        // Validacion finca
        if (!fincas.existeFinca(codigoFinca)) {
            cout << "Entrega ignoada. Finca no existe: " << codigoFinca << endl;
            ignoradas++;
        }
        else {
            Finca* f = fincas.buscarFinca(codigoFinca);
            // Creacion lote
            Lote lote;
            lote.codigoLote = codigoLote;
            lote.codigoFinca = codigoFinca;
            lote.nombreFinca = f ? f->nombre : "";
            lote.sacos = sacos;
            lote.tipoCafe = tipoCafe;
            lote.estadoActual = estado;
            lote.rutaTomada = "N/A";
            lote.distanciaKm = 0.0;
            lote.fechaHoraEntrega = fechaHora;
            // Historial
            RegistroEstado reg;
            reg.timestamp = fechaHora;
            reg.estado = estado;
            lote.historialEstados.push_back(reg);

            ArbolAVL* avl = arbol.obtenerArbolAVL(fechaArbol);
            if (!avl) {
                avl = new ArbolAVL();
                avl->insertarLote(lote);
                arbol.insertarFecha(fechaArbol, avl);
            }
            else {
                avl->insertarLote(lote);
            }
            cargadas++;
        }
        pos = finObj + 1;
    }
    cout << "Enregas cargadas: " << cargadas << endl;
    cout << "Entregas inoradas: " << ignoradas << endl;
}

void CargaJSON::cargar(const string& ruta, FincaManager& fincas, ArbolB& arbol) {
    string contenido = leerArchivo(ruta);
    if (contenido.empty())
        return;

    cout << endl << "========== CARGA MASIVA JSON ==========" << endl;
    cargarFincas(contenido, fincas);
    cargarEntregas(contenido, fincas, arbol);
    cout << "Carga JSON finalzada." << endl;
}