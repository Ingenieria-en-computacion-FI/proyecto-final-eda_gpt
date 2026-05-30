#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "memory_manager.h"
#include "process.h"

/* ── Fuerza Bruta ── */
/* Busca exhaustivamente el primer bloque libre >= size */
MemoryBlock* brute_force_find_block(MemoryManager* mm, int size);

/* ── Greedy ── */
/* First Fit: primer bloque que cabe */
int greedy_first_fit(MemoryManager* mm, int pid, int size);
/* Best Fit: bloque más pequeño que cabe */
int greedy_best_fit(MemoryManager* mm, int pid, int size);
/* Worst Fit: bloque más grande disponible */
int greedy_worst_fit(MemoryManager* mm, int pid, int size);
/* SJF: ordena procesos por burst_time ascendente */
void greedy_sjf_sort(Process* procs, int n);

/* ── Backtracking ── */
/* Intenta asignar memoria a todos los procesos; si falla, retrocede */
int backtrack_allocate(MemoryManager* mm, Process* procs, int n, int index);

/* ── Divide y Vencerás ── */
/* Compacta memoria moviendo bloques libres al final */
void divide_conquer_compact(MemoryManager* mm);
/* Merge sort por burst_time */
void merge_sort_processes(Process* procs, int left, int right);

/* ── Programación Dinámica ── */
/* Mochila 0/1: maximiza procesos asignados sin exceder memoria total */
int dp_knapsack_bottomup(Process* procs, int n, int capacity);
int dp_knapsack_topdown(Process* procs, int n, int capacity, int** memo);

#endif 