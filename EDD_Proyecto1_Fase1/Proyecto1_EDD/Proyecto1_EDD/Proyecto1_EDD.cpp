#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>

#include "ListaDoble.h"   
#include "Cola.h"         
#include "Pila.h"
#include "ListaPilas.h"

using namespace std;

ListaDoble inventario;
Cola colaDespulpado;
Pila bitacoraAdmin;
ListaPilas fincas;

int main() {
    int rol;
    cout <<"Bienvenido a EDD CoffeeTrack\n";
    cout << "1. Administrador\n";
    cout << "2. Usuario (operario/planta)\n";
    cout << "Seleccione rol: ";
    cin >> rol;
    if (rol==1) {
        menuAdmin();
    }
    else if (rol == 2) {
        if (login()){
            menuUsuario();
        }
        else {
            cout << "Credencales incorrectas.\n";
        }
    }
    else {
        cout <<"Rol invalido.\n";
    }
    return 0;
}

// MENUS
void menuAdmin() {
    int opciones;
    do {
        cout << "\n===== MENU ADMINISTRADOR =====\n";
        cout << "1. Registrar nuevo lote manual\n";
        cout << "2. Carga masiva de loes (CSV)\n";
        cout << "3. Gestionar fincas productoras\n";
        cout << "4. Registrar entrega de sacos por finca\n";
        cout << "5. Procesar lotes en cola de despulpado\n";
        cout << "6. Visualizar invntario completo\n";
        cout << "7. Consultar historial de transacciones (bitacora)\n";
        cout << "8. Salir\n";
        cout << "Opcion: ";
        cin >> opciones;

        switch (opciones) {
        case 1: registrarLoteManual();
            break;

        case 2: {
            string archivo;
            cout << "Nombre del archvo CSV: ";
            cin>>archivo;
            cargaMasivaCSV(archivo);
            break;
        }
        case 3: gestionarFincas(); 
            break;

        case 4: registrarEntregaFinca();
            break;

        case 5: procesarCola();
            break;

        case 6: visualizarInventario();
            break;

        case 7: consultarBitacora();
            break;

        case 8: cout <<"Saliendo...\n"; 
            break;

        default: cout<<"Opcion invalida, prueba con otra.\n";
        }
    } 
    while (opciones !=8);
}

void menuUsuario() {
    int op;
    do {
        cout << "\n===== MENU OPERARIO =====\n";
        cout << "1. Consultar disponibiidad de lotes\n";
        cout << "2. Enviar lote a cla de procesamiento\n";
        cout << "3. Visualizar hisorial de lotes procesados\n";
        cout << "4. Cerrar sesion\n";
        cout << "Opcion: ";
        cin >>op;

        switch (op) {
        case 1:
            consultarDisponibilidad();
            break;

        case 2:
            enviarLoteACola(); 
            break;

        case 3:
            verHistorialProcesados();
            break;

        case 4: 
            cout << "Sesion cerrada.\n";
            break;

        default: cout << "Opcion invalida.\n";
        }
    } 
    while (op!= 4);
}

string ahora() {
    time_t t=time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
    return string(buf);
}

void generarTodosReportes() {
    inventario.generarReporte("reporte_inventario");
    colaDespulpado.generarReporte("reporte_cola");
    bitacoraAdmin.generarReporte("reporte_bitacora");
    fincas.generarReporte("reporte_fincas");
}


void logAccion(string tipo, string detalles) {
    Registro r;
    r.fechaHora =ahora();
    r.tipoAccion=tipo;
    r.detalles=detalles;
    bitacoraAdmin.push(r);
    generarTodosReportes();
}

void cargaMasivaCSV(string archivo) {
    ifstream file(archivo);
    if (!file.is_open()) {
        cout <<"No se pudo abrir el archivo.\n";
        return;
    }
    string linea;
    getline(file, linea); 
    while (getline(file, linea)) {
        stringstream ss(linea);
        Lote l;
        getline(ss, l.codigo,',');
        getline(ss, l.finca,',');
        getline(ss, l.origen, ',');
        string cant; getline(ss, cant, ','); l.cantidad=stoi(cant);
        getline(ss, l.fechaRecepcion, ',');
        string min; getline(ss, min,','); l.nivelMinimo= stoi(min);
        inventario.insertarOrdenado(l);
    }
    file.close();
    logAccion("Carga masiva","Se cargaron lotes dsde " + archivo);
}

void registrarLoteManual() {
    Lote l;
    cout << "Codigo: ";cin >> l.codigo;
    cout << "Finca: ";cin >> l.finca;
    cout << "Origen:"; cin >> l.origen;
    cout << "Cantidad: "; cin >> l.cantidad;
    cout << "Fecha recepcion (YYYY-MM-DD): ";cin >> l.fechaRecepcion;
    cout << "Nivel minimo reorden: "; cin>>l.nivelMinimo;
    inventario.insertarOrdenado(l);
    logAccion("Registro lote","Lote " +l.codigo + " registrado");
    cout << "¿Encolar para despulpado? (s/n): ";
    char op;cin >>op;
    if (op == 's'|| op=='S') {
        colaDespulpado.enqueue(l);
        logAccion("Envio a cla","Lote " + l.codigo + " encolado");
    }
}



