#pragma once

#include <string>
#include <vector>

#include "student.hpp"

namespace csv {

    /**
     * @brief Split line by coma
     * @param Line Line with student's info
     * @return Vector of strings
     */
    std::vector<std::string> split_csv_line(const std::string& line);

    /**
     * @brief Read student data from CSV file
     * @param filename Path to the CSV file
     * @return Vector of Student objects
     */
    std::vector<Student> read_csv(const std::string& filename);
    
    /**
     * @brief Write student data to CSV file
     * @param filename Path to the output CSV file
     * @param students Vector of Student objects to write
     * @return true if successful, false otherwise
     */
    bool write_csv(const std::string& filename, const std::vector<Student>& students);
    
    /**
     * @brief Parse a single line from CSV
     * @param line CSV line to parse
     * @return Student object
     */
    Student parse_line(const std::string& line);
    
    /**
     * @brief Convert Student object to CSV line
     * @param student Student object to convert
     * @return CSV formatted string
     */
    std::string to_csv_line(const Student& student);
}