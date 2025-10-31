#include "student.hpp"

// Constructors
Student::Student() 
    : m_name(""), m_surname(""), m_email(""), 
      m_birth_year(0), m_birth_month(0), m_birth_day(0),
      m_group(""), m_rating(0.0f), m_phone_number("") {}

Student::Student(const std::string& name, const std::string& surname, const std::string& email,
            int birth_year, int birth_month, int birth_day, const std::string& group,
            float rating, const std::string& phone_number) 
    : m_name(name), m_surname(surname), m_email(email), 
      m_birth_year(birth_year), m_birth_month(birth_month), m_birth_day(birth_day),
      m_group(group), m_rating(rating), m_phone_number(phone_number) {}

std::string Student::get_full_name() const {
    return m_surname + " " + m_name;
}

// Comparison functions
namespace student_comparators {
    // Compare by surname and name (alphabetically)
    bool compare_by_surname_and_name(const Student& a, const Student& b) {
        if (a.m_surname != b.m_surname) {
            return a.m_surname < b.m_surname;
        }

        return a.m_name < b.m_name;
    }
    
    // Compare by rating (ascending)
    bool compare_by_rating(const Student& a, const Student& b) {
        return a.m_rating < b.m_rating;
    }
    
    // Compare by rating (descending)
    bool compare_by_rating_desc(const Student& a, const Student& b) {
        return a.m_rating > b.m_rating;
    }
    
    // Additional comparators
    bool compare_by_name(const Student& a, const Student& b) {
        return a.m_name < b.m_name;
    }
    
    bool compare_by_surname(const Student& a, const Student& b) {
        return a.m_surname < b.m_surname;
    }
    
    bool compare_by_email(const Student& a, const Student& b) {
        return a.m_email < b.m_email;
    }
    
    bool compare_by_birth_date(const Student& a, const Student& b) {
        if (a.m_birth_year != b.m_birth_year) {
            return a.m_birth_year < b.m_birth_year;
        }

        if (a.m_birth_month != b.m_birth_month) {
            return a.m_birth_month < b.m_birth_month;
        }

        return a.m_birth_day < b.m_birth_day;
    }
    
    bool compare_by_group(const Student& a, const Student& b) {
        return a.m_group < b.m_group;
    }
}
