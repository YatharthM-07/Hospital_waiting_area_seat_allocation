#ifndef PATIENT_H
#define PATIENT_H

#include <iostream>
#include <vector>
#include <string>

// Struct to represent a patient
struct Patient {
    int age;
    std::string gender;
    int urgency;
    int contagionRisk;
    bool specialNeeds;

    bool operator<(const Patient& other) const;  // For sorting in priority queue
};

// Function declarations
std::vector<Patient> readCSV(const std::string& filename);
void sortPatients(std::vector<Patient>& patients);
void allocateSeats(const std::vector<Patient>& patients);
void heapBasedSeatAllocation(std::vector<Patient>& patients);

#endif
