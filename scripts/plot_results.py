#!/usr/bin/env python3
"""
Script to generate visualization graphs from benchmark CSV files.
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
from pathlib import Path

# Set style
plt.style.use('seaborn-v0_8-darkgrid')
plt.rcParams['figure.figsize'] = (12, 8)
plt.rcParams['font.size'] = 10

# Output directory for plots
PLOTS_DIR = Path('results/plots')
PLOTS_DIR.mkdir(parents=True, exist_ok=True)

def parse_operations_benchmark(csv_path):
    """Parse the operations benchmark CSV file."""
    data = {
        'container': [],
        'data_size': [],
        'op1_count': [],
        'op2_count': [],
        'op3_count': [],
        'total_ops': [],
        'ops_per_sec': [],
        'memory_mb': []
    }
    
    current_size = None
    
    with open(csv_path, 'r') as f:
        for line in f:
            line = line.strip()
            
            if line.startswith('=== Data Size:'):
                current_size = int(line.split(':')[1].strip().replace('===', '').strip())
                continue
            
            if not line or line.startswith('Container,'):
                continue
            
            parts = [p.strip() for p in line.split(',')]
            if len(parts) >= 7 and current_size is not None:
                try:
                    data['container'].append(parts[0])
                    data['data_size'].append(current_size)
                    data['op1_count'].append(int(parts[1]))
                    data['op2_count'].append(int(parts[2]))
                    data['op3_count'].append(int(parts[3]))
                    data['total_ops'].append(int(parts[4]))
                    data['ops_per_sec'].append(float(parts[5]))
                    data['memory_mb'].append(float(parts[6]))
                except ValueError:
                    continue
    
    return pd.DataFrame(data)

def plot_operations_per_second(df):
    """Plot operations per second vs database size for different containers."""
    fig, ax = plt.subplots(figsize=(12, 8))
    
    containers = df['container'].unique()
    colors = plt.cm.tab10(np.linspace(0, 1, len(containers)))
    markers = ['o', 's', '^', 'D']
    
    for i, container in enumerate(containers):
        container_data = df[df['container'] == container]
        ax.plot(container_data['data_size'], container_data['ops_per_sec'],
                marker=markers[i % len(markers)], linewidth=2, markersize=10,
                label=container, color=colors[i])
    
    ax.set_xlabel('Database Size (records)', fontsize=14, fontweight='bold')
    ax.set_ylabel('Operations per Second', fontsize=14, fontweight='bold')
    ax.set_title('Database Performance: Operations per Second vs Size\n(10 seconds benchmark)', 
                 fontsize=16, fontweight='bold', pad=20)
    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.grid(True, alpha=0.3, which='both')
    ax.legend(fontsize=10, loc='best', framealpha=0.9)
    
    # Add value labels on points
    for container in containers:
        container_data = df[df['container'] == container]
        for _, row in container_data.iterrows():
            ax.annotate(f'{row["ops_per_sec"]:.0f}',
                       xy=(row['data_size'], row['ops_per_sec']),
                       xytext=(5, 5), textcoords='offset points',
                       fontsize=8, alpha=0.7)
    
    plt.tight_layout()
    plt.savefig(PLOTS_DIR / 'operations_per_second.png', dpi=300, bbox_inches='tight')
    print(f"Saved: {PLOTS_DIR / 'operations_per_second.png'}")
    plt.close()

def plot_memory_usage(df):
    """Plot memory usage vs database size for different containers."""
    fig, ax = plt.subplots(figsize=(12, 8))
    
    containers = df['container'].unique()
    colors = plt.cm.tab10(np.linspace(0, 1, len(containers)))
    markers = ['o', 's', '^', 'D']
    
    for i, container in enumerate(containers):
        container_data = df[df['container'] == container]
        ax.plot(container_data['data_size'], container_data['memory_mb'],
                marker=markers[i % len(markers)], linewidth=2, markersize=10,
                label=container, color=colors[i])
    
    ax.set_xlabel('Database Size (records)', fontsize=14, fontweight='bold')
    ax.set_ylabel('Memory Usage (MB)', fontsize=14, fontweight='bold')
    ax.set_title('Memory Consumption vs Database Size', 
                 fontsize=16, fontweight='bold', pad=20)
    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.grid(True, alpha=0.3, which='both')
    ax.legend(fontsize=10, loc='best', framealpha=0.9)
    
    # Add value labels on points
    for container in containers:
        container_data = df[df['container'] == container]
        for _, row in container_data.iterrows():
            ax.annotate(f'{row["memory_mb"]:.2f}',
                       xy=(row['data_size'], row['memory_mb']),
                       xytext=(5, 5), textcoords='offset points',
                       fontsize=8, alpha=0.7)
    
    plt.tight_layout()
    plt.savefig(PLOTS_DIR / 'memory_usage.png', dpi=300, bbox_inches='tight')
    print(f"Saved: {PLOTS_DIR / 'memory_usage.png'}")
    plt.close()

def plot_total_operations(df):
    """Plot total operations performed in 10 seconds."""
    fig, ax = plt.subplots(figsize=(12, 8))
    
    containers = df['container'].unique()
    data_sizes = df['data_size'].unique()
    
    x = np.arange(len(data_sizes))
    width = 0.2
    multiplier = 0
    
    colors = plt.cm.tab10(np.linspace(0, 1, len(containers)))
    
    for i, container in enumerate(containers):
        container_data = df[df['container'] == container]
        total_ops = [container_data[container_data['data_size'] == size]['total_ops'].values[0] 
                     for size in data_sizes]
        offset = width * multiplier
        ax.bar(x + offset, total_ops, width, label=container, color=colors[i], alpha=0.8)
        
        # Add value labels on bars
        for j, (pos, val) in enumerate(zip(x + offset, total_ops)):
            ax.text(pos, val, f'{val:,}', ha='center', va='bottom', fontsize=8, rotation=0)
        
        multiplier += 1
    
    ax.set_xlabel('Database Size (records)', fontsize=14, fontweight='bold')
    ax.set_ylabel('Total Operations in 10 Seconds', fontsize=14, fontweight='bold')
    ax.set_title('Total Operations Performed (10 seconds benchmark)', 
                 fontsize=16, fontweight='bold', pad=20)
    ax.set_xticks(x + width * (len(containers) - 1) / 2)
    ax.set_xticklabels(data_sizes)
    ax.set_yscale('log')
    ax.legend(fontsize=10, loc='best', framealpha=0.9)
    ax.grid(True, alpha=0.3, axis='y')
    
    plt.tight_layout()
    plt.savefig(PLOTS_DIR / 'total_operations.png', dpi=300, bbox_inches='tight')
    print(f"Saved: {PLOTS_DIR / 'total_operations.png'}")
    plt.close()

def plot_operation_breakdown(df):
    """Plot breakdown of operation types for each container at different sizes."""
    containers = df['container'].unique()
    
    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    axes = axes.flatten()
    
    data_sizes = sorted(df['data_size'].unique())
    
    for idx, size in enumerate(data_sizes):
        if idx >= len(axes):
            break
            
        ax = axes[idx]
        size_data = df[df['data_size'] == size]
        
        x = np.arange(len(containers))
        width = 0.25
        
        op1_counts = [size_data[size_data['container'] == c]['op1_count'].values[0] 
                      for c in containers]
        op2_counts = [size_data[size_data['container'] == c]['op2_count'].values[0] 
                      for c in containers]
        op3_counts = [size_data[size_data['container'] == c]['op3_count'].values[0] 
                      for c in containers]
        
        ax.bar(x - width, op1_counts, width, label='Operation 1 (Insert)', alpha=0.8, color='#1f77b4')
        ax.bar(x, op2_counts, width, label='Operation 2 (Update)', alpha=0.8, color='#ff7f0e')
        ax.bar(x + width, op3_counts, width, label='Operation 3 (Search)', alpha=0.8, color='#2ca02c')
        
        ax.set_ylabel('Operation Count', fontsize=12, fontweight='bold')
        ax.set_title(f'Operation Breakdown - Database Size: {size:,} records', 
                     fontsize=12, fontweight='bold')
        ax.set_xticks(x)
        ax.set_xticklabels([c.replace('Hybrid (unordered_map + 2x multimap indices)', 'Hybrid') 
                            for c in containers], rotation=15, ha='right')
        ax.legend(fontsize=9)
        ax.grid(True, alpha=0.3, axis='y')
        ax.set_yscale('log')
    
    plt.tight_layout()
    plt.savefig(PLOTS_DIR / 'operation_breakdown.png', dpi=300, bbox_inches='tight')
    print(f"Saved: {PLOTS_DIR / 'operation_breakdown.png'}")
    plt.close()

def plot_sorting_comparison(csv_path):
    """Plot sorting algorithm comparison."""
    df = pd.read_csv(csv_path)
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 8))
    
    # Left plot: Line chart with all algorithms
    algorithms = df['Algorithm'].values
    sizes = [col for col in df.columns if 'Size' in col]
    size_values = [int(col.split()[1]) for col in sizes]
    
    colors = plt.cm.tab10(np.linspace(0, 1, len(algorithms)))
    markers = ['o', 's', '^', 'D', 'v', '<', '>', 'p']
    
    for i, algo in enumerate(algorithms):
        times = []
        valid_sizes = []
        for size, size_val in zip(sizes, size_values):
            val = df[df['Algorithm'] == algo][size].values[0]
            if pd.notna(val) and val != 'N/A':
                times.append(float(val))
                valid_sizes.append(size_val)
        
        if times:
            ax1.plot(valid_sizes, times, marker=markers[i % len(markers)], 
                    linewidth=2, markersize=8, label=algo, color=colors[i])
    
    ax1.set_xlabel('Array Size (elements)', fontsize=14, fontweight='bold')
    ax1.set_ylabel('Execution Time (ms)', fontsize=14, fontweight='bold')
    ax1.set_title('Sorting Algorithms Performance Comparison', 
                  fontsize=14, fontweight='bold')
    ax1.set_xscale('log')
    ax1.set_yscale('log')
    ax1.grid(True, alpha=0.3, which='both')
    ax1.legend(fontsize=9, loc='best', framealpha=0.9)
    
    # Right plot: Bar chart for size 10000 (mid-range comparison)
    size_col = 'Size 10000 (ms)'
    valid_algos = []
    valid_times = []
    
    for algo in algorithms:
        val = df[df['Algorithm'] == algo][size_col].values[0]
        if pd.notna(val) and val != 'N/A':
            valid_algos.append(algo)
            valid_times.append(float(val))
    
    sorted_pairs = sorted(zip(valid_algos, valid_times), key=lambda x: x[1])
    valid_algos, valid_times = zip(*sorted_pairs)
    
    bars = ax2.barh(range(len(valid_algos)), valid_times, 
                    color=plt.cm.viridis(np.linspace(0.2, 0.9, len(valid_algos))),
                    alpha=0.8)
    
    ax2.set_yticks(range(len(valid_algos)))
    ax2.set_yticklabels(valid_algos)
    ax2.set_xlabel('Execution Time (ms)', fontsize=14, fontweight='bold')
    ax2.set_title('Sorting Performance at 10,000 Elements', 
                  fontsize=14, fontweight='bold')
    ax2.set_xscale('log')
    ax2.grid(True, alpha=0.3, axis='x')
    
    # Add value labels on bars
    for i, (bar, time) in enumerate(zip(bars, valid_times)):
        ax2.text(time, i, f'  {time:.2f} ms', va='center', fontsize=9)
    
    plt.tight_layout()
    plt.savefig(PLOTS_DIR / 'sorting_comparison.png', dpi=300, bbox_inches='tight')
    print(f"Saved: {PLOTS_DIR / 'sorting_comparison.png'}")
    plt.close()

def plot_performance_efficiency(df):
    """Plot performance efficiency (ops/sec per MB of memory)."""
    fig, ax = plt.subplots(figsize=(12, 8))
    
    # Efficiency: operations per second per MB of memory
    df['efficiency'] = df['ops_per_sec'] / df['memory_mb']
    
    containers = df['container'].unique()
    colors = plt.cm.tab10(np.linspace(0, 1, len(containers)))
    markers = ['o', 's', '^', 'D']
    
    for i, container in enumerate(containers):
        container_data = df[df['container'] == container]
        ax.plot(container_data['data_size'], container_data['efficiency'],
                marker=markers[i % len(markers)], linewidth=2, markersize=10,
                label=container, color=colors[i])
    
    ax.set_xlabel('Database Size (records)', fontsize=14, fontweight='bold')
    ax.set_ylabel('Efficiency (Ops/sec per MB)', fontsize=14, fontweight='bold')
    ax.set_title('Performance Efficiency: Operations per MB of Memory', 
                 fontsize=16, fontweight='bold', pad=20)
    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.grid(True, alpha=0.3, which='both')
    ax.legend(fontsize=10, loc='best', framealpha=0.9)
    
    plt.tight_layout()
    plt.savefig(PLOTS_DIR / 'performance_efficiency.png', dpi=300, bbox_inches='tight')
    print(f"Saved: {PLOTS_DIR / 'performance_efficiency.png'}")
    plt.close()

def main():
    """Main function to generate all plots."""
    print("\nBenchmark Data Visualization")
    
    results_dir = Path('results')
    
    if not results_dir.exists():
        print(f"Error: Results directory not found: {results_dir}")
        return
    
    ops_csv = results_dir / 'operations_benchmark.csv'
    if ops_csv.exists():
        print(f"Processing operations benchmark...")
        df_ops = parse_operations_benchmark(ops_csv)
        
        print(f"  Data points: {len(df_ops)}")
        print(f"  Containers: {len(df_ops['container'].unique())}")
        print(f"  Database sizes: {', '.join(map(str, sorted(df_ops['data_size'].unique())))}")
        
        plot_operations_per_second(df_ops)
        plot_memory_usage(df_ops)
        plot_total_operations(df_ops)
        plot_operation_breakdown(df_ops)
        plot_performance_efficiency(df_ops)
    else:
        print(f"Warning: Operations benchmark file not found: {ops_csv}")
    
    sort_csv = results_dir / 'sorting_benchmark.csv'
    if sort_csv.exists():
        print(f"Processing sorting benchmark...")
        plot_sorting_comparison(sort_csv)
    else:
        print(f"Warning: Sorting benchmark file not found: {sort_csv}")
    
    print(f"\nOutput: {PLOTS_DIR.absolute()}\n")

if __name__ == '__main__':
    main()
