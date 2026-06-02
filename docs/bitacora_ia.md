# Bitácora de IA — Mini OS Project
**Curso:** Estructuras de Datos y Algoritmos I — UNAM 2026-2  
**Herramienta utilizada:** Claude Sonnet (Anthropic)

Integrantes:

Ramirez Olvera Erick Yael
Flores Alvarez Roberto Maximiliano
Gonzalez Morales Daniel Alejandro
Rosas Salazar Jose Eduardo

---

## Entrada 1 — Diseño de estructuras de datos

### Prompt utilizado
> "Necesito implementar una pila en C para guardar el historial de ejecución de procesos en un simulador de sistema operativo. ¿Cómo la implemento con lista enlazada?"

### Respuesta de IA
La IA propuso una implementación con nodo opaco (`struct Node` interno) y estructura `Stack` expuesta solo a través de funciones. Sugirió usar `malloc`/`free` para cada nodo y manejar el caso vacío en `pop` retornando `-1`.

### Errores encontrados
- La primera versión no inicializaba `stack->size`, causando comportamiento indefinido en `stack_size()`.
- `stack_destroy` no liberaba el `struct Stack` final, solo los nodos.

### Correcciones realizadas
- Se agregó `s->size = 0` en `stack_create`.
- Se añadió `free(stack)` al final de `stack_destroy`.

### Validaciones realizadas
- `test_stack_push_pop`: push de 3 elementos, pop verifica LIFO.
- `test_stack_empty`: verifica estado vacío antes y después de operaciones.
- `test_stack_peek`: verifica que peek no extrae elementos.

---

## Entrada 2 — Cola circular para Round Robin

### Prompt utilizado
> "Implementa una cola circular basada en arreglo en C para el scheduler Round Robin. Debe redimensionarse automáticamente cuando se llena."

### Respuesta de IA
La IA propuso usar índices `head` y `tail` con aritmética módulo (`% capacity`) para el wrap-around. Para la redimensión, sugirió duplicar la capacidad y copiar en orden lógico desde `head`.

### Errores encontrados
- Al redimensionar, la copia directa con `memcpy` no funcionaba cuando `head > 0` (el arreglo estaba "rotado"). Los elementos quedaban en orden incorrecto.

### Correcciones realizadas
- Se reemplazó `memcpy` por un bucle que usa `(head + i) % capacity` para copiar en orden lógico.
- Se reinicializaron `head = 0` y `tail = size` tras la copia.

### Validaciones realizadas
- `test_round_robin_behavior`: inserta 3, extrae 1, inserta 1 más, verifica orden FIFO circular.
- `test_circular_resize`: fuerza redimensión con capacidad inicial de 2.

---

## Entrada 3 — Administración de memoria

### Prompt utilizado
> "Implementa First Fit, Best Fit y Worst Fit en C usando una lista doblemente ligada de MemoryBlock. Incluye coalescencia de bloques libres."

### Respuesta de IA
La IA propuso una función auxiliar `split_block` para dividir bloques al asignar, y `mm_coalesce` con un bucle que absorbe el siguiente bloque cuando ambos están libres.

### Errores encontrados
- `split_block` no actualizaba el puntero `prev` del bloque posterior al `remainder`, rompiendo la navegación bidireccional.
- `mm_free` no llamaba a `mm_coalesce` automáticamente, dejando bloques fragmentados.

### Correcciones realizadas
- Se agregó `if (block->next) block->next->prev = remainder;` en `split_block`.
- Se agregó llamada a `mm_coalesce(mm)` al final de `mm_free`.

### Validaciones realizadas
- `test_first_fit`: asigna dos bloques, libera el primero, verifica que la siguiente asignación reutiliza ese espacio.
- `test_coalescence`: crea dos bloques libres adyacentes, verifica que se fusionan.
- `test_fragmentation`: verifica que `mm_fragmentation` retorna > 0 cuando hay fragmentación.

---

## Entrada 4 — Algoritmos (Backtracking y DP)

### Prompt utilizado
> "Implementa el algoritmo de backtracking para asignación de memoria y la mochila 0/1 con programación dinámica (bottom-up y top-down) para maximizar procesos asignados."

### Respuesta de IA
La IA propuso `backtrack_allocate` como función recursiva que intenta First Fit para cada proceso; si falla, marca el proceso como BLOCKED y continúa. Para DP, generó una tabla 2D `dp[i][cap]`.

### Errores encontrados
- La función `backtrack_allocate` no liberaba memoria al retroceder (no hacía el "deshacer" del backtrack real).
- La tabla DP no se liberaba correctamente, produciendo leak de memoria.

### Correcciones realizadas
- Se agregó `mm_free(mm, procs[index].pid)` antes de marcar el proceso como BLOCKED en el backtrack.
- Se agregó el bucle de `free(dp[i])` y `free(dp)` al finalizar `dp_knapsack_bottomup`.

### Validaciones realizadas
- Se probó con 3 procesos y memoria de 400 MB (uno de 250 no cabe junto con los demás).
- Se verificó que DP bottom-up y top-down retornan el mismo resultado.

---

## Entrada 5 — Scripts Python (benchmark y gráficas)

### Prompt utilizado
> "Crea un script de Python que ejecute el simulador C con subprocess, mida tiempo de ejecución para diferentes tamaños de entrada, y guarde resultados en CSV."

### Respuesta de IA
La IA generó `benchmark.py` con `time.perf_counter()` para mayor precisión que `time.time()`, y `subprocess.run` con `capture_output=True`.

### Errores encontrados
- El script no manejaba el caso en que el binario no existía, produciendo un traceback poco informativo.
- No creaba el directorio `reports/csv/` si no existía.

### Correcciones realizadas
- Se agregó `try/except FileNotFoundError` con mensaje claro sobre ejecutar `make`.
- Se agregó `os.makedirs(..., exist_ok=True)` antes de escribir el CSV.

### Validaciones realizadas
- Se ejecutó con el binario compilado y se verificó que el CSV contiene las columnas `modo, n, tiempo_s`.
- Se ejecutó `validate_results.py` y todos los asserts pasaron.

---

## Resumen de uso de IA

| Módulo             | Contribución IA         | Corrección humana                        |
|--------------------|------------------------|------------------------------------------|
| Stack              | Estructura base        | Fix en destroy, inicialización size      |
| Cola circular      | Algoritmo con módulo   | Fix en redimensión con head > 0          |
| Memory manager     | split_block + coalesce | Fix puntero prev, llamada automática     |
| Backtracking       | Estructura recursiva   | Fix liberación al retroceder             |
| DP Knapsack        | Tabla 2D               | Fix memory leak                          |
| Python scripts     | Estructura general     | Manejo de errores, creación de directorios|

**Conclusión:** La IA fue útil para generar estructuras base y detectar patrones algorítmicos. Sin embargo, los errores de punteros 
(especialmente en listas doblemente ligadas) requirieron revisión y corrección manual.
El análisis de complejidad y las recurrencias fueron verificados manualmente contra la teoría del curso.



