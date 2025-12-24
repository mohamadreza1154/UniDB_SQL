#include "ArrayCollection.h"
#include <algorithm>
#include <fstream>
#include <sstream>

void ArrayCollection::insertOne(const Student& student) {
    students.push_back(student);
}

bool ArrayCollection::deleteOneById(int id) {
    auto it = std::find_if(students.begin(), students.end(),
                           [id](const Student& s) { return s.id == id; });
    if (it != students.end()) {
        students.erase(it);
        return true;
    }
    return false;
}

Student* ArrayCollection::findById(int id) {
    auto it = std::find_if(students.begin(), students.end(),
                           [id](const Student& s) { return s.id == id; });
    if (it != students.end()) {
        return &(*it);
    }
    return nullptr;
}

std::vector<Student> ArrayCollection::findAll() const {
    return students;
}

std::vector<Student> ArrayCollection::filter(const std::string& field,
                                             const std::string& value) const {
    std::vector<Student> result;
    for (const auto& s : students) {
        if (field == "id") {
            int v = std::stoi(value);
            if (s.id == v) {
                result.push_back(s);
            }
        } else if (field == "name") {
            if (s.name == value) {
                result.push_back(s);
            }
        } else if (field == "gpa") {
            double v = std::stod(value);
            if (s.gpa == v) {
                result.push_back(s);
            }
        }
    }
    return result;
}

std::size_t ArrayCollection::count() const {
    return students.size();
}

double ArrayCollection::sum(const std::string& field) const {
    double total = 0.0;
    if (field == "gpa") {
        for (const auto& s : students) {
            total += s.gpa;
        }
    } else if (field == "id") {
        for (const auto& s : students) {
            total += static_cast<double>(s.id);
        }
    }
    return total;
}

double ArrayCollection::average(const std::string& field) const {
    std::size_t n = students.size();
    if (n == 0) {
        return 0.0;
    }
    double s = sum(field);
    return s / static_cast<double>(n);
}

bool ArrayCollection::importFromCsv(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::size_t p1 = line.find(',');
        std::size_t p2 = line.find(',', p1 + 1);
        if (p1 == std::string::npos || p2 == std::string::npos) continue;

        std::string idStr  = line.substr(0, p1);
        std::string name   = line.substr(p1 + 1, p2 - p1 - 1);
        std::string gpaStr = line.substr(p2 + 1);

        Student s;
        s.id = std::stoi(idStr);
        s.name = name;
        s.gpa = std::stod(gpaStr);

        insertOne(s);
    }
    return true;
}
