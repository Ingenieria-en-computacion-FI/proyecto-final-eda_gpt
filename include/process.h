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
    BLOCKED, // si no hay memoria
    FINISHED
} ProcessState;

typedef struct {
    int pid;
    int burst_time; // tiempo total que necesita para ejecutarse
    int remaining_time; // tiempo que le queda para terminar (se actualiza al ejecutar)
    int priority;
    int memory_required;
    ProcessState state;
} Process;

Process process_create(int pid, int burst_time, int priority, int memory_required);
const char* process_state_str(ProcessState state); // Convierte el estado a string para imprimirlo

#endif 