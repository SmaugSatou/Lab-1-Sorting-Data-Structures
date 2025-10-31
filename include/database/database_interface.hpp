#pragma once

#include <vector>
#include <string>
#include <functional>

#include "student.hpp"

/**
 * @brief Base interface for all database implementations
 */

class IStudentDatabase {
public:
    virtual ~IStudentDatabase() = default;
    
    // Basic operations
    virtual bool load_from_file(const std::string& filename) = 0;
    virtual bool save_to_file(const std::string& filename) const = 0;
    virtual void add(const Student& student) = 0;
    virtual bool remove_by_phone(const std::string& phone_number) = 0;

    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;
    virtual std::vector<Student> to_vector() const = 0;
    
    // Task operations (V3 - 5:10:100 - S2)
    // Operation 1: Change student's group by phone number
    virtual bool change_group_by_phone(const std::string& phone_number, const std::string& new_group) = 0;
    
    // Operation 2: Get students from specific group sorted by surname and name
    virtual std::vector<Student> get_students_by_group_sorted(const std::string& group) const = 0;
    
    // Operation 3: Get list of groups where students with specific surname study
    virtual std::vector<std::string> get_groups_by_surname(const std::string& surname) const = 0;
    
    // Sorting operation for Task III (S2)
    virtual bool sort_by_rating_and_save(const std::string& filename,
                                         std::function<void(std::vector<Student>&, std::function<bool(const Student&, const Student&)>)> sort_func,
                                         bool ascending = true) = 0;
    
    virtual size_t estimate_memory_usage() const = 0;
    
    virtual std::string get_container_name() const = 0;
};
