#ifndef PROCESS_H
#define PROCESS_H

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