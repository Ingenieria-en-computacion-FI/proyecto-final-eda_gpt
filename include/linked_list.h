#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/* Lista ligada simple ordenada por PID
   Uso: almacenar procesos terminados ordenados */

typedef struct LLNode {
    int pid;
    int burst_time;
    struct LLNode* next;
} LLNode;

typedef struct LinkedList {
    LLNode* head;
    int size; // Número de nodos en la lista
} LinkedList;

LinkedList* ll_create();
void        ll_insert_sorted(LinkedList* list, int pid, int burst_time);
int         ll_remove(LinkedList* list, int pid);
LLNode*     ll_find(LinkedList* list, int pid);
void        ll_print(LinkedList* list);
void        ll_destroy(LinkedList* list);

#endif
