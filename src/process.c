#include <stdio.h>
#include "../include/process.h" 

Process process_create(int pid, int burst_time, int priority, int memory_required) {
    Process p;
    p.pid             = pid;
    p.burst_time      = burst_time;
    p.remaining_time  = burst_time;
    p.priority        = priority;
    p.memory_required = memory_required;
    p.state           = READY;
    return p;
}

const char* process_state_str(ProcessState state) {
    switch (state) {
        case READY:    return "READY";
        case RUNNING:  return "RUNNING";
        case BLOCKED:  return "BLOCKED";
        case FINISHED: return "FINISHED";
        default:       return "UNKNOWN";
    }
} 