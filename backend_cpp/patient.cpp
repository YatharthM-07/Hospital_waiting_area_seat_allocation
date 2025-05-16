#include "patient.h"

Patient::Patient(std::string id, std::string name, int age, std::string gender,
                 std::string urgency, std::string contagion_risk,
                 std::string special_needs)
    : id(id), name(name), age(age), gender(gender), urgency(urgency),
      contagion_risk(contagion_risk), special_needs(special_needs),
      priority_score(0.0), seat_number(-1) {
    calculate_priority();
}

void Patient::calculate_priority() {
    double age_factor = (age > 60) ? 3.0 : (age > 40) ? 2.0 : 1.0;
    double urgency_factor = (urgency == "High") ? 4.0 : (urgency == "Medium") ? 2.0 : 1.0;
    double contagion_factor = (contagion_risk == "High") ? 2.0 : 1.0;
    double special_needs_factor = (special_needs == "Yes") ? 2.0 : 1.0;

    priority_score = age_factor + urgency_factor + contagion_factor + special_needs_factor;
}
