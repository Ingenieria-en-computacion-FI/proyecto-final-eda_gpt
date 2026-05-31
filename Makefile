CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -Iinclude -g

# Todos los .c de src (recursivo)
SRC     = src/process.c \
          src/stack.c \
          src/queue.c \
          src/circular_queue.c \
          src/linked_list.c \
          src/memory/first_fit.c \
          src/scheduler/fifo.c \
          src/scheduler/round_robin.c \
          src/scheduler/sjf.c \
          src/algorithms/algorithms.c

MAIN_SRC  = src/main.c
TEST_SRC  = tests/main.c \
            tests/test_stack.c \
            tests/test_queue.c \
            tests/test_circular_queue.c \
            tests/test_memory_manager.c \
            tests/test_scheduler.c

# ── Targets ────────────────────────────────────

all: bin/main

bin/main: $(SRC) $(MAIN_SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) $(MAIN_SRC) -o bin/main

run: bin/main
	./bin/main

run-schedulers: bin/main
	./bin/main 1

run-memory: bin/main
	./bin/main 2

run-algorithms: bin/main
	./bin/main 3

test: bin/tests
	./bin/tests

bin/tests: $(SRC) $(TEST_SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) -o bin/tests

benchmark: bin/main
	python3 scripts/benchmark.py

graphs: 
	python3 scripts/graphs.py

validate:
	python3 scripts/validate_results.py

generate-processes:
	python3 scripts/generate_processes.py

clean:
	rm -rf bin/*

.PHONY: all run run-schedulers run-memory run-algorithms test benchmark graphs validate generate-processes clean 