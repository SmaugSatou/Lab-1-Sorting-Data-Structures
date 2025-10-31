#include <algorithm>
#include <set>
#include <iostream>
#include <vector>

#include "database_hybrid.hpp"
#include "csv_handler.hpp"

DatabaseHybrid::DatabaseHybrid() : primary_data(), group_index(), surname_index() {}

DatabaseHybrid::DatabaseHybrid(const std::vector<Student>& initial_data) {
    for (const auto& student : initial_data) {
        add(student);
    }
}

void DatabaseHybrid::add_to_indices(const Student& student) {
    group_index.insert({student.m_group, student.m_phone_number});
    surname_index.insert({student.m_surname, student.m_phone_number});
}

void DatabaseHybrid::remove_from_indices(const Student& student) {
    auto group_range = group_index.equal_range(student.m_group);
    for (auto it = group_range.first; it != group_range.second; ) {
        if (it->second == student.m_phone_number) {
            it = group_index.erase(it);
        } else {
            ++it;
        }
    }
    
    auto surname_range = surname_index.equal_range(student.m_surname);
    for (auto it = surname_range.first; it != surname_range.second; ) {
        if (it->second == student.m_phone_number) {
            it = surname_index.erase(it);
        } else {
            ++it;
        }
    }
}

void DatabaseHybrid::update_group_index(const std::string& phone, 
                                         const std::string& old_group, 
                                         const std::string& new_group) {
    auto group_range = group_index.equal_range(old_group);
    for (auto it = group_range.first; it != group_range.second; ) {
        if (it->second == phone) {
            it = group_index.erase(it);
            break;
        } else {
            ++it;
        }
    }
    
    group_index.insert({new_group, phone});
}

bool DatabaseHybrid::load_from_file(const std::string& filename) {
    std::vector<Student> temp = csv::read_csv(filename);
    clear();

    for (const auto& student : temp) {
        add(student);
    }

    return !primary_data.empty();
}

bool DatabaseHybrid::save_to_file(const std::string& filename) const {
    return csv::write_csv(filename, to_vector());
}

void DatabaseHybrid::add(const Student& student) {
    auto it = primary_data.find(student.m_phone_number);
    
    if (it != primary_data.end()) {
        remove_from_indices(it->second);
    }
    
    primary_data[student.m_phone_number] = student;
    
    add_to_indices(student);
}

bool DatabaseHybrid::remove_by_phone(const std::string& phone_number) {
    auto it = primary_data.find(phone_number);

    if (it != primary_data.end()) {
        remove_from_indices(it->second);
        
        primary_data.erase(it);
        return true;
    }

    return false;
}

size_t DatabaseHybrid::size() const {
    return primary_data.size();
}

bool DatabaseHybrid::empty() const {
    return primary_data.empty();
}

void DatabaseHybrid::clear() {
    primary_data.clear();
    group_index.clear();
    surname_index.clear();
}

std::vector<Student> DatabaseHybrid::to_vector() const {
    std::vector<Student> result;
    result.reserve(primary_data.size());

    for (const auto& pair : primary_data) {
        result.push_back(pair.second);
    }

    return result;
}

bool DatabaseHybrid::change_group_by_phone(const std::string& phone_number, 
                                            const std::string& new_group) {
    auto it = primary_data.find(phone_number);

    if (it != primary_data.end()) {
        std::string old_group = it->second.m_group;
        
        it->second.m_group = new_group;
        
        update_group_index(phone_number, old_group, new_group);
        
        return true;
    }
    
    return false;
}

std::vector<Student> DatabaseHybrid::get_students_by_group_sorted(const std::string& group) const {
    std::vector<Student> result;
    
    auto range = group_index.equal_range(group);
    
    for (auto it = range.first; it != range.second; ++it) {
        const std::string& phone = it->second;
        auto student_it = primary_data.find(phone);
        
        if (student_it != primary_data.end()) {
            result.push_back(student_it->second);
        }
    }
    
    std::sort(result.begin(), result.end(), 
              student_comparators::compare_by_surname_and_name);
    
    return result;
}

std::vector<std::string> DatabaseHybrid::get_groups_by_surname(const std::string& surname) const {
    std::set<std::string> unique_groups;
    
    auto range = surname_index.equal_range(surname);
    
    for (auto it = range.first; it != range.second; ++it) {
        const std::string& phone = it->second;
        auto student_it = primary_data.find(phone);
        
        if (student_it != primary_data.end()) {
            unique_groups.insert(student_it->second.m_group);
        }
    }
    
    return std::vector<std::string>(unique_groups.begin(), unique_groups.end());
}

bool DatabaseHybrid::sort_by_rating_and_save(
    const std::string& filename,
    std::function<void(std::vector<Student>&, std::function<bool(const Student&, const Student&)>)> sort_func,
    bool ascending) {
    
    std::vector<Student> sorted_data = to_vector();
    
    auto comparator = ascending ? student_comparators::compare_by_rating 
                                : student_comparators::compare_by_rating_desc;
    
    sort_func(sorted_data, comparator);
    
    return csv::write_csv(filename, sorted_data);
}

size_t DatabaseHybrid::estimate_memory_usage() const {
    size_t memory = sizeof(DatabaseHybrid);
    
    size_t bucket_count = primary_data.bucket_count();
    memory += bucket_count * sizeof(void*);
    memory += primary_data.size() * (sizeof(std::string) + sizeof(Student) + sizeof(size_t) + sizeof(void*));
    
    for (const auto& pair : primary_data) {
        memory += pair.first.capacity(); // phone number key
        memory += pair.second.m_name.capacity();
        memory += pair.second.m_surname.capacity();
        memory += pair.second.m_email.capacity();
        memory += pair.second.m_group.capacity();
        memory += pair.second.m_phone_number.capacity();
    }
    
    // Group index (multimap overhead: tree nodes + pointers)
    memory += group_index.size() * (sizeof(std::string) * 2 + sizeof(void*) * 3); // key + value + 3 pointers (parent, left, right)
    for (const auto& pair : group_index) {
        memory += pair.first.capacity();  // group name
        memory += pair.second.capacity(); // phone number
    }
    
    // Surname index (multimap overhead: tree nodes + pointers)
    memory += surname_index.size() * (sizeof(std::string) * 2 + sizeof(void*) * 3);
    for (const auto& pair : surname_index) {
        memory += pair.first.capacity();  // surname
        memory += pair.second.capacity(); // phone number
    }
    
    return memory;
}

std::string DatabaseHybrid::get_container_name() const {
    return "Hybrid (unordered_map + 2x multimap indices)";
}
