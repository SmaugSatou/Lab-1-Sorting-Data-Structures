#pragma once

#include <string>
#include <vector>
#include <functional>
#include <chrono>

namespace benchmark {
    /**
     * @brief Structure to hold benchmark results
     */
    struct BenchmarkResult {
        std::string algorithm_name;
        size_t data_size;
        double execution_time_ms;
        size_t comparisons;
        size_t swaps;
    };
    
    /**
     * @brief Measure execution time of a sorting algorithm
     * @tparam T Type of elements to sort
     * @param algorithm_name Name of the algorithm
     * @param data Data to sort (will be copied)
     * @param sort_func Sorting function
     * @param comparator Comparison function
     * @return BenchmarkResult containing timing and statistics
     */
    template <typename T>
    BenchmarkResult measure_sort(
        const std::string& algorithm_name,
        const std::vector<T>& data,
        std::function<void(std::vector<T>&, std::function<bool(const T&, const T&)>)> sort_func,
        std::function<bool(const T&, const T&)> comparator
    );
    
    /**
     * @brief Run benchmarks on multiple algorithms
     * @tparam T Type of elements to sort
     * @param data Data to sort
     * @param comparator Comparison function
     * @return Vector of benchmark results
     */
    template <typename T>
    std::vector<BenchmarkResult> run_all_benchmarks(
        const std::vector<T>& data,
        std::function<bool(const T&, const T&)> comparator
    );
    
    /**
     * @brief Save benchmark results to CSV file
     * @param results Vector of benchmark results
     * @param filename Path to output file
     * @return true if successful, false otherwise
     */
    bool save_results(const std::vector<BenchmarkResult>& results, const std::string& filename);
    
    /**
     * @brief Print benchmark results to console
     * @param results Vector of benchmark results
     */
    void print_results(const std::vector<BenchmarkResult>& results);
    
    /**
     * @brief Compare two algorithms
     * @param result1 First benchmark result
     * @param result2 Second benchmark result
     * @return Comparison string
     */
    std::string compare_algorithms(const BenchmarkResult& result1, const BenchmarkResult& result2);
}