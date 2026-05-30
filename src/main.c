/*
 * main.c — Punto de entrada del simulador de sistema operativo
 *
 * Ejecuta:
 *   1. Scheduler FIFO
 *   2. Scheduler Round Robin (quantum=2)
 *   3. Scheduler SJF
 *   4. Administración de memoria (First Fit, Best Fit, Worst Fit)
 *   5. Coalescencia y fragmentación
 *   6. Algoritmos (backtracking, DP knapsack)
 */

#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "scheduler.h"
#include "memory_manager.h"
#include "algorithms.h"

/* ── Demo Schedulers ── */
static void demo_schedulers() {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║        DEMO SCHEDULERS               ║\n");
    printf("╚══════════════════════════════════════╝\n");

    /* Procesos de ejemplo del proyecto */
    int pids[]   = {1, 2, 3};
    int bursts[] = {5, 3, 4};
    int n = 3;

    /* ── FIFO ── */
    {
        Scheduler* s = scheduler_create_fifo();
        for (int i = 0; i < n; i++)
            scheduler_add_process(s, pids[i], bursts[i]);
        scheduler_run(s);
        scheduler_print_results(s);
        scheduler_destroy(s);
    }

    /* ── Round Robin (quantum=2) ── */
    {
        Scheduler* s = scheduler_create_rr(2);
        for (int i = 0; i < n; i++)
            scheduler_add_process(s, pids[i], bursts[i]);
        scheduler_run_rr(s);
        scheduler_print_results(s);
        scheduler_destroy(s);
    }

    /* ── SJF ── */
    {
        Scheduler* s = scheduler_create_sjf();
        for (int i = 0; i < n; i++)
            scheduler_add_process(s, pids[i], bursts[i]);
        scheduler_run_sjf(s);
        scheduler_print_results(s);
        scheduler_destroy(s);
    }
}

/* ── Demo Memoria ── */
static void demo_memory() {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║      DEMO ADMINISTRACIÓN MEMORIA     ║\n");
    printf("╚══════════════════════════════════════╝\n");

    MemoryManager* mm = mm_create(1024);
    printf("\n--- Estado inicial ---\n");
    mm_print(mm);

    /* Asignar procesos */
    int a1 = greedy_first_fit(mm, 1, 200);
    int a2 = greedy_first_fit(mm, 2, 300);
    int a3 = greedy_first_fit(mm, 3, 100);
    int a4 = greedy_first_fit(mm, 4, 150);
    printf("\n--- Tras asignar P1(200), P2(300), P3(100), P4(150) ---\n");
    mm_print(mm);
    (void)a1; (void)a2; (void)a3; (void)a4;

    /* Liberar P2 y P4 → fragmentación */
    mm_free(mm, 2);
    mm_free(mm, 4);
    printf("\n--- Tras liberar P2 y P4 (fragmentación) ---\n");
    mm_print(mm);

    /* Coalescencia */
    mm_coalesce(mm);
    printf("\n--- Tras coalescencia ---\n");
    mm_print(mm);

    /* Compactación */
    mm_compact(mm);
    printf("\n--- Tras compactación ---\n");
    mm_print(mm);

    mm_destroy(mm);
}

/* ── Demo Algoritmos ── */
static void demo_algorithms() {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║         DEMO ALGORITMOS              ║\n");
    printf("╚══════════════════════════════════════╝\n");

    /* Procesos para mochila */
    Process procs[5];
    procs[0] = process_create(1, 5, 1, 100);
    procs[1] = process_create(2, 3, 2, 200);
    procs[2] = process_create(3, 7, 1, 150);
    procs[3] = process_create(4, 2, 3, 80);
    procs[4] = process_create(5, 6, 2, 300);
    int n = 5;

    /* DP Bottom-Up */
    int capacity = 500;
    int result = dp_knapsack_bottomup(procs, n, capacity);
    printf("\nDP Knapsack (Bottom-Up): max %d procesos en %d MB\n", result, capacity);

    /* DP Top-Down */
    int** memo = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        memo[i] = (int*)malloc((capacity + 1) * sizeof(int));
        for (int j = 0; j <= capacity; j++)
            memo[i][j] = -1;
    }
    int result_td = dp_knapsack_topdown(procs, n, capacity, memo);
    printf("DP Knapsack (Top-Down):  max %d procesos en %d MB\n", result_td, capacity);
    for (int i = 0; i <= n; i++) free(memo[i]);
    free(memo);

    /* Backtracking */
    printf("\nBacktracking de asignación de memoria:\n");
    MemoryManager* mm = mm_create(400);
    Process bt_procs[3];
    bt_procs[0] = process_create(10, 4, 1, 100);
    bt_procs[1] = process_create(11, 2, 1, 250);
    bt_procs[2] = process_create(12, 3, 1, 80);
    backtrack_allocate(mm, bt_procs, 3, 0);
    for (int i = 0; i < 3; i++)
        printf("  P%d: %s\n", bt_procs[i].pid, process_state_str(bt_procs[i].state));
    mm_destroy(mm);
}

/* ── Main ── */
int main(int argc, char* argv[]) {
    printf("╔══════════════════════════════════════╗\n");
    printf("║   MINI OS SIMULATOR — UNAM 2026-2    ║\n");
    printf("╚══════════════════════════════════════╝\n");

    int mode = 0;
    if (argc > 1) mode = atoi(argv[1]);

    switch (mode) {
        case 1:  demo_schedulers(); break;
        case 2:  demo_memory();     break;
        case 3:  demo_algorithms(); break;
        default:
            demo_schedulers();
            demo_memory();
            demo_algorithms();
            break;
    }

    printf("\n✓ Simulación completada.\n");
    return 0;
}