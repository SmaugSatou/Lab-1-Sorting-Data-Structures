#include <fstream>
#include <sstream>
#include <iostream>

#include "csv_handler.hpp"

namespace csv {
        
    std::vector<std::string> split_csv_line(const std::string& line) {
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
    
    Student parse_line(const std::string& line) {
        std::vector<std::string> tokens = split_csv_line(line);
        
        if (tokens.size() != 9) {
            throw std::runtime_error("Invalid CSV line: expected 9 fields, got " + std::to_string(tokens.size()));
        }
        
        Student student(
            tokens[0],                  // m_name
            tokens[1],                  // m_surname
            tokens[2],                  // m_email
            std::stoi(tokens[3]),       // m_birth_year
            std::stoi(tokens[4]),       // m_birth_month
            std::stoi(tokens[5]),       // m_birth_day
            tokens[6],                  // m_group
            std::stof(tokens[7]),       // m_rating
            tokens[8]                   // m_phone_number
        );
        
        return student;
    }
    
    std::string to_csv_line(const Student& student) {
        std::ostringstream oss;

        oss << student.m_name << ","
            << student.m_surname << ","
            << student.m_email << ","
            << student.m_birth_year << ","
            << student.m_birth_month << ","
            << student.m_birth_day << ","
            << student.m_group << ","
            << student.m_rating << ","
            << student.m_phone_number;

        return oss.str();
    }
    
    std::vector<Student> read_csv(const std::string& filename) {
        std::vector<Student> students;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return students;
        }
        
        std::string line;

        // Skip header line
        if (std::getline(file, line)) {}
        
        while (std::getline(file, line)) {
            if (line.empty()) {
                continue;
            }
            
            try {
                Student student = parse_line(line);
                students.push_back(student);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing line: " << line << std::endl;
                std::cerr << "Exception: " << e.what() << std::endl;
            }
        }
        
        file.close();

        std::cout << "Successfully read " << students.size() << " students from " << filename << std::endl;

        return students;
    }
    
    bool write_csv(const std::string& filename, const std::vector<Student>& students) {
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Could not create file " << filename << std::endl;
            return false;
        }
        
        // Write header
        file << "m_name,m_surname,m_email,m_birth_year,m_birth_month,m_birth_day,m_group,m_rating,m_phone_number\n";
        
        // Write student data
        for (const auto& student : students) {
            file << to_csv_line(student) << "\n";
        }
        
        file.close();
    
        std::cout << "Successfully wrote " << students.size() << " students to " << filename << std::endl;
    
        return true;
    }
    
}