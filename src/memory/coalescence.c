/*
 * coalescence.c — Coalescencia de bloques libres
 *
 * La implementación principal vive en first_fit.c (mm_coalesce).
 * Este archivo documenta el algoritmo de forma aislada.
 *
 * Algoritmo:
 *   Recorre la lista doblemente ligada.
 *   Cuando encuentra dos bloques libres consecutivos,
 *   los fusiona en uno solo absorbiendo el segundo.
 *
 *   Ejemplo:
 *     Antes : [P1:200][Libre:100][Libre:150][P2:300]
 *     Paso 1: fusionar Libre:100 + Libre:150 → Libre:250
 *     Después: [P1:200][Libre:250][P2:300]
 *
 * Complejidad:
 *   T(n) = O(n)  — un solo recorrido lineal
 *   S(n) = O(1)  — sin estructuras auxiliares
 *
 * Recurrencia:
 *   T(n) = T(n-1) + c  =>  T(n) = O(n)
 */

#include "memory_manager.h"
/* La función mm_coalesce está implementada en first_fit.c */
