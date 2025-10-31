#include <algorithm>

#include "sorting.hpp"
#include "student.hpp"

namespace sort_algorithms {
    
    // Bubble Sort
    void bubble_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator) {
        size_t n = data.size();

        for (size_t i = 0; i < n - 1; ++i) {
            bool swapped = false;
            for (size_t j = 0; j < n - i - 1; ++j) {
                if (!comparator(data[j], data[j + 1])) {
                    std::swap(data[j], data[j + 1]);
                    swapped = true;
                }
            }

            if (!swapped) {
                break;
            }
        }
    }
    
    // Insertion Sort
    void insertion_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator) {
        size_t n = data.size();
    
        for (size_t i = 1; i < n; ++i) {
            Student key = data[i];
            int j = i - 1;
            
            while (j >= 0 && !comparator(data[j], key)) {
                data[j + 1] = data[j];
                --j;
            }

            data[j + 1] = key;
        }
    }
    
    // Selection Sort
    void selection_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator) {
        size_t n = data.size();

        for (size_t i = 0; i < n - 1; ++i) {
            size_t min_idx = i;

            for (size_t j = i + 1; j < n; ++j) {
                if (comparator(data[j], data[min_idx])) {
                    min_idx = j;
                }
            }

            if (min_idx != i) {
                std::swap(data[i], data[min_idx]);
            }
        }
    }
    
    // Merge Sort Helper
    void merge(std::vector<Student>& data, int left, int mid, int right, 
               std::function<bool(const Student&, const Student&)> comparator) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        
        std::vector<Student> L(n1), R(n2);
        
        for (int i = 0; i < n1; ++i) {
            L[i] = data[left + i];
        }

        for (int i = 0; i < n2; ++i) {
            R[i] = data[mid + 1 + i];
        }
        
        int i = 0, j = 0, k = left;
        
        while (i < n1 && j < n2) {
            if (comparator(L[i], R[j]) || !comparator(R[j], L[i])) {
                data[k] = L[i];
                ++i;
            } else {
                data[k] = R[j];
                ++j;
            }

            ++k;
        }
        
        while (i < n1) {
            data[k] = L[i];
            ++i;
            ++k;
        }
        
        while (j < n2) {
            data[k] = R[j];
            ++j;
            ++k;
        }
    }
    
    void merge_sort_helper(std::vector<Student>& data, int left, int right,
                           std::function<bool(const Student&, const Student&)> comparator) {
        if (left >= right) {
            return;
        }
    
        int mid = left + (right - left) / 2;

        merge_sort_helper(data, left, mid, comparator);
        merge_sort_helper(data, mid + 1, right, comparator);
    
        merge(data, left, mid, right, comparator);
    }
    
    // Merge Sort
    void merge_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator) {
        if (data.size() > 1) {
            merge_sort_helper(data, 0, data.size() - 1, comparator);
        }
    }
    
    // Quick Sort Helper
    int partition(std::vector<Student>& data, int low, int high,
                  std::function<bool(const Student&, const Student&)> comparator) {
        Student pivot = data[high];
        int i = low - 1;
        
        for (int j = low; j < high; ++j) {
            if (comparator(data[j], pivot)) {
                ++i;
                std::swap(data[i], data[j]);
            }
        }

        std::swap(data[i + 1], data[high]);

        return i + 1;
    }
    
    void quick_sort_helper(std::vector<Student>& data, int low, int high,
                           std::function<bool(const Student&, const Student&)> comparator) {
        if (low >= high) {
            return;
        }
    
        int pi = partition(data, low, high, comparator);

        quick_sort_helper(data, low, pi - 1, comparator);
        quick_sort_helper(data, pi + 1, high, comparator);
    }
    
    // Quick Sort
    void quick_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator) {
        if (data.size() > 1) {
            quick_sort_helper(data, 0, data.size() - 1, comparator);
        }
    }
    
    // Heap Sort Helper
    void heapify(std::vector<Student>& data, int n, int i,
                 std::function<bool(const Student&, const Student&)> comparator) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        
        if (left < n && !comparator(data[left], data[largest])) {
            largest = left;
        }
        
        if (right < n && !comparator(data[right], data[largest])) {
            largest = right;
        }
        
        if (largest != i) {
            std::swap(data[i], data[largest]);
            heapify(data, n, largest, comparator);
        }
    }
    
    // Heap Sort
    void heap_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator) {
        int n = data.size();
        
        for (int i = n / 2 - 1; i >= 0; --i) {
            heapify(data, n, i, comparator);
        }
        
        for (int i = n - 1; i > 0; --i) {
            std::swap(data[0], data[i]);
            heapify(data, i, 0, comparator);
        }
    }
    
    // Standard library sort
    void std_sort(std::vector<Student>& data, std::function<bool(const Student&, const Student&)> comparator) {
        std::sort(data.begin(), data.end(), comparator);
    }
    
    // Radix Sort for Student rating
    void radix_sort_by_rating(std::vector<Student>& data, 
                             std::function<bool(const Student&, const Student&)> comparator) {
        if (data.empty()) {
            return;
        }
        
        bool ascending = true;

        // Check either acsending or descending
        if (data.size() >= 2) {
            Student s1, s2;
            s1.m_rating = 1;
            s2.m_rating = 2;
            ascending = comparator(s1, s2);
        }
        
        // Convert ratings to integers
        std::vector<std::pair<int, size_t>> indexed_ratings;
        indexed_ratings.reserve(data.size());
        
        for (size_t i = 0; i < data.size(); ++i) {
            int rating_int = static_cast<int>(data[i].m_rating * 100.0f);
            indexed_ratings.push_back({rating_int, i});
        }
        
        const int MAX_RATING = 10000;
        std::vector<int> count(MAX_RATING + 1, 0);
        
        for (const auto& pair : indexed_ratings) {
            count[pair.first]++;
        }
        
        // Cumulative count
        if (ascending) {
            for (int i = 1; i <= MAX_RATING; ++i) {
                count[i] += count[i - 1];
            }
        } else {
            for (int i = MAX_RATING - 1; i >= 0; --i) {
                count[i] += count[i + 1];
            }
        }
        
        std::vector<Student> output(data.size());
        
        if (ascending) {
            for (int i = indexed_ratings.size() - 1; i >= 0; --i) {
                int rating = indexed_ratings[i].first;
                size_t orig_index = indexed_ratings[i].second;
                output[count[rating] - 1] = data[orig_index];
                count[rating]--;
            }
        } else {
            for (int i = indexed_ratings.size() - 1; i >= 0; --i) {
                int rating = indexed_ratings[i].first;
                size_t orig_index = indexed_ratings[i].second;
                output[count[rating] - 1] = data[orig_index];
                count[rating]--;
            }
        }
        
        data = std::move(output);
    }
    
}