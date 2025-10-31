#pragma once

#include <vector>
#include <functional>

struct Student;

namespace sort_algorithms {
    /**
     * @brief Bubble sort implementation
     * @param data Vector of Students to sort
     * @param comparator Comparison function
     */
    void bubble_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator);
    
    /**
     * @brief Insertion sort implementation
     * @param data Vector of Students to sort
     * @param comparator Comparison function
     */
    void insertion_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator);
    
    /**
     * @brief Selection sort implementation
     * @param data Vector of Students to sort
     * @param comparator Comparison function
     */
    void selection_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator);
    
    /**
     * @brief Merge sort implementation
     * @param data Vector of Students to sort
     * @param comparator Comparison function
     */
    void merge_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator);
    
    /**
     * @brief Quick sort implementation
     * @param data Vector of Students to sort
     * @param comparator Comparison function
     */
    void quick_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator);
    
    /**
     * @brief Heap sort implementation
     * @param data Vector of Students to sort
     * @param comparator Comparison function
     */
    void heap_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator);
    
    /**
     * @brief Standard library sort (std::sort)
     * @param data Vector of Students to sort
     * @param comparator Comparison function
     */
    void std_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator);
    
    /**
     * @brief Radix Sort implementation for floating point values (rating)
     * This is a custom implementation optimized for Student rating sorting
     * Uses counting sort on the integer and fractional parts separately
     * @param data Vector of Students to sort
     * @param comparator Comparison function (ascending/descending)
     */
    void radix_sort_by_rating(std::vector<Student>& data, 
                             std::function<bool(const Student&, const Student&)> comparator);
}