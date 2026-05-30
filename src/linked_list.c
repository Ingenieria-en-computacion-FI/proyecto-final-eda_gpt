/*
 * linked_list.c — Lista ligada simple ordenada por PID
 *
 * Complejidad temporal:
 *   insert_sorted : O(n)  — recorre hasta encontrar posición
 *   remove        : O(n)  — búsqueda por PID
 *   find          : O(n)
 *
 * Complejidad espacial: O(n)
 *
 * Uso en el sistema: almacenar procesos terminados ordenados por PID.
 */

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

/* ── Crear lista vacía ── */
LinkedList* ll_create() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (!list) {
        fprintf(stderr, "ll_create: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

/* ── Insertar ordenado por PID: O(n) ── */
void ll_insert_sorted(LinkedList* list, int pid, int burst_time) {
    LLNode* node = (LLNode*)malloc(sizeof(LLNode));
    if (!node) {
        fprintf(stderr, "ll_insert_sorted: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    node->pid        = pid;
    node->burst_time = burst_time;
    node->next       = NULL;

    /* Caso: lista vacía o pid menor que el primero */
    if (list->head == NULL || pid < list->head->pid) {
        node->next  = list->head;
        list->head  = node;
        list->size++;
        return;
    }

    /* Buscar posición */
    LLNode* cur = list->head;
    while (cur->next != NULL && cur->next->pid < pid)
        cur = cur->next;

    node->next = cur->next;
    cur->next  = node;
    list->size++;
}

/* ── Eliminar por PID: O(n) ── */
int ll_remove(LinkedList* list, int pid) {
    if (list->head == NULL) return 0;

    if (list->head->pid == pid) {
        LLNode* tmp = list->head;
        list->head  = tmp->next;
        free(tmp);
        list->size--;
        return 1;
    }

    LLNode* cur = list->head;
    while (cur->next != NULL && cur->next->pid != pid)
        cur = cur->next;

    if (cur->next == NULL) return 0;  /* No encontrado */

    LLNode* tmp = cur->next;
    cur->next   = tmp->next;
    free(tmp);
    list->size--;
    return 1;
}

/* ── Buscar por PID: O(n) ── */
LLNode* ll_find(LinkedList* list, int pid) {
    LLNode* cur = list->head;
    while (cur) {
        if (cur->pid == pid) return cur;
        cur = cur->next;
    }
    return NULL;
}

/* ── Imprimir lista ── */
void ll_print(LinkedList* list) {
    printf("LinkedList [PID ordenado, size=%d]: ", list->size);
    LLNode* cur = list->head;
    while (cur) {
        printf("(pid=%d, burst=%d) ", cur->pid, cur->burst_time);
        cur = cur->next;
    }
    printf("\n");
}

/* ── Destruir lista: O(n) ── */
void ll_destroy(LinkedList* list) {
    LLNode* cur = list->head;
    while (cur) {
        LLNode* tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(list);
} 