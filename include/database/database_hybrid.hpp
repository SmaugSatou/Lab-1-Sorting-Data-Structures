#pragma once

#include <unordered_map>
#include <map>
#include <string>

#include "database_interface.hpp"

/**
 * @brief Hybrid Database implementation combining multiple data structures
 * 
 * - Primary storage: std::unordered_map<phone, Student> for O(1) phone-based lookups
 * - Group index: std::multimap<group, phone> for O(log n + k) group queries
 * - Surname index: std::multimap<surname, phone> for O(log n + k) surname queries
 */

class DatabaseHybrid : public IStudentDatabase {
private:
    std::unordered_map<std::string, Student> primary_data;
    
    std::multimap<std::string, std::string> group_index;    // group -> phone
    std::multimap<std::string, std::string> surname_index;  // surname -> phone

    // Helper methods to maintain index consistency
    void add_to_indices(const Student& student);
    void remove_from_indices(const Student& student);
    void update_group_index(const std::string& phone, const std::string& old_group, const std::string& new_group);

public:
    DatabaseHybrid();
    explicit DatabaseHybrid(const std::vector<Student>& initial_data);
    
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
