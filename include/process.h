#ifndef PROCESS_H
#define PROCESS_H

/*
Un enum es una lista de valores con nombre.
Internamente son números — READY=0, RUNNING=1, 
BLOCKED=2, FINISHED=3 — pero usamos los nombres
para que el código sea legible.
*/

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    FINISHED
} ProcessState;

typedef struct {
    int pid;
    int burst_time;
    int remaining_time;
    int priority;
    int memory_required;
    ProcessState state;
} Process;

Process process_create(int pid, int burst_time, int priority, int memory_required);
const char* process_state_str(ProcessState state);

#endif 