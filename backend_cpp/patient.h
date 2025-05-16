#ifndef PATIENT_H
#define PATIENT_H

#include <string>

class Patient {
public:
    std::string id;
    std::string name;
    int age;
    std::string gender;
    std::string urgency;
    std::string contagion_risk;
    std::string special_needs;
    double priority_score;
    int seat_number;

    Patient(std::string id, std::string name, int age, std::string gender,
            std::string urgency, std::string contagion_risk,
            std::string special_needs);

    void calculate_priority();
};

#endif // PATIENT_H
