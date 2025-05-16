#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include "patient.h"

std::vector<Patient> read_csv(const std::string& filename);
void write_csv(const std::string& filename, const std::vector<Patient>& patients);

#endif