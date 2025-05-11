#ifndef PATIENT_GRAPH_H
#define PATIENT_GRAPH_H

#include <vector>
#include <string>
#include "patient.h"  // Assuming you need the Patient struct from patient.h

class PatientGraph {
public:
    // Add a patient to the graph
    void addPatient(const Patient& p);
    
    // Add a relationship between two patients (e.g., keeping them apart due to contagion risk)
    void addRelationship(int patient1Index, int patient2Index);
    
    // Print relationships or process relationships (for seat allocation logic)
    void printRelationships();

private:
    std::vector<Patient> patients;  // Stores patients
    std::vector<std::vector<int>> adjList;  // Adjacency list for relationships
};

#endif
