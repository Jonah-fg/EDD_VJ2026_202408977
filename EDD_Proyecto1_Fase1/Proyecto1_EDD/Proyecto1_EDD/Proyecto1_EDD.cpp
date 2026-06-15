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
    Registro reg;
    reg.fechaHora =ahora();
    reg.tipoAccion=tipo;
    reg.detalles=detalles;
    bitacoraAdmin.push(reg);
    generarTodosReportes();
}

void cargaMasivaCSV(string archivo) {
    ifstream file(archivo);
    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo.\n";
        return;
    }
    string linea;
    getline(file, linea);
    int totalLeidos = 0;
    int totalInsertados= 0;
    while (getline(file, linea)) {
        stringstream ss(linea);
        Lote l;
        getline(ss, l.codigo, ',');
        getline(ss, l.finca, ',');
        getline(ss, l.origen, ',');
        string cant; getline(ss, cant, ','); l.cantidad=stoi(cant);
        getline(ss,l.fechaRecepcion, ',');
        string min; getline(ss, min,','); l.nivelMinimo= stoi(min);
        totalLeidos++;

        if (!fincas.existeFinca(l.finca)) {
            cout << "Advertencia: Finca '"<< l.finca << "' no regisrada. Lote " << l.codigo << " omitido.\n";
            continue;
        }
        inventario.insertarOrdenado(l);
         totalInsertados++;
    }
    file.close();
    logAccion("Carga masiva", "Se cargaron " + to_string(totalInsertados) + " lotes desde " + archivo);
    cout << "Carga compltada. Insertados: " <<totalInsertados<< ", omitidos: " << (totalLeidos-totalInsertados)<< "\n";
}

void registrarLoteManual() {
    Lote l;
    cout << "Codigo: ";cin >> l.codigo;
    cout <<"Finca: ";cin >> l.finca;
    if (!fincas.existeFinca(l.finca)) {
        cout << "Error: La finca '" << l.finca << "' no está reistrada. Use la opción 3 prmero.\n";
        return;
    }
    cout << "Origen:"; cin >> l.origen;
    cout <<"Cantidad: "; cin >> l.cantidad;
    cout << "Fecha recepcion (YYYY-MM-DD): ";cin >> l.fechaRecepcion;
    cout << "Nivel minimo reorden: "; cin>>l.nivelMinimo;
    inventario.insertarOrdenado(l);
    logAccion("Registro lote","Lote " +l.codigo + " registrado");
    cout << "¿Encolar para despulpado? (s/n): ";
    char op;cin >>op;
    if (op =='s'|| op=='S') {
        colaDespulpado.enqueue(l);
        logAccion("Envio a cla","Lote " +l.codigo+ " encolado");
    }
}

// Gestionar fincas (solo agregar)
void gestionarFincas() {
    string nombre;
    cout <<"Nombre de la nuva finca: ";
    cin >>nombre;
    if (fincas.existeFinca(nombre)) {
        cout << "La finca ya existe.\n";
        return;
    }
    fincas.agregarFinca(nombre);
    logAccion("Gestion finca", "Se agrego la finca " +nombre);
    cout <<"Finca registrda.\n";
}

// Registrar entrega de sacos por finca
void registrarEntregaFinca() {
    string nombre;
    string fecha;
    int cantidad;
	cout << "Nombre de la inca: ";cin >> nombre;
    if (!fincas.existeFinca(nombre)) {
        cout << "La finca no exite. Regístrela primero (opción 3).\n";
        return;
    }

    cout << "Fecha de entrega (YYYY-MM-DD): "; cin >> fecha;
    cout << "Cantidad de sacos a retirar: "; cin >> cantidad;

    Lote loteExistente;
    if (!inventario.buscarPorFinca(nombre, loteExistente)) {
        cout << "No hay loes de la finca '" <<nombre<< "' en inventario. No se puede retirar.\n";
        return;
    }

    if (loteExistente.cantidad<cantidad) {
        cout << "Cantidad insufciente. El lote " << loteExistente.codigo << "solo tiene" << loteExistente.cantidad << " sacos.\n";
        return;
    }
    int nuevaCantidad=loteExistente.cantidad-cantidad;
    inventario.actualizarCantidad(loteExistente.codigo, nuevaCantidad);

    Entrega entrega;
    entrega.fecha=fecha;
    entrega.cantidad= cantidad;
    fincas.registrarEntrega(nombre, entrega);

    logAccion("Registro entrega", "Finca " + nombre + " retiró " + to_string(cantidad) + " sacos del lote " + loteExistente.codigo + ". Nuevo stock: " + to_string(nuevaCantidad));
    cout << "Entrega reistrada. Se retiraron " << cantidad << " sacos del lote " << loteExistente.codigo << ". Stock actual: " << nuevaCantidad << "\n";
}

// Procesar lote de la cola de despulpado
void procesarCola() {
    if (colaDespulpado.vacia()) {
        cout << "No hay lotes en cola.\n";
        return;
    }
    Lote l=colaDespulpado.dequeue();
    cout << "Procesando lote: "<< l.codigo << " de finca " << l.finca << " con " << l.cantidad << " de sacos.\n";
    logAccion("Procesar lote", "Lote " + l.codigo+" procesado en desplpado");
}

void visualizarInventario() {
    inventario.mostrar();
}

void consultarBitacora() {
    cout <<"\n---- BITACORA (mas reciente primero) ----\n";
    bitacoraAdmin.mostrar();
}

bool login() {
    string usuario;
    string contrasenia;
    cout << "Usuario: "; cin >> usuario;
    cout << "Contraseña: "; cin >> contrasenia;
    return (usuario=="operario" && contrasenia=="123") || (usuario =="planta" && contrasenia=="456");
}

void consultarDisponibilidad() {
    int op;
    cout <<"Buscar por: 1. Codigo  2. Finca\n Opcion: ";
    cin >>op;
    if (op==1) {
        string cod;
        cout<< "Codigo: "; cin >>cod;
        Lote l;
        if (inventario.buscarPorCodigo(cod, l)) {
            cout << "Lote: "<< l.codigo << ", Finca: " << l.finca << ", Cantidad: " << l.cantidad << ", Estado: En bodega\n";
        }
        else {
            cout << "Lote no encontrado.\n";
        }
    }
    else if (op ==2) {
        string fin;
        cout << "Finca: "; cin >>fin;
        Lote l;
        if (inventario.buscarPorFinca(fin, l)) {
            cout << "Lote: " << l.codigo << ", Cantidad: "<< l.cantidad << "\n";
        }
        else {
            cout << "No hay lotes de esa finca.\n";
        }
    }
    else {
        cout << "Opcion invalida, ingrese otra.\n";
    }
}

void enviarLoteACola() {
    string codigo;
    cout <<"Codigo del lote a enviar a despulpado: ";
    cin >> codigo;
    Lote l;
    if (inventario.buscarPorCodigo(codigo, l)) {
        if (l.cantidad >0) {
            colaDespulpado.enqueue(l);
            logAccion("Envio a cola", "Usuario envio lote " +codigo + "a despulpado");
            cout << "Lote encolado correctamente.\n";
        }
        else {
            cout << "El lote no tiene stock disponible.\n";
        }
    }
    else {
        cout << "Lote no existe.\n";
    }
}

void verHistorialProcesados() {
    cout << "\n----- HISTORIAL DE LOTES PROCESADOS (desde bitacora) -----\n";
    bitacoraAdmin.mostrar();
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
        case 1:
            registrarLoteManual();
            break;

        case 2: {
            string archivo;
            cout << "Nombre del archvo CSV: ";
            cin >> archivo;
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

        case 8: cout << "Saliendo...\n";
            break;

        default: cout << "Opcion invalida, prueba con otra.\n";
        }
    } while (opciones != 8);
}

void menuUsuario() {
    int op;
    do {
        cout << "\n======= MENU OPERARIO =======\n";
        cout << "1. Consultar disponiiidad de lotes\n";
        cout << "2. Enviar lote a cla de procesamiento\n";
        cout << "3. Visualizar hisorial de lotes procesados\n";
        cout << "4. Cerrar sesion\n";
        cout << "Opcion: ";
        cin >> op;

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

        default: cout << "Opcion invalda.\n";
        }
    }
    while (op!= 4);
}

int main() {
    int rol;
    do {
        cout << "Bienvenido a EDD CofeTrack\n";
        cout << "1. Administrador\n";
        cout << "2. Usuario (operario/planta)\n";
        cout << "3. Salir del programa\n";
        cout << "Seleccione rol: ";
        cin >> rol;

        if (rol ==1) {
            menuAdmin();
        }
        else if (rol== 2) {
            if (login()) {
                menuUsuario();
            }
            else {
                cout <<"Credenciales incorretas.\n";
            }
        }
        else if (rol== 3) {
            cout << "Saliendo del programa...\n";
            break;
        }
        else{
            cout << "Rol invalido, ingrese uno valido.\n";
        }
    } 
    while (rol!=3);
    return 0;
}


