# Lab-1-Sorting-Data-Structures

## **Variant:** V3 - 5 - 10 - 100 - S2

### Task Description

**V3** - Three operations with ratio **5:10:100**:
1. **Operation 1 (5%)**: Change student's group by phone number
2. **Operation 2 (10%)**: Get students from specific group (sorted by surname and name)
3. **Operation 3 (100%)**: Get list of groups where students with specific surname study

**S2** - Sorting by: **Rating** (m_rating field)

### Prerequisites

- **C++ Compiler** with C++17 support (GCC, Clang, or MSVC)
- **CMake** 3.10 or higher
- **Python 3.x** (optional, for visualization)
  - pandas
  - matplotlib
  - numpy

For Python dependencies:
```bash
pip install -r scripts/requirements.txt
```

### Compilation

#### Using CMake

```bash
# Create build directory and configure
mkdir -p build

# Build the project
cmake --build build
```

The executable will be created as `student_db` (or `student_db.exe` on Windows) in the `build` directory.

### Usage

#### Quick Start

**Run the complete benchmark**:

```bash
# Linux/macOS:
./build/student_db

# Windows (PowerShell):
.\build\Debug\student_db.exe
# or
.\build\Release\student_db.exe
```

This runs the full funcionality by default.

**Show help and available commands**:

```bash
# Linux/macOS:
./build/student_db help

# Windows (PowerShell):
.\build\Debug\student_db.exe help
```

#### Benchmark Modes

**Complete Benchmark Suite**:

```bash
./build/student_db benchmark
# or simply:
./build/student_db
```

Runs both operations and sorting benchmarks. Results saved to:
- `results/operations_benchmark.csv`
- `results/sorting_benchmark.csv`

**Database Operations Benchmark Only**:

```bash
./build/student_db operations
```

- Tests: vector, unordered_map (hashmap), map (treemap), hybrid
- Data sizes: 100, 1,000, 10,000, 100,000 records
- Duration: 10 seconds per container/size combination
- Operations ratio: 5:10:100 (V3)
- Output: `results/operations_benchmark.csv`

**Sorting Algorithms Benchmark Only**:

```bash
./build/student_db sorting
```

- Algorithms: std::sort, bubble, insertion, selection, merge, quick, heap, counting
- Data sizes: 100, 1,000, 10,000, 100,000 records
- Note: O(n²) algorithms (bubble, insertion, selection) tested only on n ≤ 10,000
- Output: `results/sorting_benchmark.csv`

#### Database Operations

**Change Student Group**:

```bash
./build/student_db change-group "38(050)76-70-965" "ABC-12"
```

Changes the group of the student with the specified phone number.

**Get Students by Group**:

```bash
./build/student_db get-group "NXE-49"
```

Returns all students in the specified group, sorted by surname and name.

**Get Groups by Surname**:

```bash
./build/student_db get-surname "Кравченко"
```

Returns all groups containing students with the specified surname.

**Sort by Rating and Save**:

```bash
./build/student_db sort-rating [output_file] [algorithm]
```

Examples:

```bash
# Using quick sort (default) to results/sorted_output.csv
./build/student_db sort-rating

# Using merge sort with custom output file
./build/student_db sort-rating results/custom.csv merge

# Using counting sort
./build/student_db sort-rating results/sorted.csv counting
```

Available algorithms: `std`, `bubble`, `insertion`, `selection`, `merge`, `quick`, `heap`, `counting`

### Database Approaches

The project implements four different data structure approaches for the student database:

#### 1. **Vector-based (DatabaseVector)**
- **Container**: `std::vector<Student>`
- **Complexity**:
  - Insert: O(1) amortized
  - Search by phone: O(n)
  - Search by group: O(n)
  - Search by surname: O(n)
- **Memory**: Most compact, no overhead
- **Best for**: Small datasets, sequential access

#### 2. **HashMap-based (DatabaseHashmap)**
- **Container**: `std::unordered_map<std::string, Student>` (phone as key)
- **Complexity**:
  - Insert: O(1) average
  - Search by phone: O(1) average
  - Search by group: O(n)
  - Search by surname: O(n)
- **Memory**: Hash table overhead (~2x data size)
- **Best for**: Frequent phone lookups, Operation 1

#### 3. **TreeMap-based (DatabaseTreemap)**
- **Container**: `std::map<std::string, Student>` (phone as key)
- **Complexity**:
  - Insert: O(log n)
  - Search by phone: O(log n)
  - Search by group: O(n)
  - Search by surname: O(n)
- **Memory**: Tree structure overhead
- **Best for**: Sorted access, range queries

#### 4. **Hybrid (DatabaseHybrid)**
- **Containers**:
  - `std::unordered_map<std::string, Student>` (phone → student)
  - `std::unordered_map<std::string, std::vector<Student*>>` (group → students)
  - `std::unordered_map<std::string, std::set<std::string>>` (surname → groups)
- **Complexity**:
  - Insert: O(1) average
  - Search by phone: O(1) average
  - Search by group: O(1) average + O(k log k) for sorting k results
  - Search by surname: O(1) average
- **Memory**: Highest overhead (~3-4x data size)
- **Best for**: Balanced performance across all operations

### Sorting Algorithms

All sorting algorithms are implemented from scratch and benchmarked:

#### O(n²) Algorithms (for n ≤ 10,000)
1. **Bubble Sort**: Simple comparison-based, stable
2. **Insertion Sort**: Efficient for small/nearly sorted data, stable
3. **Selection Sort**: Minimal swaps, unstable

#### O(n log n) Algorithms
4. **Merge Sort**: Divide-and-conquer, stable, O(n) extra space
5. **Quick Sort**: Divide-and-conquer, unstable, in-place
6. **Heap Sort**: Using binary heap, unstable, in-place
7. **std::sort**: Standard library (typically introsort)

#### O(n + k) Algorithm
8. **Counting Sort**: Direct addressing for discretized ratings, stable, O(k) extra space where k is the range

All algorithms use function pointers for flexible comparison, supporting both ascending and descending order.

### Python Visualization Script

After running benchmarks, visualize results using Python:

```bash
cd scripts
python plot_results.py
```

This generates plots in `results/plots/`:
- **Operations performance**: Operations per second by container and data size
- **Memory usage**: Memory consumption comparison
- **Sorting performance**: Time comparison across algorithms and data sizes

### Performance Analysis & Conclusions

#### Database Operations Performance

##### **1. Operations per Second (Throughput)**

**Small Dataset (100 records)**:

- **Hybrid**: 446,596 ops/sec (Best - 69% faster than runner-up)
- **std::vector**: 264,480 ops/sec
- **std::unordered_map**: 247,120 ops/sec
- **std::map**: 234,995 ops/sec

**Medium Dataset (1,000 records)**:

- **Hybrid**: 69,065 ops/sec (Best - 147% faster than runner-up)
- **std::vector**: 27,995 ops/sec
- **std::unordered_map**: 25,733 ops/sec
- **std::map**: 18,407 ops/sec

**Large Dataset (10,000 records)**:

- **Hybrid**: 3,617 ops/sec (Best - 42% faster than runner-up)
- **std::vector**: 2,552 ops/sec
- **std::unordered_map**: 1,728 ops/sec
- **std::map**: 1,248 ops/sec

**Very Large Dataset (100,000 records)**:

- **Hybrid**: 417 ops/sec (Best - 109% faster than runner-up)
- **std::vector**: 200 ops/sec
- **std::unordered_map**: 89 ops/sec
- **std::map**: 65 ops/sec

**Key Insights**:

- **Hybrid consistently dominates** across all data sizes, demonstrating that maintaining multiple indices is worth the overhead for this workload
- Performance degradation as data grows: Vector maintains ~60% relative performance, while hash/tree maps drop to ~35-40% (more sensitive to scale)
- **Vector outperforms hash structures** at 10K+ records despite O(n) searches, likely due to cache locality and the 5:10:100 operation ratio (85% searches favor sequential iteration)

##### **2. Memory Consumption**

| Container | 100 records | 1K records | 10K records | 100K records |
|-----------|-------------|------------|-------------|--------------|
| **std::vector** | 0.03 MB | 0.26 MB | 2.63 MB | 26.39 MB |
| **std::unordered_map** | 0.04 MB | 0.36 MB | 3.57 MB | 36.22 MB |
| **std::map** | 0.04 MB | 0.36 MB | 3.57 MB | 35.76 MB |
| **Hybrid** | 0.06 MB | 0.59 MB | 5.87 MB | 59.24 MB |

**Key Insights**:

- **Vector is most memory-efficient** (baseline)
- Hash/tree maps: ~36% more memory (pointer overhead, buckets/nodes)
- **Hybrid uses 2.2x vector memory** but delivers 2-7x performance improvement
- Memory/performance trade-off: Hybrid achieves best efficiency ratio at large scales

##### **3. Performance per Operation Type**

For 100,000 records (10-second test):

- **Operation 1 (Update group by phone)**: Hybrid 169, Vector 94, Hash 37, Tree 33
- **Operation 2 (Get students by group)**: Hybrid 360, Vector 160, Hash 69, Tree 54
- **Operation 3 (Get groups by surname)**: Hybrid 3,644, Vector 1,742, Hash 786, Tree 567

**Key Insights**:

- Hybrid's multi-index approach provides consistent 2-5x advantage across all operation types
- Operation 3 (100% ratio) dominates the workload - optimizing it has maximum impact
- Vector's cache-friendly linear search surprisingly competitive for this specific workload distribution

#### Sorting Algorithms Performance

Based on `results/sorting_benchmark.csv`:

##### **Performance Comparison (10,000 elements)**

| Algorithm | Time (ms) | Relative to std::sort |
|-----------|-----------|----------------------|
| **Counting Sort** | 7.37 | 3.5x faster |
| **Quick Sort** | 40.68 | 1.6x faster |
| **Heap Sort** | 70.54 | 2.7x slower |
| **Merge Sort** | 133.28 | 5.2x slower |
| **Selection Sort** | 2,646.79 | 103x slower |
| **Insertion Sort** | 3,798.17 | 147x slower |
| **Bubble Sort** | 7,115.55 | 276x slower |
| **std::sort** | 25.79 | baseline |

##### **Scalability Analysis**

**100,000 elements** (O(n²) algorithms timeout):

- **Counting Sort**: 85.96 ms (Best - linear growth)
- **Quick Sort**: 341.04 ms (2.96x std::sort)
- **std::sort**: 254.33 ms
- **Heap Sort**: 535.61 ms
- **Merge Sort**: 1,265.19 ms

**Key Insights**:

1. **Counting Sort is the clear winner** for this use case (floating-point ratings with limited range):
   - 3.5x faster than std::sort at 10K elements
   - 3x faster than std::sort at 100K elements
   - Linear O(n + k) complexity vs. O(n log n) shows at scale

2. **Quick Sort performs well** but loses to std::sort at very large scales:
   - Likely due to std::sort's introsort hybrid approach (quick + heap + insertion)
   - Average-case O(n log n) with good constants

3. **O(n²) algorithms completely impractical**:
   - Bubble Sort: 276x slower than std::sort
   - Insertion Sort: 147x slower
   - Selection Sort: 103x slower
   - All timeout at 100K elements

4. **Merge Sort's stability penalty**:
   - Consistent O(n log n) but 5x slower due to O(n) extra memory allocations
   - Predictable but not the fastest

##### **Why Counting Sort Dominates**

For sorting **n elements** with values in a limited range **[0, k)**:

**Counting Sort Time Complexity:**

$$T_{counting}(n) = O(n + k)$$

Where:
- $n$ = number of elements to sort
- $k$ = range of input values (0 to k-1)

For our implementation (sorting ratings 0.0 to 100.0 with 2 decimal places):
- $k = 10001$ possible discrete values (0.00, 0.01, ..., 100.00)
- We convert floats to integers: $rating_{int} = \lfloor rating \times 100 \rfloor$

$$T_{counting}(n) = O(n + 10001) \approx O(n)$$

Since $k = 10001$ is constant and much smaller than large $n$:

$$T_{counting}(n) \approx n \text{ operations (linear time)}$$

**Algorithm Steps:**
1. Count occurrences of each rating value: $O(n)$
2. Calculate cumulative counts: $O(k) = O(10001) = O(1)$
3. Place elements in sorted order: $O(n)$

Total: $O(n) + O(1) + O(n) = O(n)$ for large datasets
### Project Structure

```
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── data/
│   └── students.csv        # 100,000 student records
├── include/
│   ├── database/           # Database interface and implementations
│   ├── models/             # Student model and comparators
│   ├── sorting/            # Sorting algorithms
│   ├── benchmark/          # Benchmarking framework
│   └── utils/              # CSV handler utilities
├── src/                    # Implementation files
├── results/                # Benchmark results and plots
└── scripts/                # Python visualization scripts
```
