# ☕ EDD CoffeeTrack – Fase 2

**Universidad San Carlos de Guatemala**  
**Facultad de Ingeniería**  
**Ingeniería en Ciencias y Sistemas**  
**Estructuras de Datos**  

---

## Datos del Estudiante

- **Nombre:** Jonathan Eduardo Fuentes Garcia  
- **Carnet:** 202408977  
- **Curso:** Estructuras de Datos  
- **Fecha de Entrega:** 30/06/2026
- :)

---

## Resumen Ejecutivo

EDD CoffeeTrack es un sistema de trazabilidad y certificación para lotes de café especial. En esta Fase 2, el sistema evoluciona desde estructuras lineales (usadas en la Fase 1) hacia estructuras no lineales para indexar, buscar y certificar las entregas de forma eficiente y segura.

El sistema implementa:

- Un **Árbol B** como índice principal por fecha (`YYYY-MM-DD`).
- **Árboles AVL** dentro de cada nodo del Árbol B para organizar los lotes entregados en esa fecha.
- Un **Árbol de Merkle** para garantizar la integridad criptográfica de los certificados emitidos.
- Carga masiva de datos desde un archivo **JSON**.
- Generación de **certificados encriptados** (con cifrado XOR) y almacenamiento en carpeta local.
- **Reportes gráficos** mediante **Graphviz** (`.dot` → `.png`) del Árbol B, AVLs, Árbol de Merkle y trazabilidad de lotes.

---

## Estructuras de Datos Implementadas

Todas las estructuras fueron implementadas **desde cero** en C++, sin usar librerías de la STL (como `std::map`, `std::set`, `std::queue`, etc.) para los fines requeridos.

### 1. Árbol B (Índice por Fecha)
- **Orden mínimo:** 3 (cada nodo puede contener entre **2 y 6 claves**).
- **Claves:** Fechas en formato `string` (`YYYY-MM-DD`).
- **Operaciones:**
  - Inserción con división de nodos al exceder la capacidad (6 claves).
  - Búsqueda por fecha (retorna el puntero al Árbol AVL asociado).
  - Recorrido in-order para listar todas las fechas registradas.
- **Complejidad:**
  - Inserción/Búsqueda: **O(logₘ n)**, donde *m* es el orden del árbol (máximo 6 claves por nodo).
  - Recorrido in-order: **O(n)**, donde *n* es el número total de fechas.

### 2. Árbol AVL (Lotes por Fecha)
- Cada nodo del Árbol B apunta a un Árbol AVL independiente que almacena los lotes de esa fecha.
- **Clave de ordenamiento:** `codigo_lote` (`int` convertido a `string`).
- **Campos del nodo AVL:** código de lote, código de finca, nombre de finca, sacos, tipo de café, ruta tomada, distancia, estado actual, historial de estados y hash del certificado.
- **Operaciones:**
  - Inserción con rotaciones (LL, RR, LR, RL).
  - Búsqueda por código de lote.
  - Recorrido in-order.
  - Avance de estado (con timestamp) siguiendo la secuencia:  
    `recibido → en_cola → procesado → en_bodega → certificado_emitido`.
- **Complejidad:**
  - Inserción/Búsqueda: **O(log n)**, donde *n* es el número de lotes en esa fecha.

### 3. Árbol de Merkle (Integridad de Certificados)
- **Hojas:** Hash del contenido original de cada certificado (antes de encriptar).
- **Nodos internos:** Hash de la concatenación de sus dos hijos.
- **Manejo de impares:** Si el número de hojas es impar, la última hoja se duplica.
- **Operaciones:**
  - Construcción completa desde una lista de hashes.
  - Actualización automática al generar un nuevo certificado.
  - Verificación de integridad (recalcular hash y comparar).
- **Complejidad:**
  - Construcción: **O(n)**, donde *n* es el número de certificados.
  - Verificación: **O(log n)** (ideal), aunque en esta implementación se recalcula la raíz completamente, resultando en **O(n)** por simplicidad y consistencia.

### 4. Función de Hashing
Se implementó una función híbrida que combina **FNV-1a de 64 bits** con una etapa de **avalancha** (mezcla de bits) para mejorar la distribución.

- **Entrada:** `std::string`.
- **Salida:** Cadena hexadecimal de **32 caracteres** (2 bloques de 16 bytes).
- **Justificación:** Es determinista, rápida y suficientemente distribuida para evitar colisiones en el contexto del proyecto. Se aplica sobre el contenido del certificado (`hash_contenido`) y sobre el código del lote (`hash_nombre`).

### 5. Encriptación (Certificados)
Se utilizó un cifrado **XOR** con una clave fija (`"CoffeeTrack2026EDD_Secret!"`).

- **Razón:** Es un método simétrico simple y reversible, ideal para proteger el contenido de los certificados sin necesidad de librerías externas. El objetivo no es seguridad militar, sino evitar la lectura directa del archivo.

---

## Carga Masiva desde JSON

El sistema lee un archivo JSON con dos secciones principales:

1. **`fincas`**: Define las fincas productoras (código, nombre, región, propietario).
2. **`entregas`**: Define los lotes (fecha, código_lote, finca, sacos, tipo_cafe, estado).

**Flujo de procesamiento:**
1. Se cargan primero todas las fincas (se almacenan en `FincaManager`).
2. Se procesan las entregas una por una.
3. Se valida que la finca exista. Si no existe, la entrega se ignora y se incrementa el contador de ignoradas.
4. Si la fecha no existe en el Árbol B, se crea un nuevo AVL y se inserta la fecha.
5. Se inserta el lote en el AVL correspondiente.

**Complejidad:**  
El parseo manual del JSON recorre todo el archivo una sola vez, resultando en **O(n)**, donde *n* es el número total de objetos (fincas + entregas).

---

## Menú Principal

El sistema expone un menú interactivo en consola con las siguientes opciones:

1. **Gestión de Datos:** Carga JSON y registro manual.
2. **Consultas y Trazabilidad:** Búsquedas, listado de fechas, trazabilidad y avance de estados.
3. **Rutas:** (Omitido – ver sección "Decisiones de Diseño").
4. **Certificados:** Generación individual, masiva y visualización desencriptada.
5. **Árbol de Merkle:** Construcción, verificación y hash raíz.
6. **Reportes Graphviz:** Generación de imágenes para Árbol B, AVL, Merkle y trazabilidad.

---

## Reportes con Graphviz

Todos los reportes se generan en formato `.dot` y se compilan automáticamente a `.png` usando el comando `dot -Tpng`.

| Reporte | Descripción | Formato de nodos |
| :--- | :--- | :--- |
| **Árbol B** | Muestra las fechas como claves. Nodos hoja resaltados en **celeste**. | Rectangulares (`record`). |
| **AVL (por fecha)** | Muestra código d lote, finca, sacos y factor de balance (BF). BF fuera de `{-1,0,1}` se colorea en **rojo**. | Circulares. |
| **Árbol de Merkle** | Muestra hashes truncados (8 caracteres). Hojas en **verde**, raíz en **azul**. | Rectangulares. |
| **Trazabilidad** | Diagrama lineal de estados con timestamps, flecha al certificado y posición en Merkle. | Rectangulares. |

---

## Análisis de Complejidad Global

A continuación se resume la complejidad temporal y espacial de las principales operaciones del sistema.

| Estructura / Operación | Complejidad Temporal | Complejidad Espacial | Observaciones |
| :--- | :--- | :--- | :--- |
| **Árbol B** – Inserción | **O(logₘ n)** | **O(n)** | *m* = orden del árbol (máximo 6 claves por nodo). |
| **Árbol B** – Búsqueda por fecha | **O(logₘ n)** | **O(1)** | Retorna puntero al AVL correspondiente. |
| **Árbol B** – Recorrido in-order | **O(n)** | **O(n)** | Para listar todas las fechas. |
| **Árbol AVL** – Inserción | **O(log n)** | **O(n)** | *n* = número de lotes en esa fecha. |
| **Árbol AVL** – Búsqueda por código | **O(log n)** | **O(1)** | |
| **Árbol de Merkle** – Construcción | **O(n)** | **O(n)** | *n* = número de certificados (hojas). |
| **Árbol de Merkle** – Verificación | **O(n)** | **O(1)** | Recalcula la raíz completa por simplicidad. |
| **Carga JSON** – Parseo | **O(n)** | **O(1)** | *n* = número total de objetos (fincas + entregas). |
| **Generación de certificado** | **O(1)** | **O(1)** | Creación de archivo y hash. |
| **Reporte Graphviz** – Generación | **O(n)** | **O(n)** | Escritura del archivo `.dot`. |

> **Nota:**  
> - **n** representa el tamaño de la entrada (númer de fechas, lotes o certificados, según el contexto).  
> - Todas las estructuras son implementadas manualmente, sin uso de STL, por lo que la complejidad refleja el comportamiento real del código.

## Instrucciones de Compilación y Ejecución

### Requisitos
- Compilador C++ (g++ o MSVC).
- Graphviz instalado y `dot` accesible desde la terminal (para los reportes).

### Compilación (Linux/Mac)
```bash
g++ -std=c++11 -o EDD_CoffeeTrack *.cpp
