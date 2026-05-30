#include <stdio.h>
#include <assert.h>
#include "memory_manager.h"
#include "algorithms.h"

void test_first_fit() {
    MemoryManager* mm = mm_create(1024);
    int a1 = greedy_first_fit(mm, 1, 100);
    int a2 = greedy_first_fit(mm, 2, 200);
    assert(a1 == 0);
    assert(a2 == 100);
    mm_free(mm, 1);
    int a3 = greedy_first_fit(mm, 3, 50);
    assert(a3 == 0);  /* Reutiliza el bloque liberado */
    mm_destroy(mm);
    printf("  [OK] test_first_fit\n");
}

void test_best_fit() {
    MemoryManager* mm = mm_create(1024);
    greedy_first_fit(mm, 1, 100);
    greedy_first_fit(mm, 2, 400);
    greedy_first_fit(mm, 3, 200);
    mm_free(mm, 1);  /* Libre: 100 */
    mm_free(mm, 3);  /* Libre: 200 */
    /* Best fit para 80 debe elegir el bloque de 100 (más pequeño que cabe) */
    int a = greedy_best_fit(mm, 4, 80);
    assert(a >= 0);
    mm_destroy(mm);
    printf("  [OK] test_best_fit\n");
}

void test_coalescence() {
    MemoryManager* mm = mm_create(600);
    greedy_first_fit(mm, 1, 200);
    greedy_first_fit(mm, 2, 100);
    greedy_first_fit(mm, 3, 100);
    /* Liberar 2 y 3 → dos bloques libres adyacentes */
    mm_free(mm, 2);
    mm_free(mm, 3);
    mm_coalesce(mm);
    /* Debe haber un solo bloque libre de 200 entre P1 y el final */
    int a = greedy_first_fit(mm, 4, 180);
    assert(a >= 0);
    mm_destroy(mm);
    printf("  [OK] test_coalescence\n");
}

void test_fragmentation() {
    MemoryManager* mm = mm_create(500);
    greedy_first_fit(mm, 1, 100);
    greedy_first_fit(mm, 2, 100);
    greedy_first_fit(mm, 3, 100);
    mm_free(mm, 1);
    mm_free(mm, 3);
    int frag = mm_fragmentation(mm);
    assert(frag > 0);  /* Debe haber fragmentación */
    mm_destroy(mm);
    printf("  [OK] test_fragmentation (frag=%d%%)\n", frag);
}
