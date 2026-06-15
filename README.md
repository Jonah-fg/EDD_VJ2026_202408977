# Manual Técnico – EDD CoffeeTrack Fase 1

**Universidad San Carlos de Guatemala**  
**Facultad de Ingeniería**  
**Ingeniería en Ciencias y Sistemas**  
**Estructura de Datos**  

**Estudiante:** Jonathan Eduardo Fuentes Garcia 
**Carné:** 202408977
**Fecha:** 14/06/2026 

---

## ¿Qué es EDD CoffeeTrack?

Es un programa de consola hecho en C++ que ayuda a cotrolar el café en un beneficio húmedo. Sirve para llevar el inventario de lotes de café, la cola de despulpado, las entregas que hacen las fincas y las acciones del administrador. Todo se guarda usando listas, colas y pilas hechas a mano (sin usar las que ya trae C++). También genera dibujos de las estructuras con Grphviz.

---

## Estructuras de datos que usé
 
### 1. Lista doblemente enlazada (`ListaDoble`)
**Para qué sirve:** Guarda los lotes de café pergamino que están en la bodega. Los lotes se ordenan por código de lote.

**Cómo está hecha:**  
Cada nodo tiene un `Lote` (con código, finca, origen, cantidad, fecha de recepción, nivel mínimo) y dos punteros: uno al siguiente (`sig`) y otro al anterior (`ant`). La lista tiene punteros a la cabeza y a la cola.

**Métodos importantes:**
- `insertarOrdenado()`: Agrega un lote en el lugar que le toca según su código (orden ascendente). Si la lista está vacía, el nuevo es cabeza y cola. Si el nuevo código es menor que el de la cabeza, se inserta al inicio. Si no, recorre hasta encontrar la posición correcta y engancha el nodo.
- `buscarPorCodigo()` y `buscarPorFinca()`: Recorren la lista desde la cabeza hasta encontrar lo que buscan.
- `mostrar()`: Imprime todos los lotes. Si la cantidad es menor que el nivel mínimo, muestra `[ALERTA: bajo stock]`.
- `generarReporte()`: Crea un archivo `.dot` y luego una imagen PNG. Los nodos son rectangulares, las flechas van para los dos lados (bidireccionales) y el color es verde si hay suficiente stock, rojo si falta.

**Complejidad:**  
- Insertar ordenado: O(n) porque en el peor caso recorre toda la lista.  
- Buscar: O(n) también.  
- Mostrar: O(n).

### 2. Cola FIFO (`Cola`)
**Para qué sirve:** Guarda los lotes que están esperando a ser despulpados. El primero en entrar es el primero en salir.

**Cómo está hecha:**  
Cada nodo tiene un `Lote` y un puntero al siguiente. La cola tiene punteros al frente y al final.

**Métodos:**
- `enqueue()`: Agrega un lote al final. Si la cola está vacía, el nuevo es frente y final. Si no, el final actual apunta al nuevo y el nuevo pasa a ser el final.
- `dequeue()`: Saca el lote del frente. Guarda el dato, mueve el frente al siguiente y borra el nodo. Si la cola se queda vacía, el final también se pone a `nullptr`.
- `generarReporte()`: Hace un dibujo con nodos circulares y flechas de izquierda a derecha (una sola dirección). También muestra cuántos lotes están esperando.

**Complejidad:**  
- Enqueue y dequeue: O(1) (solo cambian punteros).  
- Mostrar: O(n).

### 3. Pila LIFO (`Pila`) – Bitácora del administrador
**Para qué sirve:** Guarda un historial de las acciones importantes que hace el administrador (registrar lote, procesar cola, registrar entrega). La última acción es la primera que se muestra.

**Cómo está hecha:**  
Cada nodo tiene un `Registro` (fechaHora, tipoAccion, detalles) y un puntero `abajo` hacia el nodo más antiguo. Solo se guarda el `tope`.

**Métodos:**
- `push()`: Crea un nuevo nodo, su `abajo` apunta al tope actual, y luego el tope se actualiza al nuevo.
- `pop()`: Saca el nodo del tope, devuelve su registro y lo elimina.
- `generarReporte()`: Dibuja una pila vertical (de arriba hacia abajo). El nodo de arriba es el más reciente (tope). Colores: azul para "Registro lote", naranja para "Registro entrega", verde para "Procesar lote".

**Complejidad:**  
- Push y pop: O(1).  
- Mostrar: O(n).

### 4. Lista de pilas (`ListaPilas`) – Fincas con entregas
**Para qué sirve:** Cada finca productora tiene una pila donde se guardan las entregas de sacos que hizo por día de cosecha. La lista de fincas es simplemente enlazada (solo un puntero hacia la siguiente).

**Cómo está hecha:**  
Hay dos estructuras anidadas:
- `NodoFinca`: tiene el nombre de la finca, un puntero `topeEntregas` (que apunta a una pila de entregas) y un puntero `sig` a la siguiente finca.
- `NodoEntrega`: tiene `fecha` y `cantidad`, y un puntero `abajo` (para la pila).

**Métodos:**
- `agregarFinca()`: Crea un nuevo nodo finca y lo pone al inicio de la lista.
- `existeFinca()`: Recorre la lista para ver si ya está registrada.
- `registrarEntrega()`: Busca la finca, crea un nuevo nodo de entrega y lo apila (push) en la pila de esa finca.
- `generarReporte()`: Dibuja un cluster (cajita) por cada finca, con el nombre arriba. Dentro de cada cluster, las entregas se apilan verticalmente (la más reciente arriba). Las fincas se colocan una al lado de la otra (horizontal).

**Complejidad:**  
- Agregar finca: O(1) (inserta al inicio).  
- Buscar finca y registrar entrega: O(m) donde m es el número de fincas (recorre la lista). Luego el push es O(1).  
- Mostrar: O(m + total entregas).

---

## Cómo funciona el programa paso a paso

### Al iniciar
Te pregunta si eres administrador o usuario. Si eres administrador, entras directo. Si eres usuario, te pide usuario y contraseña (solo sirven `operario/123` y `planta/456`).

### Menú de administrador (7 opciones + salir)
1. **Registrar lote manual:** Ingresas los datos (código, finca, origen, cantidad, fecha, nivel mínimo). El lote se guarda en la lista doble ordenada. Luego te pregunta si quieres encolarlo para despulpado. Si dices que sí, también se agrega a la cola.
2. **Carga masiva CSV:** Lees un archivo .csv con varios lotes y se agregan todos al inventario.
3. **Gestionar fincas:** Agregas una nueva finca (solo el nombre). Se guada en la lista simple de fincas.
4. **Registrar entrega:** Eliges una finca que ya existe, pones fecha y cantidad. Eso se apila en la pila de entregas de esa finca. Además, se crea un lote nuevo en el inventario (con código automático como `ENT-fecha-nombre`) con esa cantidad.
5. **Procesar cola:** Saca el primer lote de la cola de despulpado, muestra sus detalles y lo registra en la bitácora.
6. **Ver inventario:** Muestra todos los lotes ordenados, con alertas si alguno tiene bajo stock.
7. **Consultar bitácora:** Muestra todas las acciones que hizo el administrador, de la más reciente a la más antigua.
8. **Salir:** Regresa al menú de selección de rol.

### Menú de usuario (operario)
1. **Consultar disponibilidad:** Buscas un lote por código o por finca. Te dice la cantidad y el estado.
2. **Enviar lote a cola:** Ingresas el código de un lote que esté en inventario (con stock >0) y se encola paa despulpado.
3. **Ver historial de procesados:** Muestra toda la bitácora (pila). Ahí puedes ver las acciones de tipo "Procesar lote".
4. **Cerrar sesión:** Vuelve al menú de roles.

### Reportes gráficos

Cada vez que haces algo que cambia los datos (registrar, encolar, procesar, agregar finca, etc.), se actualizan cuatro imágenes PNG:
- `reporte_inventario.png` – Lista doble con colores.
- `reporte_cola.png` – Cola circular.
- `reporte_bitacora.png` – Pila vertical con colores.
- `reporte_fincas.png` – Lista horizontal de fincas con pilas de entregas.

Para que esto funcione, necesitas tener instalado Graphviz y que el comando `dot` se pueda ejecutar desde la terminal.

---

## Cómo compilar y ejecutar

### Requisitos
- Un compiador de C++ (g++, Visual Studio, etc.)
- Graphviz instalado (desde [graphviz.org](https://graphviz.org/)).

### Pasos
1. Guarda todos los archivos en una carpeta (`ListaDoble.h`, `ListaDoble.cpp`, `Cola.h`, `Cola.cpp`, `Pila.h`, `Pila.cpp`, `ListaPilas.h`, `ListaPilas.cpp`, `Lote.h`, `Registro.h`, `Entrega.h`, `main.cpp`).
2. Abre una terminal en esa carpeta.
3. Compila con:  
   `g++ -std=c++11 *.cpp -o coffeetrack`
4. Ejecuta:  
   - En Windows: `coffeetrack.exe`  
   - En Linux/Mac: `./coffeetrack`

### Archivo CSV de ejemplo (`datos_ejemplo.csv`)
Pon este contenido en un archivo y guárdalo en la misma carpeta del programa:
```csv
codigo,finca,origen,cantidad,fechaRecepcion,nivelMinimo
L001,FincaElSol,Antigua,100,2026-06-01,20
L002,FincaLaMontana,Huehuetenango,50,2026-06-02,15
L003,FincaElBosque,Coban,200,2026-06-03,30

