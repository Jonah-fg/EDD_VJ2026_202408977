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
    cout << "Bienvenido a EDD CoffeeTrack\n";
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

        default: cout <<"Opcion invalida, prueba con otra.\n";
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

