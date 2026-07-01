#include "GeneradorReportes.h"
#include "hash_utils.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

GeneradorReportes::GeneradorReportes(ArbolB& b, ArbolMerkle& m, FincaManager& f)
    : arbolB(b), arbolMerkle(m), fincaManager(f) {
}

// MÉTODO PARA COMPILAR DOT A PNG
void GeneradorReportes::compilarDot(const string& archivoDot){
    string archivoPng=archivoDot;
    size_t pos=archivoPng.find(".dot");
    if (pos != string::npos) {
        archivoPng.replace(pos,4, ".png");
    }
    else{
        archivoPng+=".png";
    }
    string comando = "dot -Tpng " + archivoDot +" -o "+ archivoPng;
    int resultado = system(comando.c_str());
    if (resultado == 0) {
        cout <<"Reporte generado:"<< archivoPng << endl;
    }
    else {
        cout << "Error al generar el PNG. Asegurate de tener Graphviz instalado."<< endl;
    }
}


// 6.1 REPORTE DEL ÁRBOL B
void GeneradorReportes::generarNodoB(NodoB* nodo, ofstream& dot, int& id) {
    if (!nodo) return;
    int miId = id++;

    // Escribir el nodo con sus claves
    dot<< "  nodo"<< miId <<" [label=\"";
    for (int i =0; i<nodo->numeroClaves; i++) {
        if (i > 0) 
            dot<<" | ";

        dot <<"<f" <<i<< "> "<< nodo->fechas[i];
    }
    dot<< " | <f"<< nodo->numeroClaves<< "> \"];\n";

    // si es hoja, color 
    if (nodo->esHoja){
        dot<< "  nodo" << miId <<" [style=filled, fillcolor=lightblue];\n";
    }
    else {
        for (int i=0; i<=nodo->numeroClaves; i++){
            if (nodo->hijos[i]){
                generarNodoB(nodo->hijos[i], dot, id);
                dot << "  nodo"<< miId << ":f" <<i<< " -> nodo" << id- 1<< ";\n";
            }
        }
    }
}

void GeneradorReportes::reporteArbolB(const string& nombreArchivo) {
    string archivoDot = nombreArchivo + ".dot";
    ofstream dot(archivoDot);
    if (!dot.is_open()){
        cout << "Error: no se pudo crear " << archivoDot << endl;
        return;
    }

    dot <<"digraph \"ArbolB\" {\n";
    dot << "  node [shape=record];\n";
    dot << "  rankdir=TB;\n";

    NodoB* raiz = arbolB.getRaiz();
    if (!raiz) {
        dot << "  node [label=\"Árbol B vacío\"];\n";
    }
    else {
        int id= 0;
        generarNodoB(raiz,dot, id);
    }
    dot << "}\n";
    dot.close();
    compilarDot(archivoDot);
}


// 6.2 REPORTE DEL AVL DE UNA FECHA
void GeneradorReportes::generarNodoAVL(NodoAVL* nodo, ofstream& dot, int& id) {
    if (!nodo)
        return;

    int miId =id++;

    int alturaIzq = nodo->hijoIzquierdo ? nodo->hijoIzquierdo->altura : 0;
    int alturaDer = nodo->hijoDerecho ? nodo->hijoDerecho->altura : 0;
    int bf = alturaIzq - alturaDer;

    // Mostrar código, finca, sacos y BF
    dot << "  nodo" << miId << " [label=\""
        << nodo->lote.codigoLote << "\\n"
        << nodo->lote.nombreFinca << "\\n"
        << nodo->lote.sacos << " sacos\\n"
        << "BF: "<<bf
        << "\"];\n";

    // Color rojo si BF está fuera de rango
    if (bf<-1 || bf >1) {
        dot << "  nodo" << miId <<" [style=filled, fillcolor=red];\n";
    }

    if (nodo->hijoIzquierdo) {
        generarNodoAVL(nodo->hijoIzquierdo, dot, id);
        dot << "  nodo"<< miId << " -> nodo" << id - 1 << " [label=\"I\"];\n";
    }
    if (nodo->hijoDerecho) {
        generarNodoAVL(nodo->hijoDerecho, dot, id);
        dot << "  nodo"<< miId << " -> nodo" << id - 1 << " [label=\"D\"];\n";
    }
}


void GeneradorReportes::reporteAVL(const string& fecha, const string& nombreArchivo) {
    string archivoDot =nombreArchivo + "_" + fecha + ".dot";
    ofstream dot(archivoDot);
    if (!dot.is_open()) {
        cout << "Error: no se pudo crar "<< archivoDot << endl;
        return;
    }

    ArbolAVL* avl=arbolB.obtenerArbolAVL(fecha);
    dot << "digraph \"AVL_" << fecha << "\" {\n";
    dot << " node [shape=circle];\n";
    dot << " rankdir=TB;\n";

    if (!avl){
        dot << "  node [label=\"No hay lotes para " << fecha << "\"];\n";
    }
    else {
        NodoAVL* raiz=avl->getRaiz();
        if (!raiz) {
            dot << " node [label=\"AVL vacío\"];\n";
        }
        else {
            int id=0;
            generarNodoAVL(raiz, dot, id);
        }
    }
    dot <<"}\n";
    dot.close();

    compilarDot(archivoDot);
}


// 6.4 REPORTE DEL ÁRBOL DE MERKLE
int GeneradorReportes::generarNodoMerkle(NodoMerkle* nodo, ofstream& dot, int& id, bool esRaiz) {
    if (!nodo)
        return -1;

    int miId = id++;
    string hashMostrar = nodo->hash.substr(0, 8);
    dot << "  nodo"<< miId<< " [label=\"" <<hashMostrar<< "\"";

    if (esRaiz) {
        dot << ", color=blue, style=filled, fillcolor=lightblue";
    }
    else if (!nodo->izquierdo && !nodo->derecho) {
        dot << ", style=filled, fillcolor=lightgreen";
    }
    dot << "];\n";

    int idIzq =-1;
    int idDer =-1;
    if (nodo->izquierdo) {
        idIzq = generarNodoMerkle(nodo->izquierdo, dot, id, false);
        dot << "  nodo" << miId << " -> nodo" << idIzq << ";\n";
    }
    if (nodo->derecho) {
        idDer = generarNodoMerkle(nodo->derecho, dot, id, false);
        dot << "  nodo" << miId << " -> nodo" << idDer << ";\n";
    }
    return miId;
}


void GeneradorReportes::reporteMerkle(const string& nombreArchivo) {
    string archivoDot=nombreArchivo + ".dot";
    ofstream dot(archivoDot);
    if (!dot.is_open()) {
        cout << "Error: no se pudo cear " << archivoDot << endl;
        return;
    }

    dot << "digraph \"Merkle\" {\n";
    dot << "  node [shape=box];\n";
    dot << "  rankdir=TB;\n";
    dot << "  // Árbol de Merkle con " <<arbolMerkle.obtenerNumeroHojas()<< " certificados\n";

    NodoMerkle* raiz=arbolMerkle.getRaiz();
    if (!raiz) {
        dot << "  node [label=\"Árbol de Merkle vacío\"];\n";
    }
    else {
        int id=0;
        // Pasar true para marcar raíz
        generarNodoMerkle(raiz, dot, id, true);
        dot << "  info [label=\"Certificados: " << arbolMerkle.obtenerNumeroHojas() << "\", shape=plaintext, fontsize=16];\n";
    }
    dot << "}\n";
    dot.close();
    compilarDot(archivoDot);
}

// 6.5 REPORTE DE TRAZABILIDAD DE UN LOTE
void GeneradorReportes::reporteTrazabilidad(const string& codigoLote, const string& nombreArchivo) {
    string archivoDot = nombreArchivo + "_" + codigoLote + ".dot";
    ofstream dot(archivoDot);
    if (!dot.is_open()) {
        cout <<"Error: no se udo crear " << archivoDot << endl;
        return;
    }

    // Buscar el lote en el árbol B
    vector<string> fechas=arbolB.listarTodasLasFechas();
    Lote* loteEncontrado =nullptr;
    for (const string& f : fechas) {
        ArbolAVL* avl =arbolB.obtenerArbolAVL(f);
        if (avl) {
            loteEncontrado=avl->buscarLote(codigoLote);
            if (loteEncontrado) 
                break;
        }
    }
    dot << "digraph \"Trazabilidad_" <<codigoLote<< "\" {\n";;
    dot << "  node [shape=box];\n";
    dot << "  rankdir=LR;\n";

    if (!loteEncontrado) {
        dot << " node [label=\"Lote " << codigoLote << " no encontrado\", color=red];\n";
    }
    else {
        // Generar nodos para cada estado
        int id=0;
        for (const auto& reg : loteEncontrado->historialEstados) {
            dot << "  estado" << id << " [label=\"" << reg.timestamp << "\\n" << reg.estado << "\"];\n";
            if (id >0) {
                dot << "  estado" << id - 1 << " -> estado" << id << ";\n";
            }
            id++;
        }

        // Nodo del certificado
        if (!loteEncontrado->hashCertificado.empty()) {
            dot << "  certificado [label=\"Certificado\\n"<<loteEncontrado->hashCertificado.substr(0, 8) << "...\", shape=box, color=blue];\n";
            if (id > 0) {
                dot << "  estado" <<id - 1<< " -> certificado;\n";
            }
            int pos =arbolMerkle.obtenerPosicionHoja(loteEncontrado->hashCertificado);
            string raiz=arbolMerkle.obtenerHashRaiz();
            if (pos != -1) {
                dot << "  merkle_info [label=\"Merkle: hoja #" << (pos + 1) << " de " << arbolMerkle.obtenerNumeroHojas()
                    << "\\nRaíz: "<<raiz.substr(0, 16) <<"...\", shape=plaintext, fontsize=12];\n";
                // Coneccion del certificado a la info del Merkle 
                dot << "  certificado->merkle_info [style=dashed, color=gray];\n";
            }
        }
    }
    dot << "}\n";
    dot.close();

    compilarDot(archivoDot);
}
