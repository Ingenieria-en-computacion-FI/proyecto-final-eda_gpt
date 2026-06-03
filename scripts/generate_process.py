#!/usr/bin/env python3
"""
generate_processes.py — Generador de procesos aleatorios para el simulador

Genera procesos simulados con valores aleatorios reproducibles (semilla fija)
y los exporta a CSV para que el simulador C los lea como entrada.

Cada proceso contiene los mismos campos que el struct Process de C:
    pid, burst_time, priority, memory_required

Flujo:
    1. Genera n procesos con random.seed(42) para resultados reproducibles
    2. Imprime la tabla de procesos en consola
    3. Exporta los procesos a un archivo CSV

Salida:
    data/inputs/processes.csv  — leído por el simulador C (./bin/main)

Uso:
    python3 scripts/generate_processes.py              # 10 procesos, ruta default
    python3 scripts/generate_processes.py 500          # 500 procesos, ruta default
    python3 scripts/generate_processes.py 500 data/inputs/test.csv  # ruta custom
"""

import random
import csv
import os
import sys

def generate_processes(n: int = 10, seed: int = 42) -> list[dict]:
    random.seed(seed)
    processes = []
    for i in range(n):
        processes.append({
            "pid":             i + 1,
            "burst_time":      random.randint(1, 20),
            "priority":        random.randint(1, 5),
            "memory_required": random.randint(10, 500),
        })
    return processes

def save_to_csv(processes: list[dict], path: str):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=processes[0].keys())
        writer.writeheader()
        writer.writerows(processes)
    print(f"[✓] {len(processes)} procesos guardados en {path}")

def print_processes(processes: list[dict]):
    print(f"\n{'PID':<6} {'Burst':<8} {'Prioridad':<12} {'Memoria':<10}")
    print("-" * 40)
    for p in processes:
        print(f"{p['pid']:<6} {p['burst_time']:<8} {p['priority']:<12} {p['memory_required']:<10}")

if __name__ == "__main__":
    n       = int(sys.argv[1]) if len(sys.argv) > 1 else 10
    outfile = sys.argv[2] if len(sys.argv) > 2 else "data/inputs/processes.csv"

    processes = generate_processes(n)
    print_processes(processes)
    save_to_csv(processes, outfile)
