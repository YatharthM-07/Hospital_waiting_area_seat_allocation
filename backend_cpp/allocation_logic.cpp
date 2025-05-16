#include "allocation_logic.h"
#include <algorithm>
#include <stdexcept>

AllocationLogic::AllocationLogic() {}

// Assigns a seat to the patient based on priority and contagion risk
// Returns seat number (0-49) or -1 if no seat is available
int AllocationLogic::assign_seat(const Patient& patient, std::vector<Patient>& patients) {
    // Validate patient data
    if (patient.id.empty() || patient.urgency.empty()) {
        return -1; // Invalid patient data
    }

    // Determine if patient is priority or contagious
    bool is_priority = patient.urgency == "High" || patient.age > 60 || patient.special_needs == "Yes";
    bool is_contagious = patient.contagion_risk == "High";

    // Initialize seat tracking: occupied (true if taken) and blocked (true if adjacent to contagious patient)
    std::vector<bool> occupied(TOTAL_SEATS, false);
    std::vector<bool> blocked(TOTAL_SEATS, false);

    // Mark occupied and blocked seats based on existing patients
    for (const auto& p : patients) {
        if (p.seat_number >= 0 && p.seat_number < TOTAL_SEATS) {
            occupied[p.seat_number] = true;
            if (p.contagion_risk == "High") {
                int row = p.seat_number / COLS;
                int col = p.seat_number % COLS;
                // Block seats according to rules
                if (col == 0 && !occupied[row * COLS + 1]) {
                    blocked[row * COLS + 1] = true; // Left edge: block right
                } else if (col == 9 && !occupied[row * COLS + 8]) {
                    blocked[row * COLS + 8] = true; // Right edge: block left
                } else {
                    // Middle seats: block left and right
                    if (col > 0 && !occupied[row * COLS + (col - 1)]) {
                        blocked[row * COLS + (col - 1)] = true; // Left
                    }
                    if (col < COLS - 1 && !occupied[row * COLS + (col + 1)]) {
                        blocked[row * COLS + (col + 1)] = true; // Right
                    }
                }
            }
        }
    }

    // Assign seat based on priority
    int seat = -1;
    if (is_priority) {
        // Try front row (seats 0-9) for priority patients
        for (int i = 0; i < 10; ++i) {
            if (!occupied[i] && !blocked[i] && (!is_contagious || is_seat_valid_for_contagious(i, patients, occupied))) {
                seat = i;
                break;
            }
        }
        // If no front-row seat, displace a non-priority patient
        if (seat == -1) {
            for (auto& p : patients) {
                if (p.seat_number >= 0 && p.seat_number < 10 &&
                    p.urgency != "High" && p.age <= 60 && p.special_needs != "Yes" &&
                    (!is_contagious || is_seat_valid_for_contagious(p.seat_number, patients, occupied))) {
                    seat = p.seat_number;
                    // Reassign displaced patient to another seat
                    if (!reassign_displaced_patient(p, patients, occupied, blocked)) {
                        continue; // Skip if reassignment fails
                    }
                    break;
                }
            }
        }
    }

    // If no priority seat or not priority, find any available seat
    if (seat == -1) {
        for (int i = 0; i < TOTAL_SEATS; ++i) {
            if (!occupied[i] && !blocked[i] && (!is_contagious || is_seat_valid_for_contagious(i, patients, occupied))) {
                seat = i;
                break;
            }
        }
    }

    // Update seat number in patients vector (match by ID)
    if (seat >= 0) {
        for (auto& p : patients) {
            if (p.id == patient.id) {
                p.seat_number = seat;
                break;
            }
        }
    }

    return seat;
}

// Helper function to check if a seat is valid for a contagious patient
bool AllocationLogic::is_seat_valid_for_contagious(int seat, const std::vector<Patient>& patients, const std::vector<bool>& occupied) {
    int row = seat / COLS;
    int col = seat % COLS;
    std::vector<int> required_blocked_seats;

    // Determine seats that would need to be blocked if this patient is contagious
    if (col == 0) {
        required_blocked_seats.push_back(row * COLS + 1); // Right seat
    } else if (col == 9) {
        required_blocked_seats.push_back(row * COLS + 8); // Left seat
    } else {
        if (col > 0) required_blocked_seats.push_back(row * COLS + (col - 1)); // Left
        if (col < COLS - 1) required_blocked_seats.push_back(row * COLS + (col + 1)); // Right
    }

    // Check if any required blocked seat is occupied
    for (int blocked_seat : required_blocked_seats) {
        if (blocked_seat >= 0 && blocked_seat < TOTAL_SEATS && occupied[blocked_seat]) {
            return false; // Cannot assign this seat, as it would block an occupied seat
        }
    }

    // Check if this seat is blocked by another contagious patient
    for (const auto& p : patients) {
        if (p.contagion_risk == "High" && p.seat_number >= 0 && p.seat_number < TOTAL_SEATS) {
            int p_row = p.seat_number / COLS;
            int p_col = p.seat_number % COLS;
            if (p_row == row) { // Same row
                if (p_col == 0 && col == 1) return false; // Left edge blocks right
                if (p_col == 9 && col == 8) return false; // Right edge blocks left
                if (p_col > 0 && p_col < 9) {
                    if (col == p_col - 1 || col == p_col + 1) return false; // Middle blocks left/right
                }
            }
        }
    }

    return true;
}

// Helper function to reassign a displaced patient
bool AllocationLogic::reassign_displaced_patient(Patient& patient, std::vector<Patient>& patients, const std::vector<bool>& occupied, const std::vector<bool>& blocked) {
    bool is_contagious = patient.contagion_risk == "High";
    for (int i = 0; i < TOTAL_SEATS; ++i) {
        if (!occupied[i] && !blocked[i] && (!is_contagious || is_seat_valid_for_contagious(i, patients, occupied))) {
            patient.seat_number = i;
            return true;
        }
    }
    patient.seat_number = -1; // No seat available
    return false;
}