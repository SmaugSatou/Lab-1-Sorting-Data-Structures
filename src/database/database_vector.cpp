#include <algorithm>
#include <set>
#include <iostream>

#include "database_vector.hpp"
#include "csv_handler.hpp"

DatabaseVector::DatabaseVector() : data() {}

DatabaseVector::DatabaseVector(const std::vector<Student>& initial_data) : data(initial_data) {}

bool DatabaseVector::load_from_file(const std::string& filename) {
    data = csv::read_csv(filename);
    return !data.empty();
}

bool DatabaseVector::save_to_file(const std::string& filename) const {
    return csv::write_csv(filename, data);
}

void DatabaseVector::add(const Student& student) {
    data.push_back(student);
}

bool DatabaseVector::remove_by_phone(const std::string& phone_number) {
    auto it = std::find_if(data.begin(), data.end(), 
    [&phone_number](const Student& s) {
        return s.m_phone_number == phone_number;
    });
    
    if (it != data.end()) {
        data.erase(it);
        return true;
    }

    return false;
}

size_t DatabaseVector::size() const {
    return data.size();
}

bool DatabaseVector::empty() const {
    return data.empty();
}

void DatabaseVector::clear() {
    data.clear();
}

std::vector<Student> DatabaseVector::to_vector() const {
    return data;
}

bool DatabaseVector::change_group_by_phone(const std::string& phone_number, const std::string& new_group) {
    for (auto& student : data) {
        if (student.m_phone_number == phone_number) {
            student.m_group = new_group;
            return true;
        }
    }

    return false;
}

std::vector<Student> DatabaseVector::get_students_by_group_sorted(const std::string& group) const {
    std::vector<Student> result;
    result.reserve(data.size() / 10);
    
    for (const auto& student : data) {
        if (student.m_group == group) {
            result.push_back(student);
        }
    }
    
    std::sort(result.begin(), result.end(), 
              student_comparators::compare_by_surname_and_name);
    
    return result;
}

std::vector<std::string> DatabaseVector::get_groups_by_surname(const std::string& surname) const {
    std::set<std::string> unique_groups;
    
    for (const auto& student : data) {
        if (student.m_surname == surname) {
            unique_groups.insert(student.m_group);
        }
    }
    
    return std::vector<std::string>(unique_groups.begin(), unique_groups.end());
}

bool DatabaseVector::sort_by_rating_and_save(
    const std::string& filename,
    std::function<void(std::vector<Student>&, std::function<bool(const Student&, const Student&)>)> sort_func,
    bool ascending) {
    
    std::vector<Student> sorted_data = data;
    
    auto comparator = ascending ? student_comparators::compare_by_rating 
                                : student_comparators::compare_by_rating_desc;
    
    sort_func(sorted_data, comparator);
    
    return csv::write_csv(filename, sorted_data);
}

size_t DatabaseVector::estimate_memory_usage() const {
    size_t memory = sizeof(DatabaseVector);
    memory += data.capacity() * sizeof(Student);
    
    for (const auto& student : data) {
        memory += student.m_name.capacity();
        memory += student.m_surname.capacity();
        memory += student.m_email.capacity();
        memory += student.m_group.capacity();
        memory += student.m_phone_number.capacity();
    }
    
    return memory;
}

std::string DatabaseVector::get_container_name() const {
    return "std::vector";
}