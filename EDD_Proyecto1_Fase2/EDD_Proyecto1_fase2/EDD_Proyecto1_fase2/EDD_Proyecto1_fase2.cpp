#include <iostream>
#include <vector>
#include <string>
#include "ArbolAVL.h"
#include "Arbol_b.h"
#include "hash_utils.h"
#include "Encryption.h"

using namespace std;

void separador() {
    cout<<"\n========================================\n";
}

int main() {
    cout << "===== PRUEBAS DEL SISTEMA EDD COFFEETRACK =====\n";

    // PRUEBA: HASH_UTILS
    separador();
    cout << "PRUEBA 1: HASH_UTILS\n";

    string texto1 ="L001";
    string texto2="L002";
    string hash1 =calcularHash(texto1);
    string hash2=calcularHash(texto2);

    cout << "Hash de '" << texto1 << "': " <<hash1 << "\n";
    cout << "Hash de '" << texto2 << "': " <<hash2 << "\n";
    cout << "Longitud del hash: " <<hash1.length()<< "(debe ser 32)\n";

    string hash1_bis=calcularHash(texto1);
    cout <<"¿Determinista? "<< (hash1 == hash1_bis ? "SÍ" : "NO") <<"\n";


 //PRUEBA: ÁRBOL AVL (solo)
    separador();
    cout << "PRUEBA: ÁRBOL AVL (inserción, búsqueda, listado, cambio de estado)\n";

    ArbolAVL avl;

    Lote l1;
    l1.codigoLote = "L003";
    l1.codigoFinca ="F001";
    l1.nombreFinca ="Finca La Hermosa";
    l1.sacos = 45;
    l1.tipoCafe = "Bourbon";
    l1.estadoActual="recibido";
    l1.fechaHoraEntrega = "2026-06-15 10:00:00";

    Lote l2;
    l2.codigoLote = "L001";
    l2.codigoFinca = "F002";
    l2.nombreFinca ="Finca El Roble";
    l2.sacos = 30;
    l2.tipoCafe = "Caturra";
    l2.estadoActual= "recibido";
    l2.fechaHoraEntrega= "2026-06-15 11:00:00";

    Lote l3;
    l3.codigoLote = "L005";
    l3.codigoFinca= "F003";
    l3.nombreFinca="Finca Las Nubes";
    l3.sacos = 55;
    l3.tipoCafe = "Gesha";
    l3.estadoActual="recibido";
    l3.fechaHoraEntrega= "2026-06-20 09:00:00";

    avl.insertarLote(l1);
    avl.insertarLote(l2);
    avl.insertarLote(l3);

    cout << "Lotes insertados. Listado in-order (ordenado por código):\n";
    vector<Lote> lotes=avl.listarTodosLosLotes();
    for (const auto& l : lotes) {
        cout << "  "<< l.codigoLote << " | " <<l.nombreFinca << " | " << l.sacos << " sacos | Estado: " << l.estadoActual<<"\n";
    }

    // Buscar un lote
    Lote* encontrado=avl.buscarLote("L003");
    if (encontrado) {
        cout << "\nBúsqueda de L003: " << encontrado->nombreFinca << " (encontrado ✓)\n";
    }
    else {
        cout << "\nBúsqueda de L003: NO encontrado ✗\n";
    }

    // Avanzar estado de L001
    bool ok=avl.avanzarEstado("L001", "en_cola", "2026-06-15 12:00:00");
    if (ok){
        cout << "\nEstado de L001 cambiado a 'en_cola' ✓\n";
    }
    else{
        cout << "\nError al cambiar estado de L001 ✗\n";
    }

    // Mostrar historial de L001
    Lote* l001=avl.buscarLote("L001");
    if (l001){
        cout << "Historial de L001:\n";
        for (const auto& reg : l001->historialEstados) {
            cout << "  " <<reg.timestamp <<" -> "<< reg.estado << "\n";
        }
    }

    return 0;
}