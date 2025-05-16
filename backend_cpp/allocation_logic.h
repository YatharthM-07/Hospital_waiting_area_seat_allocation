#ifndef ALLOCATION_LOGIC_H
#define ALLOCATION_LOGIC_H

#include <vector>
#include "patient.h"

class AllocationLogic {
private:
    static const int ROWS = 5;
    static const int COLS = 10;
    static const int TOTAL_SEATS = ROWS * COLS;

    // Helper function to check if a seat is valid for a contagious patient
    bool is_seat_valid_for_contagious(int seat, const std::vector<Patient>& patients, const std::vector<bool>& occupied);
    // Helper function to reassign a displaced patient
    bool reassign_displaced_patient(Patient& patient, std::vector<Patient>& patients, const std::vector<bool>& occupied, const std::vector<bool>& blocked);

public:
    AllocationLogic();
    int assign_seat(const Patient& patient, std::vector<Patient>& patients);
};

#endif