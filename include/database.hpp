#pragma once

#include <vector>
#include <string>
#include <functional>

template <typename T>
class DataBase {
private:
    std::vector<T> data;

public:
    /**
     * @brief Default constructor
     */
    DataBase();
    
    /**
     * @brief Constructor with initial data
     * @param initial_data Vector of initial elements
     */
    DataBase(const std::vector<T>& initial_data);
    
    /**
     * @brief Load data from file
     * @param filename Path to the data file
     * @return true if successful, false otherwise
     */
    bool load_from_file(const std::string& filename);
    
    /**
     * @brief Save data to file
     * @param filename Path to the output file
     * @return true if successful, false otherwise
     */
    bool save_to_file(const std::string& filename) const;
    
    /**
     * @brief Add element to database
     * @param element Element to add
     */
    void add(const T& element);
    
    /**
     * @brief Remove element at index
     * @param index Index of element to remove
     * @return true if successful, false otherwise
     */
    bool remove(size_t index);
    
    /**
     * @brief Get element at index
     * @param index Index of element
     * @return Reference to element
     */
    T& get(size_t index);
    
    /**
     * @brief Get element at index (const version)
     * @param index Index of element
     * @return Const reference to element
     */
    const T& get(size_t index) const;
    
    /**
     * @brief Get all data
     * @return Reference to data vector
     */
    std::vector<T>& get_data();
    
    /**
     * @brief Get all data (const version)
     * @return Const reference to data vector
     */
    const std::vector<T>& get_data() const;
    
    /**
     * @brief Get database size
     * @return Number of elements
     */
    size_t size() const;
    
    /**
     * @brief Check if database is empty
     * @return true if empty, false otherwise
     */
    bool empty() const;
    
    /**
     * @brief Clear all data
     */
    void clear();
    
    /**
     * @brief Sort data using specified algorithm
     * @param sort_func Sorting function
     * @param comparator Comparison function
     */
    void sort(std::function<void(std::vector<T>&, std::function<bool(const T&, const T&)>)> sort_func,
              std::function<bool(const T&, const T&)> comparator);
    
    /**
     * @brief Search for element
     * @param predicate Search predicate
     * @return Vector of indices of matching elements
     */
    std::vector<size_t> search(std::function<bool(const T&)> predicate) const;
    
    /**
     * @brief Filter data by predicate
     * @param predicate Filter predicate
     * @return Vector of elements matching predicate
     */
    std::vector<T> filter(std::function<bool(const T&)> predicate) const;
};

// Specialization for Student database - V3 specific tasks
#include "student.hpp"

/**
 * @brief Specialized database class for Student with task-specific methods
 */
class StudentDatabase : public DataBase<Student> {
public:
    /**
     * @brief Default constructor
     */
    StudentDatabase();
    
    /**
     * @brief Constructor with initial data
     * @param initial_data Vector of initial students
     */
    StudentDatabase(const std::vector<Student>& initial_data);
    
    /**
     * @brief Task 1: Change student's group by phone number
     * @param phone_number Student's phone number (m_phone_number)
     * @param new_group New group to assign
     * @return true if student found and updated, false otherwise
     */
    bool change_group_by_phone(const std::string& phone_number, const std::string& new_group);
    
    /**
     * @brief Task 2: Get students from specific group sorted by surname and name
     * @param group Group name (m_group)
     * @return Vector of students from the group sorted alphabetically by surname and name
     */
    std::vector<Student> get_students_by_group_sorted(const std::string& group) const;
    
    /**
     * @brief Task 3: Get list of groups where students with specific surname study
     * @param surname Surname to search for (m_surname)
     * @return Vector of unique group names
     */
    std::vector<std::string> get_groups_by_surname(const std::string& surname) const;
    
    /**
     * @brief Task 4: Sort all students by rating and save to CSV file
     * @param filename Path to output CSV file
     * @param sort_func Sorting function to use
     * @param ascending true for ascending order, false for descending
     * @return true if successful, false otherwise
     */
    bool sort_by_rating_and_save(const std::string& filename,
                                  std::function<void(std::vector<Student>&, std::function<bool(const Student&, const Student&)>)> sort_func,
                                  bool ascending = true);
};
