#include "patient.h"
#include "allocation_logic.h"
#include "utils.h"
#include <iostream>
#include <stdexcept>
#include <set>

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <input_csv>" << std::endl;
            return 1;
        }

        std::string csv_file = argv[1];
        std::vector<Patient> patients = read_csv(csv_file);

        if (!patients.empty()) {
            // Check for duplicate ID
            std::set<std::string> ids;
            for (size_t i = 0; i < patients.size() - 1; ++i) {
                ids.insert(patients[i].id);
            }
            const Patient& new_patient = patients.back();
            if (ids.find(new_patient.id) != ids.end()) {
                std::cerr << "Error: Duplicate ID " << new_patient.id << std::endl;
                std::cout << "-1" << std::endl; // Indicate failure
                return 1;
            }

            AllocationLogic allocator;
            int seat = allocator.assign_seat(new_patient, patients);
            std::cout << seat << std::endl; // Output seat number
            write_csv(csv_file, patients); // Update CSV
        } else {
            std::cerr << "Error: No patients in CSV" << std::endl;
            std::cout << "-1" << std::endl;
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "-1" << std::endl;
        return 1;
    }
}