#pragma once

#include <vector>
#include <string>

#include "database_interface.hpp"

/**
 * @brief Approach 1: Database implementation using std::vector
 * 
 * Container: std::vector<Student>
 */

class DatabaseVector : public IStudentDatabase {
private:
    std::vector<Student> data;

public:
    DatabaseVector();
    explicit DatabaseVector(const std::vector<Student>& initial_data);
    
    bool load_from_file(const std::string& filename) override;
    bool save_to_file(const std::string& filename) const override;
    void add(const Student& student) override;
    bool remove_by_phone(const std::string& phone_number) override;

    size_t size() const override;
    bool empty() const override;
    void clear() override;
    std::vector<Student> to_vector() const override;
    
    bool change_group_by_phone(const std::string& phone_number, const std::string& new_group) override;
    std::vector<Student> get_students_by_group_sorted(const std::string& group) const override;
    std::vector<std::string> get_groups_by_surname(const std::string& surname) const override;
    bool sort_by_rating_and_save(const std::string& filename,
                                  std::function<void(std::vector<Student>&, std::function<bool(const Student&, const Student&)>)> sort_func,
                                  bool ascending = true) override;
    
    size_t estimate_memory_usage() const override;
    std::string get_container_name() const override;
};
