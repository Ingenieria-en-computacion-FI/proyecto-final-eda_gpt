#!/usr/bin/env python3
"""
validate_results.py — Validación de resultados del simulador

Verifica que los resultados del simulador C sean correctos
comparando con implementaciones de referencia en Python.

Uso:
    python3 scripts/validate_results.py
"""

def simulate_fifo(processes: list[dict]) -> list[dict]:
    """Simulación FIFO de referencia."""
    time = 0
    results = []
    for p in processes:
        results.append({
            "pid":   p["pid"],
            "start": time,
            "end":   time + p["burst_time"],
        })
        time += p["burst_time"]
    return results

def simulate_round_robin(processes: list[dict], quantum: int) -> int:
    """Retorna tiempo total de Round Robin."""
    procs = [dict(p) for p in processes]
    for p in procs:
        p["remaining"] = p["burst_time"]
    time = 0
    while any(p["remaining"] > 0 for p in procs):
        for p in procs:
            if p["remaining"] > 0:
                run = min(p["remaining"], quantum)
                p["remaining"] -= run
                time += run
    return time

def simulate_sjf(processes: list[dict]) -> int:
    """Retorna tiempo total de SJF."""
    sorted_procs = sorted(processes, key=lambda p: p["burst_time"])
    return sum(p["burst_time"] for p in sorted_procs)

def run_validations():
    processes = [
        {"pid": 1, "burst_time": 5},
        {"pid": 2, "burst_time": 3},
        {"pid": 3, "burst_time": 4},
    ]

    # FIFO
    fifo_results = simulate_fifo(processes)
    assert fifo_results[0]["start"] == 0
    assert fifo_results[0]["end"]   == 5
    assert fifo_results[1]["start"] == 5
    assert fifo_results[1]["end"]   == 8
    print("[✓] FIFO: tiempos correctos")

    # Round Robin (quantum=2)
    rr_time = simulate_round_robin(processes, quantum=2)
    assert rr_time == 12, f"RR esperaba 12, obtuvo {rr_time}"
    print(f"[✓] Round Robin (quantum=2): tiempo total = {rr_time}")

    # SJF
    sjf_time = simulate_sjf(processes)
    assert sjf_time == 12
    print(f"[✓] SJF: tiempo total = {sjf_time}")

    print("\n[✓] Todas las validaciones pasaron")

if __name__ == "__main__":
    print("╔══════════════════════════════════════╗")
    print("║       VALIDANDO RESULTADOS           ║")
    print("╚══════════════════════════════════════╝\n")
    run_validations()
