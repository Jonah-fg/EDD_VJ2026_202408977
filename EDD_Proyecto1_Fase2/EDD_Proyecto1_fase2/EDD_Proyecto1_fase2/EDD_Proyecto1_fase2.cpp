#include <iostream>
#include <vector>
#include <string>
#include "ArbolAVL.h"
#include "Arbol_b.h"
#include "hash_utils.h"
#include "Encryption.h"
#include "Generador_certificados.h"

using namespace std;

void separador() {
    cout<<"\n========================================\n";
}

int main() {
    cout << "===== PRUEBAS DEL SISTEMA EDD COFFEETRACK =====\n";

    // PRUEBA: HASH_UTILS
    separador();
    cout << "PRUEBA: HASH_UTILS\n";

    string texto1 ="L001";
    string texto2="L002";
    string hash1 =calcularHash(texto1);
    string hash2=calcularHash(texto2);

    cout << "Hash de '" << texto1 << "': " <<hash1 << "\n";
    cout << "Hash de '" << texto2 << "': " <<hash2 << "\n";
    cout << "Longitud del hash: " <<hash1.length()<< "(debe ser 32)\n";

    string hash1_bis=calcularHash(texto1);
    cout <<"¿Determinista? "<< (hash1 == hash1_bis ? "SÍ" : "NO") <<"\n";

	//Prueba : ENCRYPTION
	separador();
	cout << "PRUEBA 2: ENCRYPTION (XOR)\n";
	string original="Este es un texto de prueba para encriptar.";
	string encriptado=Encryption::encriptar(original);
	string desencriptado= Encryption::desencriptar(encriptado);
	cout << "Texto original: "<< original << "\n";
	cout << "Texto encriptao: "<< encriptado << "\n";
	cout << "Texto desencriptado: " << desencriptado << "\n";
	cout << "¿Coincide con el original? " <<(original==desencriptado ? "SÍ" : "NO") << "\n"; 


    separador();
	//PRUEBA : ARBOL MERKLE
    cout << "--- ARBOL DE MERKLE ---\n";
    ArbolMerkle merkle;
    vector<string> hashes= {calcularHash("cert1"), calcularHash("cert2"), calcularHash("cert3")};

    merkle.construir(hashes);
    cout <<"Hojas: " << hashes.size()<< "\n";
    cout << "Raiz: " <<merkle.obtenerHashRaiz() << "\n";
    // Agregar una hoja más
    merkle.agregarHoja(calcularHash("cert4"));
    cout << "Despues de agregar cert4, raiz: "<< merkle.obtenerHashRaiz() << "\n";
    cout <<"Verificacion de intgrdad de 'cert3': "<<(merkle.verificarIntegridad(calcularHash("cert3")) ? "VALIDO" : "INVALIDO") << "\n\n";


    separador();
	//PRUEBA: Certificado y encriptación
    cout << "--- GENERADOR DE CERTIFICADOS ---\n";
    Lote lotePrueba;
    lotePrueba.codigoLote = "L001";
    lotePrueba.codigoFinca = "F001";
    lotePrueba.nombreFinca ="Finca La Hermosa";
    lotePrueba.sacos =45;
    lotePrueba.tipoCafe ="Bourbon";
    lotePrueba.estadoActual="recibido";
    lotePrueba.fechaHoraEntrega = "2026-06-15 10:00:00";
    lotePrueba.rutaTomada = "F001 -> BENEFICIO";
    lotePrueba.distanciaKm =127.0;
    lotePrueba.hashCertificado = "";

    try {
        // Necesitamos un objeto ArbolMerkle para pasar a la función (lo reutilizamos)
        string hashGenerado = GeneradorCertificados::generarYGuardarCertificado(lotePrueba, merkle);
        cout << "Certificado generado exitosamente.\n";
        cout << "Hash de cntenido: "<< hashGenerado << "\n";
        cout << "Hash guardado en el lote: " << lotePrueba.hashCertificado << "\n";
        cout << "Estado del lote: " << lotePrueba.estadoActual << "\n";

        cout << "Hojas en Merkle despus del certificado: " << merkle.cantidadHojas() << "\n";

    }
    catch (const exception& e) {
        cout << "Error al genrar certificado: "<< e.what()<< "\n";
    }


    //PRUEBA: ÁRBOL AVL (solo)
    separador();
    cout << "PRUEBA: ÁRBOL AVL (insecion, busqueda, listado, camio de estao)\n";

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
    l3.nombreFinca="Finca Las Nbes";
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

    Lote* encontrado=avl.buscarLote("L003");
    if (encontrado) {
        cout << "\nBúsqeda de L003: " << encontrado->nombreFinca << " (encontrado ✓)\n";
    }
    else {
        cout << "\nBúsqueda de L003: no encontrado ✗\n";
    }

    bool ok=avl.avanzarEstado("L001", "en_cola", "2026-06-15 12:00:00");
    if (ok){
        cout << "\nEstado de L001 cambiado a 'en_cola' ✓\n";
    }
    else{
        cout << "\nError al cambiar estado de L001 ✗\n";
    }

    return 0;
}