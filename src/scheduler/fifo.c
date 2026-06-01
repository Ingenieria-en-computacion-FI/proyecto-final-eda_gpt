/*
 * fifo.c — Scheduler FIFO (First In, First Out)
 *
 * El proceso que llega primero es el primero en ejecutarse.
 * No hay interrupción: el proceso corre hasta terminar.
 *
 * Complejidad temporal:
 *   add_process : O(1)
 *   next        : O(1)
 *   run         : O(n)
 *
 * Recurrencia: T(n) = T(n-1) + O(1) => T(n) = O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

/* ── Crear scheduler FIFO ── */
Scheduler* scheduler_create_fifo() {
    Scheduler* s = (Scheduler*)malloc(sizeof(Scheduler));
    if (!s) {
        fprintf(stderr, "scheduler_create_fifo: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    s->type          = SCHED_FIFO;
    s->count         = 0;
    s->quantum       = 0;
    s->current_time  = 0;
    s->completed     = 0;
    s->fifo_queue    = queue_create();
    s->rr_queue      = NULL;
    s->finished_list = ll_create();
    s->history       = stack_create();
    memset(s->processes, 0, sizeof(s->processes));
    return s;
}

/* ── Agregar proceso ── */
void scheduler_add_process(Scheduler* s, int pid, int burst_time) {
    Process p = process_create(pid, burst_time, 0, 0);
    scheduler_add_process_full(s, p);
}

void scheduler_add_process_full(Scheduler* s, Process p) {
    if (s->count >= MAX_PROCESSES) {
        fprintf(stderr, "scheduler_add_process: límite alcanzado\n");
        return;
    }
    s->processes[s->count++] = p;

    /* Encolar según tipo */
    if (s->type == SCHED_FIFO)
        enqueue(s->fifo_queue, p.pid);
    else if (s->type == SCHED_ROUND_ROBIN)
        cq_enqueue(s->rr_queue, p.pid);
    /* SJF se ordena antes de ejecutar */
}

/* ── Obtener PID del siguiente proceso (FIFO): O(1) ── */
int scheduler_next(Scheduler* s) {
    if (queue_is_empty(s->fifo_queue)) return -1;
    return dequeue(s->fifo_queue);
}

/* ── Ejecutar simulación FIFO completa ── */
static void run_fifo(Scheduler* s) {
    printf("\n=== Simulacion FIFO ===\n");
    printf("%-6s %-8s %-8s %-12s\n", "PID", "Inicio", "Fin", "Tiempo CPU");

    /* Volver a encolar todos los procesos en orden */
    Queue* q = s->fifo_queue;
    /* La cola ya tiene los PIDs en orden de llegada */
    while (!queue_is_empty(q)) {
        int pid = dequeue(q);

        /* Buscar proceso */
        Process* p = NULL;
        for (int i = 0; i < s->count; i++) {
            if (s->processes[i].pid == pid) {
                p = &s->processes[i];
                break;
            }
        }
        if (!p) continue;

        p->state        = RUNNING;
        int start_time  = s->current_time;
        s->current_time += p->burst_time;
        p->remaining_time = 0;
        p->state        = FINISHED;

        stack_push(s->history, pid);
        ll_insert_sorted(s->finished_list, pid, p->burst_time);
        s->completed++;

        printf("%-6d %-8d %-8d %-12d\n",
               pid, start_time, s->current_time, p->burst_time);
    }

    printf("\nTiempo total: %d unidades\n", s->current_time);
}

void scheduler_run(Scheduler* s) {
    if (s->type == SCHED_FIFO)
        run_fifo(s);
}

void scheduler_print_results(Scheduler* s) {
    printf("\n=== Procesos terminados (ordenados por PID) ===\n");
    ll_print(s->finished_list);
    printf("\n=== Historial de ejecucion (pila) ===\n");
    stack_print(s->history);
}

void scheduler_destroy(Scheduler* s) {
    if (s->fifo_queue)    queue_destroy(s->fifo_queue);
    if (s->rr_queue)      cq_destroy(s->rr_queue);
    if (s->finished_list) ll_destroy(s->finished_list);
    if (s->history)       stack_destroy(s->history);
    free(s);
} 