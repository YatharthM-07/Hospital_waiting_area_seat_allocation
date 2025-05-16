#include "utils.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream> // Added for std::cerr

std::vector<Patient> read_csv(const std::string& filename) {
    std::vector<Patient> patients;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 8) {
            try {
                int age = std::stoi(tokens[2]);
                Patient p(tokens[0], tokens[1], age, tokens[3], tokens[4],
                          tokens[5], tokens[6]);
                p.priority_score = std::stod(tokens[7]);
                if (tokens.size() > 8 && !tokens[8].empty()) {
                    p.seat_number = std::stoi(tokens[8]);
                }
                patients.push_back(p);
            } catch (const std::exception& e) {
                std::cerr << "Warning: Skipping invalid CSV line: " << line << std::endl;
                continue;
            }
        }
    }
    file.close();
    return patients;
}

void write_csv(const std::string& filename, const std::vector<Patient>& patients) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to write to file: " + filename);
    }
    file << "id,name,age,gender,urgency,contagion_risk,special_needs,priority_score,seat_number\n";
    for (const auto& p : patients) {
        file << p.id << "," << p.name << "," << p.age << "," << p.gender << ","
             << p.urgency << "," << p.contagion_risk << "," << p.special_needs << ","
             << p.priority_score << "," << (p.seat_number >= 0 ? std::to_string(p.seat_number) : "") << "\n";
    }
    file.close();
}