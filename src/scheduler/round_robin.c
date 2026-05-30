/*
 * round_robin.c — Scheduler Round Robin
 *
 * Cada proceso recibe un quantum de tiempo.
 * Si no termina, regresa al final de la cola circular.
 *
 * Complejidad temporal:
 *   add_process : O(1)
 *   run         : O(n * ceil(burst/quantum))  ≈ O(n * B/Q)
 *
 * Recurrencia por proceso:
 *   T(burst) = T(burst - quantum) + O(1)
 *   => T(burst) = O(burst / quantum)
 *
 * Ejemplo del proyecto (Quantum=2):
 *   P1(5) P2(3) P3(4)
 *   t=0-2:  P1 ejecuta 2, remaining=3, regresa
 *   t=2-4:  P2 ejecuta 2, remaining=1, regresa
 *   t=4-6:  P3 ejecuta 2, remaining=2, regresa
 *   t=6-8:  P1 ejecuta 2, remaining=1, regresa
 *   t=8-9:  P2 ejecuta 1, termina
 *   t=9-11: P3 ejecuta 2, termina
 *   t=11-12:P1 ejecuta 1, termina
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

/* ── Crear scheduler Round Robin ── */
Scheduler* scheduler_create_rr(int quantum) {
    Scheduler* s = (Scheduler*)malloc(sizeof(Scheduler));
    if (!s) {
        fprintf(stderr, "scheduler_create_rr: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    s->type          = SCHED_ROUND_ROBIN;
    s->count         = 0;
    s->quantum       = quantum;
    s->current_time  = 0;
    s->completed     = 0;
    s->fifo_queue    = NULL;
    s->rr_queue      = cq_create(MAX_PROCESSES);
    s->finished_list = ll_create();
    s->history       = stack_create();
    memset(s->processes, 0, sizeof(s->processes));
    return s;
}

/* ── Auxiliar: buscar proceso por PID ── */
static Process* find_process(Scheduler* s, int pid) {
    for (int i = 0; i < s->count; i++)
        if (s->processes[i].pid == pid)
            return &s->processes[i];
    return NULL;
}

/* ── Ejecutar simulación Round Robin ── */
void scheduler_run_rr(Scheduler* s) {
    printf("\n=== Simulación Round Robin (quantum=%d) ===\n", s->quantum);
    printf("%-6s %-8s %-8s %-10s %-10s\n",
           "PID", "t_inicio", "t_fin", "Ejecutado", "Restante");

    while (!cq_is_empty(s->rr_queue)) {
        int pid = cq_dequeue(s->rr_queue);
        Process* p = find_process(s, pid);
        if (!p || p->state == FINISHED) continue;

        p->state           = RUNNING;
        int start_time     = s->current_time;
        int executed       = (p->remaining_time < s->quantum)
                                ? p->remaining_time
                                : s->quantum;

        s->current_time   += executed;
        p->remaining_time -= executed;

        stack_push(s->history, pid);  /* Registrar en historial */

        if (p->remaining_time == 0) {
            p->state = FINISHED;
            ll_insert_sorted(s->finished_list, pid, p->burst_time);
            s->completed++;
            printf("%-6d %-8d %-8d %-10d %-10s\n",
                   pid, start_time, s->current_time, executed, "FINISHED");
        } else {
            p->state = READY;
            cq_enqueue(s->rr_queue, pid);   /* Reinsertar al final */
            printf("%-6d %-8d %-8d %-10d %-10d\n",
                   pid, start_time, s->current_time, executed, p->remaining_time);
        }
    }

    printf("\nTiempo total: %d unidades | Procesos completados: %d\n",
           s->current_time, s->completed);
}
