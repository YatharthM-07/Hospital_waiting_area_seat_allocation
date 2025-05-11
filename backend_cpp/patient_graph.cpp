#include "patient_graph.h"
#include <iostream>

void PatientGraph::addPatient(const Patient& p) {
    patients.push_back(p);
    adjList.push_back(std::vector<int>());
}

void PatientGraph::addRelationship(int patient1Index, int patient2Index) {
    // Add a bidirectional edge between the two patients
    adjList[patient1Index].push_back(patient2Index);
    adjList[patient2Index].push_back(patient1Index);
}

void PatientGraph::printRelationships() {
    for (int i = 0; i < adjList.size(); ++i) {
        std::cout << "Patient " << i << " has relationships with: ";
        for (int j : adjList[i]) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
}
