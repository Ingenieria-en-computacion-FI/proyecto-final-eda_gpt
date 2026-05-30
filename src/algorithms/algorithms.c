/*
 * algorithms.c — Algoritmos del sistema operativo
 *
 * ┌─────────────────────────────────────────────┐
 * │  Algoritmo        │ Temporal  │ Espacial     │
 * ├─────────────────────────────────────────────┤
 * │  Fuerza Bruta     │ O(n)      │ O(1)         │
 * │  Greedy FF/BF/WF  │ O(n)      │ O(1)         │
 * │  Greedy SJF sort  │ O(n log n)│ O(n)         │
 * │  Backtracking     │ O(2^n)    │ O(n)         │
 * │  Merge Sort       │ O(n log n)│ O(n)         │
 * │  DP Knapsack BU   │ O(n*W)    │ O(n*W)       │
 * │  DP Knapsack TD   │ O(n*W)    │ O(n*W)       │
 * └─────────────────────────────────────────────┘
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algorithms.h"

/* ═══════════════════════════════════════════════
   FUERZA BRUTA
   Búsqueda exhaustiva del primer bloque libre >= size
   T(n) = O(n), S(n) = O(1)
   ═══════════════════════════════════════════════ */
MemoryBlock* brute_force_find_block(MemoryManager* mm, int size) {
    MemoryBlock* cur = mm->head;
    while (cur) {
        if (cur->free && cur->size >= size)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

/* ═══════════════════════════════════════════════
   GREEDY — First Fit
   Primer bloque libre suficientemente grande.
   Retorna dirección de inicio o -1.
   T(n) = O(n)
   ═══════════════════════════════════════════════ */
int greedy_first_fit(MemoryManager* mm, int pid, int size) {
    MemoryBlock* cur = mm->head;
    while (cur) {
        if (cur->free && cur->size >= size) {
            /* Dividir bloque si queda espacio */
            if (cur->size > size) {
                MemoryBlock* rem = (MemoryBlock*)malloc(sizeof(MemoryBlock));
                rem->start = cur->start + size;
                rem->size  = cur->size - size;
                rem->free  = 1;
                rem->pid   = -1;
                rem->prev  = cur;
                rem->next  = cur->next;
                if (cur->next) cur->next->prev = rem;
                cur->next = rem;
            }
            cur->size = size;
            cur->free = 0;
            cur->pid  = pid;
            return cur->start;
        }
        cur = cur->next;
    }
    return -1;
}

/* ═══════════════════════════════════════════════
   GREEDY — Best Fit
   Bloque más pequeño >= size.
   T(n) = O(n)
   ═══════════════════════════════════════════════ */
int greedy_best_fit(MemoryManager* mm, int pid, int size) {
    MemoryBlock* best = NULL;
    MemoryBlock* cur  = mm->head;
    while (cur) {
        if (cur->free && cur->size >= size)
            if (!best || cur->size < best->size) best = cur;
        cur = cur->next;
    }
    if (!best) return -1;

    if (best->size > size) {
        MemoryBlock* rem = (MemoryBlock*)malloc(sizeof(MemoryBlock));
        rem->start = best->start + size;
        rem->size  = best->size - size;
        rem->free  = 1; rem->pid = -1;
        rem->prev  = best; rem->next = best->next;
        if (best->next) best->next->prev = rem;
        best->next = rem;
    }
    best->size = size; best->free = 0; best->pid = pid;
    return best->start;
}

/* ═══════════════════════════════════════════════
   GREEDY — Worst Fit
   Bloque más grande disponible.
   T(n) = O(n)
   ═══════════════════════════════════════════════ */
int greedy_worst_fit(MemoryManager* mm, int pid, int size) {
    MemoryBlock* worst = NULL;
    MemoryBlock* cur   = mm->head;
    while (cur) {
        if (cur->free && cur->size >= size)
            if (!worst || cur->size > worst->size) worst = cur;
        cur = cur->next;
    }
    if (!worst) return -1;

    if (worst->size > size) {
        MemoryBlock* rem = (MemoryBlock*)malloc(sizeof(MemoryBlock));
        rem->start = worst->start + size;
        rem->size  = worst->size - size;
        rem->free  = 1; rem->pid = -1;
        rem->prev  = worst; rem->next = worst->next;
        if (worst->next) worst->next->prev = rem;
        worst->next = rem;
    }
    worst->size = size; worst->free = 0; worst->pid = pid;
    return worst->start;
}

/* ═══════════════════════════════════════════════
   GREEDY — SJF Sort (Insertion Sort para pocos, Merge Sort para muchos)
   Ordena procesos por burst_time ascendente.
   T(n) = O(n log n) con merge sort
   ═══════════════════════════════════════════════ */
void greedy_sjf_sort(Process* procs, int n) {
    merge_sort_processes(procs, 0, n - 1);
}

/* ═══════════════════════════════════════════════
   BACKTRACKING
   Intenta asignar memoria a todos los procesos.
   Si un proceso no cabe, retrocede y marca como BLOCKED.
   T(n) = O(2^n) peor caso, O(n) caso promedio
   S(n) = O(n) — pila de llamadas recursivas
   ═══════════════════════════════════════════════ */
int backtrack_allocate(MemoryManager* mm, Process* procs, int n, int index) {
    if (index == n) return 1;  /* Caso base: todos procesados */

    /* Intentar asignar memoria al proceso actual */
    int addr = greedy_first_fit(mm, procs[index].pid, procs[index].memory_required);
    if (addr >= 0) {
        procs[index].state = RUNNING;
        /* Recursión: intentar con el siguiente */
        if (backtrack_allocate(mm, procs, n, index + 1))
            return 1;
        /* Backtrack: liberar y marcar como BLOCKED */
        mm_free(mm, procs[index].pid);
        procs[index].state = BLOCKED;
    } else {
        procs[index].state = BLOCKED;
    }

    /* Intentar sin este proceso */
    return backtrack_allocate(mm, procs, n, index + 1);
}

/* ═══════════════════════════════════════════════
   DIVIDE Y VENCERÁS — Merge Sort por burst_time
   Recurrencia: T(n) = 2T(n/2) + O(n)
   Solución (Teorema Maestro, caso 2): T(n) = O(n log n)
   ═══════════════════════════════════════════════ */
static void merge(Process* procs, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Process* L = (Process*)malloc(n1 * sizeof(Process));
    Process* R = (Process*)malloc(n2 * sizeof(Process));

    memcpy(L, procs + left,    n1 * sizeof(Process));
    memcpy(R, procs + mid + 1, n2 * sizeof(Process));

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].burst_time <= R[j].burst_time)
            procs[k++] = L[i++];
        else
            procs[k++] = R[j++];
    }
    while (i < n1) procs[k++] = L[i++];
    while (j < n2) procs[k++] = R[j++];

    free(L);
    free(R);
}

void merge_sort_processes(Process* procs, int left, int right) {
    if (left >= right) return;           /* Caso base */
    int mid = left + (right - left) / 2;
    merge_sort_processes(procs, left, mid);      /* Dividir izquierda */
    merge_sort_processes(procs, mid + 1, right); /* Dividir derecha */
    merge(procs, left, mid, right);              /* Combinar */
}

/* ═══════════════════════════════════════════════
   PROGRAMACIÓN DINÁMICA — Mochila 0/1 Bottom-Up
   Maximiza la cantidad de procesos que caben en memoria.
   T(n) = O(n * W), S(n) = O(n * W)
   donde W = capacidad total de memoria
   ═══════════════════════════════════════════════ */
int dp_knapsack_bottomup(Process* procs, int n, int capacity) {
    /* Tabla dp[i][w] = max procesos usando primeros i con capacidad w */
    int** dp = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)calloc(capacity + 1, sizeof(int));
    }

    for (int i = 1; i <= n; i++) {
        int w = procs[i-1].memory_required;
        for (int cap = 0; cap <= capacity; cap++) {
            dp[i][cap] = dp[i-1][cap];  /* No incluir proceso i */
            if (w <= cap && dp[i-1][cap-w] + 1 > dp[i][cap])
                dp[i][cap] = dp[i-1][cap-w] + 1;  /* Incluir proceso i */
        }
    }

    int result = dp[n][capacity];
    for (int i = 0; i <= n; i++) free(dp[i]);
    free(dp);
    return result;
}

/* ═══════════════════════════════════════════════
   PROGRAMACIÓN DINÁMICA — Mochila 0/1 Top-Down (Memoización)
   T(n) = O(n * W), S(n) = O(n * W)
   ═══════════════════════════════════════════════ */
int dp_knapsack_topdown(Process* procs, int n, int capacity, int** memo) {
    if (n == 0 || capacity == 0) return 0;
    if (memo[n][capacity] != -1) return memo[n][capacity];

    int w = procs[n-1].memory_required;

    /* No incluir proceso n */
    int without = dp_knapsack_topdown(procs, n-1, capacity, memo);

    /* Incluir proceso n si cabe */
    int with = 0;
    if (w <= capacity)
        with = 1 + dp_knapsack_topdown(procs, n-1, capacity - w, memo);

    memo[n][capacity] = (with > without) ? with : without;
    return memo[n][capacity];
}
