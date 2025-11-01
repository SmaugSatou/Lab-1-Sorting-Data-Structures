#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <set>

#include "benchmark.hpp"
#include "sorting.hpp"
#include "student.hpp"
#include "csv_handler.hpp"

namespace benchmark {
    
    // Measure sorting algorithm performance
    template <typename T>
    SortBenchmarkResult measure_sort(
        const std::string& algorithm_name,
        const std::vector<T>& data,
        std::function<void(std::vector<T>&, std::function<bool(const T&, const T&)>)> sort_func,
        std::function<bool(const T&, const T&)> comparator) {
        
        SortBenchmarkResult result;
        result.algorithm_name = algorithm_name;
        result.data_size = data.size();
        
        std::vector<T> data_copy = data;
        
        // Measure time
        auto start = std::chrono::high_resolution_clock::now();
        sort_func(data_copy, comparator);
        auto end = std::chrono::high_resolution_clock::now();
        
        // Calculate elapsed time in milliseconds
        std::chrono::duration<double, std::milli> elapsed = end - start;
        result.execution_time_ms = elapsed.count();
        
        return result;
    }
    
    // Operations benchmark with ratio support
    OperationBenchmarkResult run_operations_benchmark(
        IStudentDatabase* db,
        double duration_seconds,
        int op1_ratio,
        int op2_ratio,
        int op3_ratio) {
        
        OperationBenchmarkResult result;
        result.container_name = db->get_container_name();
        result.data_size = db->size();
        result.op1_count = 0;
        result.op2_count = 0;
        result.op3_count = 0;
        
        // Setup random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Create weighted distribution
        std::vector<int> weights = {op1_ratio, op2_ratio, op3_ratio};
        std::discrete_distribution<> dist(weights.begin(), weights.end());
        
        // Get sample data for operations
        std::vector<Student> all_students = db->to_vector();
        if (all_students.empty()) {
            result.duration_seconds = 0;
            result.total_operations = 0;
            result.operations_per_second = 0;
            result.memory_usage_bytes = 0;
            result.memory_usage_mb = 0;
            return result;
        }
        
        // Prepare random phones, groups, and surnames
        std::vector<std::string> phones, groups, surnames;
        std::set<std::string> unique_groups, unique_surnames;
        
        for (const auto& s : all_students) {
            phones.push_back(s.m_phone_number);
            unique_groups.insert(s.m_group);
            unique_surnames.insert(s.m_surname);
        }
    
        groups.assign(unique_groups.begin(), unique_groups.end());
        surnames.assign(unique_surnames.begin(), unique_surnames.end());
        
        std::uniform_int_distribution<> phone_dist(0, phones.size() - 1);
        std::uniform_int_distribution<> group_dist(0, groups.size() - 1);
        std::uniform_int_distribution<> surname_dist(0, surnames.size() - 1);
        std::uniform_int_distribution<> new_group_dist(0, groups.size() - 1);
        
        // Run operations for specified duration
        auto start_time = std::chrono::high_resolution_clock::now();
        auto end_time = start_time + std::chrono::duration<double>(duration_seconds);
        
        while (std::chrono::high_resolution_clock::now() < end_time) {
            int operation = dist(gen);
            
            switch (operation) {
                case 0: // Operation 1: Change group by phone
                    {
                        std::string phone = phones[phone_dist(gen)];
                        std::string new_group = groups[new_group_dist(gen)];
                        db->change_group_by_phone(phone, new_group);
                        result.op1_count++;
                    }
                    break;
                    
                case 1: // Operation 2: Get students by group sorted
                    {
                        std::string group = groups[group_dist(gen)];
                        auto students = db->get_students_by_group_sorted(group);
                        result.op2_count++;
                    }
                    break;
                    
                case 2: // Operation 3: Get groups by surname
                    {
                        std::string surname = surnames[surname_dist(gen)];
                        auto result_groups = db->get_groups_by_surname(surname);
                        result.op3_count++;
                    }
                    break;
            }
        }
        
        auto actual_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> actual_duration = actual_end - start_time;
        
        result.duration_seconds = actual_duration.count();
        result.total_operations = result.op1_count + result.op2_count + result.op3_count;
        result.operations_per_second = result.total_operations / result.duration_seconds;
        result.memory_usage_bytes = db->estimate_memory_usage();
        result.memory_usage_mb = result.memory_usage_bytes / (1024.0 * 1024.0);
        
        return result;
    }
    
    // Run benchmarks on all database implementations
    std::vector<OperationBenchmarkResult> run_all_operations_benchmarks(
        const std::vector<size_t>& data_sizes,
        double duration_seconds) {
        
        std::vector<OperationBenchmarkResult> all_results;
        
        // Load full dataset
        std::vector<Student> full_data = csv::read_csv("data/students.csv");
        
        for (size_t data_size : data_sizes) {
            std::cout << "\n=== Testing with data size: " << data_size << " ===\n" << std::endl;
            
            // Prepare subset of data
            std::vector<Student> subset(full_data.begin(), 
                                       full_data.begin() + std::min(data_size, full_data.size()));
            
            // Variant V3 ratios: 5:10:100
            const int OP1_RATIO = 5;   // Change group by phone
            const int OP2_RATIO = 10;  // Get students by group sorted
            const int OP3_RATIO = 100; // Get groups by surname
            
            // Test DatabaseVector
            std::cout << "Testing DatabaseVector (std::vector)..." << std::endl;
            DatabaseVector db_vector(subset);
            auto result_vector = run_operations_benchmark(&db_vector, duration_seconds, OP1_RATIO, OP2_RATIO, OP3_RATIO);
            all_results.push_back(result_vector);
            
            // Test DatabaseHashMap
            std::cout << "Testing DatabaseHashMap (std::unordered_map)..." << std::endl;
            DatabaseHashMap db_hashmap(subset);
            auto result_hashmap = run_operations_benchmark(&db_hashmap, duration_seconds, OP1_RATIO, OP2_RATIO, OP3_RATIO);
            all_results.push_back(result_hashmap);
            
            // Test DatabaseTreeMap
            std::cout << "Testing DatabaseTreeMap (std::map)..." << std::endl;
            DatabaseTreeMap db_treemap(subset);
            auto result_treemap = run_operations_benchmark(&db_treemap, duration_seconds, OP1_RATIO, OP2_RATIO, OP3_RATIO);
            all_results.push_back(result_treemap);
            
            // Test DatabaseHybrid
            std::cout << "Testing DatabaseHybrid (unordered_map + multimap indices)..." << std::endl;
            DatabaseHybrid db_hybrid(subset);
            auto result_hybrid = run_operations_benchmark(&db_hybrid, duration_seconds, OP1_RATIO, OP2_RATIO, OP3_RATIO);
            all_results.push_back(result_hybrid);
        }
        
        return all_results;
    }
    
    // Sorting benchmarks comparing all 7 sorting algorithms
    std::vector<SortBenchmarkResult> run_sorting_benchmarks(
        const std::vector<Student>& data,
        std::function<bool(const Student&, const Student&)> comparator) {
        
        std::vector<SortBenchmarkResult> results;
        
        // Test std::sort 
        std::cout << "Testing std::sort..." << std::endl;
        results.push_back(measure_sort<Student>("std::sort", data, 
                                      sort_algorithms::std_sort, comparator));
        
        // Test Bubble Sort
        if (data.size() <= 1000) {
            std::cout << "Testing Bubble Sort..." << std::endl;
            results.push_back(measure_sort<Student>("Bubble Sort", data,
                                          sort_algorithms::bubble_sort, comparator));
        }
        
        // Test Insertion Sort
        if (data.size() <= 1000) {
            std::cout << "Testing Insertion Sort..." << std::endl;
            results.push_back(measure_sort<Student>("Insertion Sort", data,
                                          sort_algorithms::insertion_sort, comparator));
        }
        
        // Test Selection Sort
        if (data.size() <= 1000) {
            std::cout << "Testing Selection Sort..." << std::endl;
            results.push_back(measure_sort<Student>("Selection Sort", data,
                                          sort_algorithms::selection_sort, comparator));
        }
        
        // Test Merge Sort
        std::cout << "Testing Merge Sort..." << std::endl;
        results.push_back(measure_sort<Student>("Merge Sort", data,
                                      sort_algorithms::merge_sort, comparator));
        
        // Test Quick Sort
        std::cout << "Testing Quick Sort..." << std::endl;
        results.push_back(measure_sort<Student>("Quick Sort", data,
                                      sort_algorithms::quick_sort, comparator));
        
        // Test Heap Sort
        std::cout << "Testing Heap Sort..." << std::endl;
        results.push_back(measure_sort<Student>("Heap Sort", data,
                                      sort_algorithms::heap_sort, comparator));
        
        // Test Radix Sort
        std::cout << "Testing Radix Sort..." << std::endl;
        results.push_back(measure_sort<Student>("Radix Sort", data,
                                      sort_algorithms::radix_sort_by_rating, comparator));
        
        return results;
    }
    
    // Save operation results to CSV
    bool save_operation_results(const std::vector<OperationBenchmarkResult>& results, 
                                const std::string& filename) {
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Could not create file " << filename << std::endl;
            return false;
        }
        
        // Write header
        file << "Container,Data Size,Op1 Count,Op2 Count,Op3 Count,Total Ops,"
             << "Duration (s),Ops/sec,Memory (bytes),Memory (MB)\n";
        
        // Write results
        for (const auto& result : results) {
            file << result.container_name << ","
                 << result.data_size << ","
                 << result.op1_count << ","
                 << result.op2_count << ","
                 << result.op3_count << ","
                 << result.total_operations << ","
                 << std::fixed << std::setprecision(2) << result.duration_seconds << ","
                 << std::fixed << std::setprecision(2) << result.operations_per_second << ","
                 << result.memory_usage_bytes << ","
                 << std::fixed << std::setprecision(2) << result.memory_usage_mb << "\n";
        }
        
        file.close();
        std::cout << "Operation benchmark results saved to " << filename << std::endl;
        return true;
    }
    
    // Save sorting results to CSV
    bool save_sort_results(const std::vector<SortBenchmarkResult>& results, 
                          const std::string& filename) {
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Could not create file " << filename << std::endl;
            return false;
        }
        
        file << "Algorithm,Data Size,Execution Time (ms)\n";
        
        for (const auto& result : results) {
            file << result.algorithm_name << ","
                 << result.data_size << ","
                 << std::fixed << std::setprecision(6) << result.execution_time_ms << "\n";
        }
        
        file.close();
        std::cout << "Sort benchmark results saved to " << filename << std::endl;
        return true;
    }
    
    // Print operation results to console
    void print_operation_results(const std::vector<OperationBenchmarkResult>& results) {
        std::cout << "\n" << std::string(120, '=') << std::endl;
        std::cout << "OPERATION BENCHMARK RESULTS" << std::endl;
        std::cout << std::string(120, '=') << std::endl;
        
        std::cout << std::left 
                  << std::setw(15) << "Container"
                  << std::setw(12) << "Data Size"
                  << std::setw(10) << "Op1"
                  << std::setw(10) << "Op2"
                  << std::setw(10) << "Op3"
                  << std::setw(12) << "Total Ops"
                  << std::setw(12) << "Ops/sec"
                  << std::setw(15) << "Memory (MB)" << std::endl;
        std::cout << std::string(120, '-') << std::endl;
        
        for (const auto& result : results) {
            std::cout << std::left 
                      << std::setw(15) << result.container_name
                      << std::setw(12) << result.data_size
                      << std::setw(10) << result.op1_count
                      << std::setw(10) << result.op2_count
                      << std::setw(10) << result.op3_count
                      << std::setw(12) << result.total_operations
                      << std::fixed << std::setprecision(2)
                      << std::setw(12) << result.operations_per_second
                      << std::setw(15) << result.memory_usage_mb << std::endl;
        }
        
        std::cout << std::string(120, '=') << std::endl << std::endl;
    }
    
    // Print sorting results to console
    void print_sort_results(const std::vector<SortBenchmarkResult>& results) {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "SORTING BENCHMARK RESULTS" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        std::cout << std::left << std::setw(20) << "Algorithm"
                  << std::setw(15) << "Data Size"
                  << std::setw(20) << "Time (ms)" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (const auto& result : results) {
            std::cout << std::left << std::setw(20) << result.algorithm_name
                      << std::setw(15) << result.data_size
                      << std::fixed << std::setprecision(3) 
                      << std::setw(20) << result.execution_time_ms << std::endl;
        }
        
        std::cout << std::string(60, '=') << std::endl << std::endl;
    }
    
}