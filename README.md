[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/6f5mpNsA)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=24029549&assignment_repo_type=AssignmentRepo)
# Mini OS Project

Proyecto integrador de:

- estructuras de datos,
- algoritmos,
- complejidad,
- benchmarking,
- integración C/Python.

## Compilación

```bash
make
```

## Ejecutar

```bash
./bin/main
```

## Ejecutar pruebas

```bash
make test
```


## Ejecutar benchmarking
```bash
python3 scripts/benchmarking.py
```

# Makefile Inicial
```makefile
CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -Iinclude

SRC=$(wildcard src/**/*.c src/*.c)
TESTS=$(wildcard tests/*.c)

all:
	$(CC) $(CFLAGS) $(SRC) -o bin/main

run:
	./bin/main

test:
	$(CC) $(CFLAGS) $(TESTS) $(SRC) -o bin/tests
	./bin/tests

clean:
	rm -f bin/*

```

## Requerimientos
 - matplotlib
 - numpy
 - pandas


## Estructura de proyecto
```
mini-os-project/
│
├── README.md
├── Makefile
├── .gitignore
├── requirements.txt
│
├── bin/
│
├── docs/
│   ├── bitacora_ia.md
│   ├── reporte.tex
│   └── arquitectura/
│       ├── scheduler.md
│       ├── memoria.md
│       └── estructuras.md
│
├── data/
│   ├── inputs/
│   ├── outputs/
│   ├── benchmarks/
│   └── logs/
│
├── include/
│   ├── process.h
│   ├── stack.h
│   ├── queue.h
│   ├── circular_queue.h
│   ├── linked_list.h
│   ├── doubly_linked_list.h
│   ├── scheduler.h
│   ├── memory_manager.h
│   └── algorithms.h
│
├── src/
│   ├── main.c
│   │
│   ├── stack.c
│   ├── queue.c
│   ├── circular_queue.c
│   ├── linked_list.c
│   ├── doubly_linked_list.c
│   │
│   ├── scheduler/
│   │   ├── fifo.c
│   │   ├── round_robin.c
│   │   └── sjf.c
│   │
│   ├── memory/
│   │   ├── first_fit.c
│   │   ├── best_fit.c
│   │   ├── worst_fit.c
│   │   ├── coalescence.c
│   │   └── compactation.c
│   │
│   ├── algorithms/
│   │   ├── brute_force.c
│   │   ├── greedy.c
│   │   ├── backtracking.c
│   │   ├── divide_conquer.c
│   │   ├── dp_topdown.c
│   │   ├── dp_bottomup.c
│   │   └── incremental.c
│   │
│   └── utils/
│       ├── logger.c
│       ├── timer.c
│       └── parser.c
│
├── tests/
│   ├── test_stack.c
│   ├── test_queue.c
│   ├── test_circular_queue.c
│   ├── test_memory_manager.c
│   ├── test_scheduler.c
│   └── integration_tests.c
│
├── scripts/
│   ├── benchmark.py
│   ├── generate_processes.py
│   ├── graphs.py
│   └── validate_results.py
│
├── reports/
│   ├── csv/
│   ├── png/
│   └── pdf/
│
└── gtk/
    ├── optional_ui.c
    └── optional_ui.h
```
