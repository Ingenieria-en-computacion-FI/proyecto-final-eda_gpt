#!/usr/bin/env python3
"""
benchmark.py — Benchmarking del simulador Mini-OS

Mide el tiempo de ejecución real del simulador C lanzándolo como subproceso
para diferentes tamaños de entrada (n = 10, 50, 100, 500, 1000, 5000) y
tres modos de operación (schedulers, memoria, algoritmos).

Flujo:
    1. Ejecuta ./bin/main con subprocess.run() por cada combinación (modo, n)
    2. Mide el tiempo transcurrido con time.perf_counter()
    3. Acumula los resultados en una lista de diccionarios
    4. Exporta los resultados a reports/csv/benchmark.csv

Salida:
    reports/csv/benchmark.csv  — leído por graphs.py para generar gráficas

Uso:
    python3 scripts/benchmark.py
    
Requisitos:
    Compilar el simulador C antes de ejecutar: make
"""

import subprocess
import time
import csv
import os
import sys

BINARY  = "./bin/main"
CSV_OUT = "reports/csv/benchmark.csv"


def run_simulation(mode: int, n: int) -> float:
    """Ejecuta el simulador y mide tiempo en segundos."""
    try:
        start = time.perf_counter()
        result = subprocess.run(
            [BINARY, str(mode), str(n)],
            capture_output=True,
            text=True,
            timeout=30,
        )
        end = time.perf_counter()
        if result.returncode != 0:
            print(f"  [WARN] Simulador retornó código {result.returncode}", file=sys.stderr)
        return round(end - start, 6)
    except FileNotFoundError:
        print(f"[ERROR] Binario no encontrado: {BINARY}", file=sys.stderr)
        print("        Ejecuta 'make' primero.", file=sys.stderr)
        sys.exit(1)
    except subprocess.TimeoutExpired:
        print("[WARN] Timeout en simulación", file=sys.stderr)
        return -1.0


def run_benchmark() -> list[dict]:
    sizes   = [10, 50, 100, 500, 1000, 5000]
    modes   = {1: "FIFO/RR/SJF", 2: "Memoria", 3: "Algoritmos"}
    results = []

    print(f"\n{'Modo':<20} {'N':<8} {'Tiempo (s)':<14}")
    print("-" * 44)

    for mode, label in modes.items():
        for n in sizes:
            elapsed = run_simulation(mode, n)
            row = {"modo": label, "n": n, "tiempo_s": elapsed}
            results.append(row)
            print(f"{label:<20} {n:<8} {elapsed:<14.6f}")

    return results


def save_results(results: list[dict], path: str):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=["modo", "n", "tiempo_s"])
        writer.writeheader()
        writer.writerows(results)
    print(f"\n[✓] Resultados guardados en {path}")


if __name__ == "__main__":
    print("========================================")
    print("          BENCHMARKING MINI-OS          ")
    print("========================================\n")

    results = run_benchmark()
    save_results(results, CSV_OUT)
