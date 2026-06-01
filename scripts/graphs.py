#!/usr/bin/env python3
"""
graphs.py — Generación de gráficas de benchmarking

Lee reports/csv/benchmark.csv y genera:
  1. Tiempo de ejecución por modo y tamaño de entrada
  2. Comparativa de schedulers
  3. Curvas de complejidad teórica vs medida
pip list
Uso:
    python3 scripts/graphs.py
"""

import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

CSV_IN   = "reports/csv/benchmark.csv"
PNG_DIR  = "reports/png"


def load_data(path: str) -> pd.DataFrame:
    if not os.path.exists(path):
        print(f"[ERROR] No se encontró {path}")
        print("        Ejecuta benchmark.py primero.")
        raise SystemExit(1)
    return pd.read_csv(path)


def plot_time_by_mode(df: pd.DataFrame):
    """Gráfica: tiempo de ejecución por modo."""
    fig, ax = plt.subplots(figsize=(10, 6))
    for mode, group in df.groupby("modo"):
        ax.plot(group["n"], group["tiempo_s"], marker="o", label=mode)

    ax.set_title("Tiempo de Ejecución por Modo", fontsize=14)
    ax.set_xlabel("Número de Procesos (n)")
    ax.set_ylabel("Tiempo (segundos)")
    ax.legend()
    ax.grid(True, alpha=0.3)
    ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{int(x):,}"))

    path = os.path.join(PNG_DIR, "tiempo_por_modo.png")
    fig.savefig(path, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"[✓] {path}")


def plot_complexity_comparison(df: pd.DataFrame):
    """Gráfica: tiempo medido vs curvas teóricas O(n), O(n log n)."""
    fig, axes = plt.subplots(1, 2, figsize=(14, 6))
    modes = df["modo"].unique()

    for ax, mode in zip(axes, modes[:2]):
        group = df[df["modo"] == mode].copy()
        n_vals = group["n"].values
        t_vals = group["tiempo_s"].values

        # Normalizar para comparar forma
        if t_vals.max() > 0:
            t_norm = t_vals / t_vals.max()
        else:
            t_norm = t_vals

        n_norm = n_vals / n_vals.max()
        nlogn  = (n_vals * np.log2(np.maximum(n_vals, 1)))
        nlogn  = nlogn / nlogn.max()

        ax.plot(n_vals, t_norm,  "o-",  label="Medido",      color="steelblue")
        ax.plot(n_vals, n_norm,  "--",  label="O(n)",         color="orange", alpha=0.7)
        ax.plot(n_vals, nlogn,   "-.",  label="O(n log n)",   color="green",  alpha=0.7)

        ax.set_title(f"{mode} — Complejidad", fontsize=12)
        ax.set_xlabel("n")
        ax.set_ylabel("Tiempo normalizado")
        ax.legend()
        ax.grid(True, alpha=0.3)

    path = os.path.join(PNG_DIR, "complejidad_comparativa.png")
    fig.tight_layout()
    fig.savefig(path, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"[✓] {path}")


def plot_scheduler_comparison(df: pd.DataFrame):
    """Gráfica de barras: comparativa de schedulers para n=1000."""
    sched_df = df[df["modo"] == "FIFO/RR/SJF"]
    if sched_df.empty:
        return

    fig, ax = plt.subplots(figsize=(8, 5))
    ax.bar(sched_df["n"].astype(str), sched_df["tiempo_s"], color=["#4e79a7", "#f28e2b", "#e15759", "#76b7b2"])
    ax.set_title("Tiempo de Simulación Schedulers por Tamaño", fontsize=13)
    ax.set_xlabel("Número de Procesos")
    ax.set_ylabel("Tiempo (s)")
    ax.grid(axis="y", alpha=0.3)

    path = os.path.join(PNG_DIR, "schedulers_comparativa.png")
    fig.savefig(path, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"[✓] {path}")


if __name__ == "__main__":
    print("========================================")
    print("          GENERANDO GRÁFICAS            ")
    print("========================================\n")

    os.makedirs(PNG_DIR, exist_ok=True)
    df = load_data(CSV_IN)

    plot_time_by_mode(df)
    plot_complexity_comparison(df)
    plot_scheduler_comparison(df)

    print("\n[✓] Gráficas guardadas en reports/png/")