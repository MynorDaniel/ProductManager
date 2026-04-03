# ProductManager

Aplicación en C++ que gestiona un catálogo de productos usando varias estructuras de datos implementadas desde cero:

- Lista enlazada desordenada
- Lista enlazada ordenada
- Árbol AVL
- Árbol B
- Árbol B+

Incluye operaciones de inserción, eliminación, búsqueda, carga desde CSV, comparación de búsquedas y visualización de estructuras con Graphviz.

---

## Requisitos

- Linux (probado en entorno bash)
- `g++` con soporte para C++17
- `cmake` (>= 3.10)
- `make`
- `graphviz`

---

## Compilación

Desde la carpeta `build/`:

```bash
cmake --build build
```

---

## Ejecución

```bash
./build/ProductManager
```

También se puede usar el script incluido:

```bash
./exec.sh
```

> `exec.sh` limpia la terminal, compila (`cmake --build build`) y luego ejecuta `./build/ProductManager`.

---



