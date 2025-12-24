#include "ArrayCollection.h"
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;
void ArrayCollection::insertOne(const Student& student) {
    students.push_back(student);
}

bool ArrayCollection::deleteOneById(int id) {
    auto it = find_if(students.begin(), students.end(),
                           [id](const Student& s) { return s.id == id; });
    if (it != students.end()) {
        students.erase(it);
        return true;
    }
    return false;
}

Student* ArrayCollection::findById(int id) {
    auto it = find_if(students.begin(), students.end(),
                           [id](const Student& s) { return s.id == id; });
    if (it != students.end()) {
        return &(*it);
    }
    return nullptr;
}

vector<Student> ArrayCollection::findAll() const {
    return students;
}

vector<Student> ArrayCollection::filter(const string& field,
                                             const string& value) const {
    vector<Student> result;
    for (const auto& s : students) {
        if (field == "id") {
            int v = stoi(value);
            if (s.id == v) {
                result.push_back(s);
            }
        } else if (field == "name") {
            if (s.name == value) {
                result.push_back(s);
            }
        } else if (field == "gpa") {
            double v = stod(value);
            if (s.gpa == v) {
                result.push_back(s);
            }
        }
    }
    return result;
}

size_t ArrayCollection::count() const {
    return students.size();
}

double ArrayCollection::sum(const string& field) const {
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

double ArrayCollection::average(const string& field) const {
    size_t n = students.size();
    if (n == 0) {
        return 0.0;
    }
    double s = sum(field);
    return s / static_cast<double>(n);
}

bool ArrayCollection::importFromCsv(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) return false;

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        if (p1 == string::npos || p2 == string::npos) continue;

        string idStr  = line.substr(0, p1);
        string name   = line.substr(p1 + 1, p2 - p1 - 1);
        string gpaStr = line.substr(p2 + 1);

        Student s;
        s.id = stoi(idStr);
        s.name = name;
        s.gpa = stod(gpaStr);

        insertOne(s);
    }
    return true;
}
