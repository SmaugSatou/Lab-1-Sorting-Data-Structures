#pragma once

#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <memory>

#include "database.hpp"

namespace benchmark {

    /**
     * @brief Structure to hold operation benchmark results
     */
    struct OperationBenchmarkResult {
        std::string container_name;
        size_t data_size;
        size_t op1_count;  // Change group by phone
        size_t op2_count;  // Get students by group
        size_t op3_count;  // Get groups by surname
        size_t total_operations;
        double duration_seconds;
        double operations_per_second;
        size_t memory_usage_bytes;
        double memory_usage_mb;
    };
    
    /**
     * @brief Structure to hold sorting benchmark results
     */
    struct SortBenchmarkResult {
        std::string algorithm_name;
        size_t data_size;
        double execution_time_ms;
    };
    
    /**
     * @brief Run operations benchmark for specified duration with ratio
     * 
     * Operations are selected randomly with ratio A:B:C (5:10:100 for V3)
     * Operation 1 (A=5): Change student's group by phone
     * Operation 2 (B=10): Get students by group sorted
     * Operation 3 (C=100): Get groups by surname
     * 
     * @param db Database instance to benchmark
     * @param duration_seconds Duration to run benchmark (default 10)
     * @param op1_ratio Ratio for operation 1 (default 5)
     * @param op2_ratio Ratio for operation 2 (default 10)
     * @param op3_ratio Ratio for operation 3 (default 100)
     * @return OperationBenchmarkResult with statistics
     */
    OperationBenchmarkResult run_operations_benchmark(
        IStudentDatabase* db,
        double duration_seconds = 10.0,
        int op1_ratio = 5,
        int op2_ratio = 10,
        int op3_ratio = 100
    );
    
    /**
     * @brief Run operations benchmarks on all three database implementations
     * @param data_sizes Vector of data sizes to test (100, 1000, 10000, 100000)
     * @param duration_seconds Duration for each benchmark
     * @return Vector of all benchmark results
     */
    std::vector<OperationBenchmarkResult> run_all_operations_benchmarks(
        const std::vector<size_t>& data_sizes,
        double duration_seconds = 10.0
    );
    
    /**
     * @brief Measure execution time of a sorting algorithm
     * @tparam T Type of elements to sort
     * @param algorithm_name Name of the algorithm
     * @param data Data to sort (will be copied)
     * @param sort_func Sorting function
     * @param comparator Comparison function
     * @return SortBenchmarkResult containing timing and statistics
     */
    template <typename T>
    SortBenchmarkResult measure_sort(
        const std::string& algorithm_name,
        const std::vector<T>& data,
        std::function<void(std::vector<T>&, std::function<bool(const T&, const T&)>)> sort_func,
        std::function<bool(const T&, const T&)> comparator
    );
    
    /**
     * @brief Run sorting benchmarks
     * @param data Data to sort
     * @param comparator Comparison function
     * @return Vector of benchmark results
     */
    std::vector<SortBenchmarkResult> run_sorting_benchmarks(
        const std::vector<Student>& data,
        std::function<bool(const Student&, const Student&)> comparator
    );
    
    /**
     * @brief Save operation benchmark results to CSV file
     * @param results Vector of operation benchmark results
     * @param filename Path to output file
     * @return true if successful, false otherwise
     */
    bool save_operation_results(const std::vector<OperationBenchmarkResult>& results, 
                                const std::string& filename);
    
    /**
     * @brief Save sorting benchmark results to CSV file
     * @param results Vector of sorting benchmark results
     * @param filename Path to output file
     * @return true if successful, false otherwise
     */
    bool save_sort_results(const std::vector<SortBenchmarkResult>& results, 
                          const std::string& filename);
    
    /**
     * @brief Print operation benchmark results to console
     * @param results Vector of operation benchmark results
     */
    void print_operation_results(const std::vector<OperationBenchmarkResult>& results);
    
    /**
     * @brief Print sorting benchmark results to console
     * @param results Vector of sorting benchmark results
     */
    void print_sort_results(const std::vector<SortBenchmarkResult>& results);
}