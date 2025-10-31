#pragma once

#include <string>

struct Student {
    std::string m_name;         // Name
    std::string m_surname;      // Surname
    std::string m_email;        // [a-z.]*@student.org
    int m_birth_year;           // 1950..2010
    int m_birth_month;          // 1..12
    int m_birth_day;            // 1..Number of days in m_birth_month
    std::string m_group;        // [A-Z][A-Z][A-Z]-[0-9][0-9]
    float m_rating;             // 0..100
    std::string m_phone_number; // 38(0xx)xx-xx-xxx
    
    /**
     * @brief Default constructor
     */
    Student();
    
    /**
     * @brief Parameterized constructor
     */
    Student(const std::string& name, const std::string& surname, const std::string& email,
            int birth_year, int birth_month, int birth_day, const std::string& group,
            float rating, const std::string& phone_number);
    
    /**
     * @brief Get full name
     * @return Full name (surname + name)
     */
    std::string get_full_name() const;
        
};

/**
 * @brief Comparison functions for Student sorting
 */
namespace student_comparators {
    /**
     * @brief Compare students by surname, then by name (alphabetically)
     */
    bool compare_by_surname_and_name(const Student& a, const Student& b);
    
    /**
     * @brief Compare students by rating (ascending order)
     */
    bool compare_by_rating(const Student& a, const Student& b);
    
    /**
     * @brief Compare students by rating (descending order)
     */
    bool compare_by_rating_desc(const Student& a, const Student& b);
    
    // Additional comparators
    bool compare_by_name(const Student& a, const Student& b);
    bool compare_by_surname(const Student& a, const Student& b);
    bool compare_by_email(const Student& a, const Student& b);
    bool compare_by_birth_date(const Student& a, const Student& b);
    bool compare_by_group(const Student& a, const Student& b);
    bool compare_by_rating(const Student& a, const Student& b);
    bool compare_by_rating_desc(const Student& a, const Student& b);
}