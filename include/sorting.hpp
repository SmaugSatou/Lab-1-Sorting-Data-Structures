#pragma once

#include <vector>
#include <functional>

namespace sorting {
    /**
     * @brief Bubble sort implementation
     * @tparam T Type of elements to sort
     * @param data Vector of elements to sort
     * @param comparator Comparison function
     */
    template <typename T>
    void bubble_sort(std::vector<T>& data, std::function<bool(const T&, const T&)> comparator);
    
    /**
     * @brief Insertion sort implementation
     * @tparam T Type of elements to sort
     * @param data Vector of elements to sort
     * @param comparator Comparison function
     */
    template <typename T>
    void insertion_sort(std::vector<T>& data, std::function<bool(const T&, const T&)> comparator);
    
    /**
     * @brief Selection sort implementation
     * @tparam T Type of elements to sort
     * @param data Vector of elements to sort
     * @param comparator Comparison function
     */
    template <typename T>
    void selection_sort(std::vector<T>& data, std::function<bool(const T&, const T&)> comparator);
    
    /**
     * @brief Merge sort implementation
     * @tparam T Type of elements to sort
     * @param data Vector of elements to sort
     * @param comparator Comparison function
     */
    template <typename T>
    void merge_sort(std::vector<T>& data, std::function<bool(const T&, const T&)> comparator);
    
    /**
     * @brief Quick sort implementation
     * @tparam T Type of elements to sort
     * @param data Vector of elements to sort
     * @param comparator Comparison function
     */
    template <typename T>
    void quick_sort(std::vector<T>& data, std::function<bool(const T&, const T&)> comparator);
    
    /**
     * @brief Heap sort implementation
     * @tparam T Type of elements to sort
     * @param data Vector of elements to sort
     * @param comparator Comparison function
     */
    template <typename T>
    void heap_sort(std::vector<T>& data, std::function<bool(const T&, const T&)> comparator);
}