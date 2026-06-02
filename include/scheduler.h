#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "queue.h"
#include "circular_queue.h"
#include "../include/linked_list.h"
#include "stack.h"

#define MAX_PROCESSES 256   

/*
Internamente SCHED_FIFO=0, SCHED_ROUND_ROBIN=1, SCHED_SJF=2
*/
typedef enum {
    SCHED_FIFO,
    SCHED_ROUND_ROBIN,
    SCHED_SJF
} SchedulerType;

typedef struct {
    SchedulerType type;
    Process processes[MAX_PROCESSES];
    int count;
    int quantum;          /* Solo Round Robin */
    int current_time;
    int completed;
    Queue* fifo_queue;       /* FIFO */
    CircularQueue* rr_queue;        /* Round Robin */
    LinkedList* finished_list;    /* Procesos terminados ordenados por PID */
    Stack*  history;          /* Historial de ejecución */
} Scheduler;

Scheduler* scheduler_create_fifo();
Scheduler* scheduler_create_rr(int quantum);
Scheduler* scheduler_create_sjf();
void scheduler_add_process(Scheduler* s, int pid, int burst_time);
void scheduler_add_process_full(Scheduler* s, Process p);
int scheduler_next(Scheduler* s);       /* Retorna PID del siguiente */
void scheduler_run(Scheduler* s);        /* Ejecuta simulación FIFO */
void scheduler_run_rr(Scheduler* s);     /* Ejecuta simulación Round Robin */
void scheduler_run_sjf(Scheduler* s);    /* Ejecuta simulación SJF */
void scheduler_print_results(Scheduler* s);
void scheduler_destroy(Scheduler* s);

#endif