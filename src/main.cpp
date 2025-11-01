#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <algorithm>
#include <vector>

#include "database.hpp"
#include "student.hpp"
#include "csv_handler.hpp"
#include "sorting.hpp"
#include "benchmark.hpp"

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [mode] [arguments]\n\n";
    std::cout << "Benchmark Modes:\n";
    std::cout << "  benchmark            Complete benchmark suite (default)\n";
    std::cout << "  operations           Database operations benchmark\n";
    std::cout << "  sorting              Sorting algorithms benchmark\n\n";
    std::cout << "Operation Modes:\n";
    std::cout << "  change-group <phone> <new_group>\n";
    std::cout << "                       Change student's group by phone\n";
    std::cout << "  get-group <group>    Get students from group (sorted)\n";
    std::cout << "  get-surname <surname>\n";
    std::cout << "                       Get groups by surname\n";
    std::cout << "  sort-rating [output] [algorithm]\n";
    std::cout << "                       Sort by rating and save\n";
    std::cout << "                       Output: results/sorted_output.csv (default)\n";
    std::cout << "                       Algorithms: std, bubble, insertion, selection,\n";
    std::cout << "                                   merge, quick, heap, radix\n";
    std::cout << "                       Default algorithm: quick\n";
    std::cout << "  help                 Show this help\n";
}



void run_operations_benchmark_mode() {
    std::cout << "Database Operations Benchmark (V3: 5:10:100)\n";
    std::cout << "Containers: vector, unordered_map, map\n";
    std::cout << "Sizes: 100, 1000, 10000, 100000\n";
    std::cout << "Duration: 10s per test\n\n";
    
    std::vector<size_t> data_sizes = {100, 1000, 10000, 100000};
    auto results = benchmark::run_all_operations_benchmarks(data_sizes, 10.0);
    
    // benchmark::print_operation_results(results);
    benchmark::save_operation_results(results, "results/operations_benchmark.csv");
    
    std::cout << "\nOutput: results/operations_benchmark.csv\n";
}

void run_sorting_benchmark_mode() {
    std::cout << "Sorting Algorithms Benchmark (S2: rating)\n";
    std::cout << "Algorithms: std::sort, bubble, insertion, selection, merge, quick, heap, radix\n";
    std::cout << "Note: O(n^2) algorithms tested only on n <= 10000\n\n";
    
    std::vector<Student> full_data = csv::read_csv("data/students.csv");
    std::vector<size_t> data_sizes = {100, 1000, 10000, 100000};
    std::vector<benchmark::SortBenchmarkResult> all_results;
    
    for (size_t size : data_sizes) {
        if (size > full_data.size()) continue;
        
        std::cout << "\n=== Testing with data size: " << size << " ===\n";
        std::vector<Student> subset(full_data.begin(), full_data.begin() + size);
        auto results = benchmark::run_sorting_benchmarks(subset, student_comparators::compare_by_rating);
        all_results.insert(all_results.end(), results.begin(), results.end());
    }
    
    benchmark::print_sort_results(all_results);
    benchmark::save_sort_results(all_results, "results/sorting_benchmark.csv");
    
    std::cout << "\nOutput: results/sorting_benchmark.csv\n";
}

void run_benchmark_mode() {
    run_operations_benchmark_mode();

    std::cout << "\n";

    run_sorting_benchmark_mode();
}

IStudentDatabase* load_database(const std::string& filename) {
    DatabaseVector* db = new DatabaseVector();

    if (!db->load_from_file(filename)) {
        std::cerr << "Error: Failed to load " << filename << "\n";
        delete db;
        return nullptr;
    }

    return db;
}

void run_change_group(IStudentDatabase* db, const std::string& phone, const std::string& new_group) {
    if (db->change_group_by_phone(phone, new_group)) {
        std::cout << "Changed group to '" << new_group << "' for phone: " << phone << "\n";
    } else {
        std::cout << "Phone not found: " << phone << "\n";
    }
}

void run_get_group(IStudentDatabase* db, const std::string& group) {
    auto students = db->get_students_by_group_sorted(group);

    std::cout << "Group '" << group << "': " << students.size() << " students\n";

    for (const auto& s : students) {
        std::cout << "  " << s.m_surname << " " << s.m_name << " (rating: " << s.m_rating << ")\n";
    }
}

void run_get_surname(IStudentDatabase* db, const std::string& surname) {
    auto groups = db->get_groups_by_surname(surname);

    std::cout << "Surname '" << surname << "' found in " << groups.size() << " groups:\n";

    for (const auto& g : groups) {
        std::cout << "  " << g << "\n";
    }
}

void run_sort_rating(IStudentDatabase* db, const std::string& output, const std::string& algorithm) {
    using SortFunc = std::function<void(std::vector<Student>&, std::function<bool(const Student&, const Student&)>)>;
    
    static const std::unordered_map<std::string, SortFunc> algorithms = {
        {"std", sort_algorithms::std_sort},
        {"bubble", sort_algorithms::bubble_sort},
        {"insertion", sort_algorithms::insertion_sort},
        {"selection", sort_algorithms::selection_sort},
        {"merge", sort_algorithms::merge_sort},
        {"quick", sort_algorithms::quick_sort},
        {"heap", sort_algorithms::heap_sort},
        {"radix", sort_algorithms::radix_sort_by_rating}
    };
    
    auto it = algorithms.find(algorithm);
    if (it == algorithms.end()) {
        std::cerr << "Error: Unknown algorithm '" << algorithm << "'\n";
        return;
    }
    
    if (db->sort_by_rating_and_save(output, it->second, true)) {
        std::cout << "Sorted " << db->size() << " students by rating using " << algorithm << "\n";
        std::cout << "Output: " << output << "\n";
    } else {
        std::cerr << "Error: Failed to save to " << output << "\n";
    }
}

int main(int argc, char* argv[]) {
    std::string mode = "benchmark";
    
    if (argc > 1) {
        mode = argv[1];
        if (mode == "help" || mode == "-h" || mode == "--help") {
            print_usage(argv[0]);
            return 0;
        }
    }
    
    if (mode == "benchmark") {
        run_benchmark_mode();
        return 0;
    } else if (mode == "operations") {
        run_operations_benchmark_mode();
        return 0;
    } else if (mode == "sorting") {
        run_sorting_benchmark_mode();
        return 0;
    }
    
    IStudentDatabase* db = load_database("data/students.csv");
    if (!db) {
        return -1;
    }
    
    if (mode == "change-group") {
        if (argc < 4) {
            std::cerr << "Error: change-group requires <phone> <new_group>\n";
            delete db;
            return 1;
        }
        run_change_group(db, argv[2], argv[3]);
        
    } else if (mode == "get-group") {
        if (argc < 3) {
            std::cerr << "Error: get-group requires <group>\n";
            delete db;
            return 1;
        }
        run_get_group(db, argv[2]);
        
    } else if (mode == "get-surname") {
        if (argc < 3) {
            std::cerr << "Error: get-surname requires <surname>\n";
            delete db;
            return 1;
        }
        run_get_surname(db, argv[2]);
        
    } else if (mode == "sort-rating") {
        std::string output = argc >= 3 ? argv[2] : "results/sorted_output.csv";
        std::string algorithm = argc >= 4 ? argv[3] : "quick";
        
        if (argc == 3) {
            static const std::vector<std::string> known_algorithms = {
                "std", "bubble", "insertion", "selection", "merge", "quick", "heap", "radix"
            };
            if (std::find(known_algorithms.begin(), known_algorithms.end(), argv[2]) != known_algorithms.end()) {
                algorithm = argv[2];
                output = "results/sorted_output.csv";
            }
        }
        
        run_sort_rating(db, output, algorithm);
        
    } else {
        std::cerr << "Error: Invalid mode '" << mode << "'\n\n";
        print_usage(argv[0]);
        delete db;
        return 1;
    }
    
    delete db;
    return 0;
}