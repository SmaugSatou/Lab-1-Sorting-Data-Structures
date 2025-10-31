#include <algorithm>
#include <set>
#include <iostream>
#include <vector>

#include "database_treemap.hpp"
#include "csv_handler.hpp"

DatabaseTreeMap::DatabaseTreeMap() : data() {}

DatabaseTreeMap::DatabaseTreeMap(const std::vector<Student>& initial_data) {
    for (const auto& student : initial_data) {
        data[student.m_phone_number] = student;
    }
}

bool DatabaseTreeMap::load_from_file(const std::string& filename) {
    std::vector<Student> temp = csv::read_csv(filename);
    data.clear();

    for (const auto& student : temp) {
        data[student.m_phone_number] = student;
    }

    return !data.empty();
}

bool DatabaseTreeMap::save_to_file(const std::string& filename) const {
    return csv::write_csv(filename, to_vector());
}

void DatabaseTreeMap::add(const Student& student) {
    data[student.m_phone_number] = student;
}

bool DatabaseTreeMap::remove_by_phone(const std::string& phone_number) {
    auto it = data.find(phone_number);

    if (it != data.end()) {
        data.erase(it);
        return true;
    }

    return false;
}

size_t DatabaseTreeMap::size() const {
    return data.size();
}

bool DatabaseTreeMap::empty() const {
    return data.empty();
}

void DatabaseTreeMap::clear() {
    data.clear();
}

std::vector<Student> DatabaseTreeMap::to_vector() const {
    std::vector<Student> result;
    result.reserve(data.size());

    for (const auto& pair : data) {
        result.push_back(pair.second);
    }

    return result;
}

bool DatabaseTreeMap::change_group_by_phone(const std::string& phone_number, const std::string& new_group) {
    auto it = data.find(phone_number);

    if (it != data.end()) {
        it->second.m_group = new_group;
        return true;
    }

    return false;
}

std::vector<Student> DatabaseTreeMap::get_students_by_group_sorted(const std::string& group) const {
    std::vector<Student> result;
    
    for (const auto& pair : data) {
        if (pair.second.m_group == group) {
            result.push_back(pair.second);
        }
    }
    
    std::sort(result.begin(), result.end(), 
              student_comparators::compare_by_surname_and_name);
    
    return result;
}

std::vector<std::string> DatabaseTreeMap::get_groups_by_surname(const std::string& surname) const {
    std::set<std::string> unique_groups;
    
    for (const auto& pair : data) {
        if (pair.second.m_surname == surname) {
            unique_groups.insert(pair.second.m_group);
        }
    }
    
    return std::vector<std::string>(unique_groups.begin(), unique_groups.end());
}

bool DatabaseTreeMap::sort_by_rating_and_save(
    const std::string& filename,
    std::function<void(std::vector<Student>&, std::function<bool(const Student&, const Student&)>)> sort_func,
    bool ascending) {
    
    std::vector<Student> sorted_data = to_vector();
    
    auto comparator = ascending ? student_comparators::compare_by_rating 
                                : student_comparators::compare_by_rating_desc;
    
    sort_func(sorted_data, comparator);
    
    return csv::write_csv(filename, sorted_data);
}

size_t DatabaseTreeMap::estimate_memory_usage() const {
    size_t memory = sizeof(DatabaseTreeMap);
    
    // Red-Black Tree overhead: each node has parent/left/right pointers + color bit
    size_t node_overhead = sizeof(void*) * 3 + sizeof(bool); // Three pointers + color
    
    // Each entry has key + value + node overhead
    memory += data.size() * (sizeof(std::string) + sizeof(Student) + node_overhead);
    
    for (const auto& pair : data) {
        memory += pair.first.capacity(); // phone number key
        memory += pair.second.m_name.capacity();
        memory += pair.second.m_surname.capacity();
        memory += pair.second.m_email.capacity();
        memory += pair.second.m_group.capacity();
        memory += pair.second.m_phone_number.capacity();
    }
    
    return memory;
}

std::string DatabaseTreeMap::get_container_name() const {
    return "std::map";
}
