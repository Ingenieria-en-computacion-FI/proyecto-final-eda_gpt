/*
 * memory_manager.c — Administrador de memoria con lista doblemente ligada
 *
 * Algoritmos implementados:
 *   First Fit  (Greedy)    : O(n)
 *   Best Fit   (Greedy)    : O(n)
 *   Worst Fit  (Greedy)    : O(n)
 *   Coalescencia           : O(n)
 *   Compactación           : O(n²)  divide y vencerás conceptual
 *
 * Complejidad espacial: O(n) — n bloques en la lista.
 *
 * Recurrencia Coalescencia (un solo paso):
 *   T(n) = T(n-1) + O(1) => T(n) = O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

/* ─────────────────────────────────────────────
   Crear administrador de memoria
   Inicializa un único bloque libre de tamaño total_memory
   ───────────────────────────────────────────── */
MemoryManager* mm_create(int total_memory) {
    MemoryManager* mm = (MemoryManager*)malloc(sizeof(MemoryManager));
    if (!mm) {
        fprintf(stderr, "mm_create: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    MemoryBlock* block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    if (!block) {
        fprintf(stderr, "mm_create: malloc block failed\n");
        exit(EXIT_FAILURE);
    }

    block->start = 0;
    block->size  = total_memory;
    block->free  = 1;
    block->pid   = -1;
    block->next  = NULL;
    block->prev  = NULL;

    mm->head         = block;
    mm->total_memory = total_memory;
    return mm;
}

/* ─────────────────────────────────────────────
   Auxiliar: dividir un bloque libre en dos
   [bloque_ocupado | bloque_restante]
   ───────────────────────────────────────────── */
static void split_block(MemoryBlock* block, int size, int pid) {
    /* Solo dividir si queda espacio significativo (> 0) */
    if (block->size > size) {
        MemoryBlock* remainder = (MemoryBlock*)malloc(sizeof(MemoryBlock));
        remainder->start = block->start + size;
        remainder->size  = block->size - size;
        remainder->free  = 1;
        remainder->pid   = -1;
        remainder->prev  = block;
        remainder->next  = block->next;

        if (block->next)
            block->next->prev = remainder;
        block->next = remainder;
    }

    block->size = size;
    block->free = 0;
    block->pid  = pid;
}

/* ─────────────────────────────────────────────
   First Fit (Greedy)
   Asigna el PRIMER bloque libre suficientemente grande.
   Retorna la dirección de inicio (start) o -1 si falla.
   T(n) = O(n)
   ───────────────────────────────────────────── */
int mm_allocate_first_fit(MemoryManager* mm, int size) {
    MemoryBlock* cur = mm->head;
    while (cur) {
        if (cur->free && cur->size >= size) {
            split_block(cur, size, size); /* pid temporal = size */
            return cur->start;
        }
        cur = cur->next;
    }
    return -1;  /* No hay espacio */
}

/* ─────────────────────────────────────────────
   Best Fit (Greedy)
   Asigna el bloque libre MÁS PEQUEÑO que sea suficiente.
   Minimiza fragmentación interna.
   T(n) = O(n)
   ───────────────────────────────────────────── */
int mm_allocate_best_fit(MemoryManager* mm, int size) {
    MemoryBlock* best = NULL;
    MemoryBlock* cur  = mm->head;

    while (cur) {
        if (cur->free && cur->size >= size) {
            if (best == NULL || cur->size < best->size)
                best = cur;
        }
        cur = cur->next;
    }

    if (best == NULL) return -1;
    split_block(best, size, size);
    return best->start;
}

/* ─────────────────────────────────────────────
   Worst Fit (Greedy)
   Asigna el bloque libre MÁS GRANDE disponible.
   Maximiza espacio restante tras la asignación.
   T(n) = O(n)
   ───────────────────────────────────────────── */
int mm_allocate_worst_fit(MemoryManager* mm, int size) {
    MemoryBlock* worst = NULL;
    MemoryBlock* cur   = mm->head;

    while (cur) {
        if (cur->free && cur->size >= size) {
            if (worst == NULL || cur->size > worst->size)
                worst = cur;
        }
        cur = cur->next;
    }

    if (worst == NULL) return -1;
    split_block(worst, size, size);
    return worst->start;
}

/* ─────────────────────────────────────────────
   Liberar memoria por PID
   Marca todos los bloques del PID como libres.
   T(n) = O(n)
   ───────────────────────────────────────────── */
void mm_free(MemoryManager* mm, int pid) {
    MemoryBlock* cur = mm->head;
    while (cur) {
        if (!cur->free && cur->pid == pid) {
            cur->free = 1;
            cur->pid  = -1;
        }
        cur = cur->next;
    }
    /* Coalescencia automática tras liberar */
    mm_coalesce(mm);
}

/* ─────────────────────────────────────────────
   Coalescencia de bloques libres adyacentes
   Combina bloques libres consecutivos en uno.
   
   Recurrencia: T(n) = T(n-1) + O(1) => T(n) = O(n)
   Ejemplo:
     Antes: [P1][Libre:100][Libre:200][P2]
     Después:[P1][Libre:300][P2]
   ───────────────────────────────────────────── */
void mm_coalesce(MemoryManager* mm) {
    MemoryBlock* cur = mm->head;
    while (cur && cur->next) {
        if (cur->free && cur->next->free) {
            /* Absorber el siguiente bloque */
            MemoryBlock* to_remove = cur->next;
            cur->size += to_remove->size;
            cur->next  = to_remove->next;

            if (to_remove->next)
                to_remove->next->prev = cur;

            free(to_remove);
            /* No avanzar: puede haber más bloques libres seguidos */
        } else {
            cur = cur->next;
        }
    }
}

/* ─────────────────────────────────────────────
   Compactación (Divide y Vencerás conceptual)
   Mueve todos los bloques ocupados al inicio,
   consolidando toda la memoria libre al final.
   T(n) = O(n²) en el peor caso.
   ───────────────────────────────────────────── */
void mm_compact(MemoryManager* mm) {
    int current_start = 0;
    MemoryBlock* cur  = mm->head;

    /* Primera pasada: reubicar bloques ocupados */
    while (cur) {
        if (!cur->free) {
            cur->start = current_start;
            current_start += cur->size;
        }
        cur = cur->next;
    }

    /* Segunda pasada: eliminar bloques libres intercalados */
    cur = mm->head;
    while (cur && cur->next) {
        if (cur->next->free) {
            MemoryBlock* to_remove = cur->next;
            cur->next = to_remove->next;
            if (to_remove->next)
                to_remove->next->prev = cur;
            free(to_remove);
        } else {
            cur = cur->next;
        }
    }

    /* Crear un único bloque libre al final si queda espacio */
    int used = 0;
    cur = mm->head;
    while (cur) {
        if (!cur->free) used += cur->size;
        cur = cur->next;
    }

    int remaining = mm->total_memory - used;
    if (remaining > 0) {
        MemoryBlock* free_block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
        free_block->start = current_start;
        free_block->size  = remaining;
        free_block->free  = 1;
        free_block->pid   = -1;
        free_block->next  = NULL;

        /* Insertar al final */
        if (mm->head == NULL) {
            free_block->prev = NULL;
            mm->head = free_block;
        } else {
            MemoryBlock* tail = mm->head;
            while (tail->next) tail = tail->next;
            tail->next        = free_block;
            free_block->prev  = tail;
        }
    }
}

/* ─────────────────────────────────────────────
   Calcular porcentaje de fragmentación externa
   Fragmentación = 1 - (bloque_libre_max / total_libre)
   Retorna valor entre 0 y 100.
   ───────────────────────────────────────────── */
int mm_fragmentation(MemoryManager* mm) {
    int total_free = 0;
    int max_free   = 0;

    MemoryBlock* cur = mm->head;
    while (cur) {
        if (cur->free) {
            total_free += cur->size;
            if (cur->size > max_free)
                max_free = cur->size;
        }
        cur = cur->next;
    }

    if (total_free == 0) return 0;
    return (int)((1.0 - (double)max_free / total_free) * 100);
}

/* ─────────────────────────────────────────────
   Imprimir estado de la memoria
   ───────────────────────────────────────────── */
void mm_print(MemoryManager* mm) {
    printf("=== Estado de Memoria (total=%d) ===\n", mm->total_memory);
    MemoryBlock* cur = mm->head;
    while (cur) {
        if (cur->free)
            printf("  [LIBRE   | start=%4d | size=%4d]\n", cur->start, cur->size);
        else
            printf("  [PID=%-4d | start=%4d | size=%4d]\n", cur->pid, cur->start, cur->size);
        cur = cur->next;
    }
    printf("  Fragmentacion externa: %d%%\n", mm_fragmentation(mm));
    printf("====================================\n");
}

/* ─────────────────────────────────────────────
   Destruir administrador de memoria
   ───────────────────────────────────────────── */
void mm_destroy(MemoryManager* mm) {
    MemoryBlock* cur = mm->head;
    while (cur) {
        MemoryBlock* tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(mm);
}