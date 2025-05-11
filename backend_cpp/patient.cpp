#include "patient.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

std::vector<Patient> readCSV(const std::string& filename = "data/hospital_patient_dataset.csv") {
    std::vector<Patient> patients;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error opening the file: " << filename << std::endl;
        return patients;  // Return empty vector if file cannot be opened
    }

    std::string line;
    
    // Skip the header line (if it exists)
    std::getline(file, line);

    // Read the rest of the lines (patient data)
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;

        Patient p;
        std::getline(ss, field, ','); p.age = std::stoi(field);  // Extract age
        std::getline(ss, field, ','); p.gender = field;          // Extract gender
        std::getline(ss, field, ','); p.urgency = std::stoi(field);  // Extract urgency
        std::getline(ss, field, ','); p.contagionRisk = std::stoi(field);  // Extract contagionRisk
        std::getline(ss, field, ','); p.specialNeeds = (field == "yes");  // Special needs flag

        patients.push_back(p);  // Add the patient to the vector
    }

    return patients;  // Return the vector of patients
}
