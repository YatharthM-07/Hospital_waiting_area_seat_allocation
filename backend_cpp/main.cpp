#include "patient.h"
#include "patient_graph.h"  // If using a graph
#include <iostream>
#include <vector>

int main() {
    // Step 1: Read patient data from the CSV file in the "data" folder
    std::vector<Patient> patients = readCSV();  // This automatically looks for the file in "data/hospital_patient_dataset.csv"

    // Step 2: Sort patients based on their priority
    sortPatients(patients);

    // Step 3: Allocate seats to the patients
    heapBasedSeatAllocation(patients);

    return 0;
}
