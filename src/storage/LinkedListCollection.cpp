#include "LinkedListCollection.h"
#include <fstream>
#include <sstream>

LinkedListCollection::LinkedListCollection()
    : head(nullptr), tail(nullptr), size(0) {
}

LinkedListCollection::~LinkedListCollection() {
    clear();
}

void LinkedListCollection::clear() {
    Node* cur = head;
    while (cur) {
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

LinkedListCollection::Node* LinkedListCollection::findNodeById(int id) const {
    Node* cur = head;
    while (cur) {
        if (cur->data.id == id) {
            return cur;
        }
        cur = cur->next;
    }
    return nullptr;
}

void LinkedListCollection::insertOne(const Student& student) {
    Node* node = new Node(student);
    if (!head) {
        head = tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    ++size;
}

bool LinkedListCollection::deleteOneById(int id) {
    Node* node = findNodeById(id);
    if (!node) return false;

    if (node->prev) node->prev->next = node->next;
    else head = node->next;

    if (node->next) node->next->prev = node->prev;
    else tail = node->prev;

    delete node;
    --size;
    return true;
}

Student* LinkedListCollection::findById(int id) {
    Node* node = findNodeById(id);
    if (!node) return nullptr;
    return &node->data;
}

std::vector<Student> LinkedListCollection::findAll() const {
    std::vector<Student> result;
    result.reserve(size);
    Node* cur = head;
    while (cur) {
        result.push_back(cur->data);
        cur = cur->next;
    }
    return result;
}

std::vector<Student> LinkedListCollection::filter(const std::string& field,
                                                  const std::string& value) const {
    std::vector<Student> result;
    Node* cur = head;
    while (cur) {
        const Student& s = cur->data;
        if (field == "id") {
            int v = std::stoi(value);
            if (s.id == v) result.push_back(s);
        } else if (field == "name") {
            if (s.name == value) result.push_back(s);
        } else if (field == "gpa") {
            double v = std::stod(value);
            if (s.gpa == v) result.push_back(s);
        }
        cur = cur->next;
    }
    return result;
}

std::size_t LinkedListCollection::count() const {
    return size;
}

double LinkedListCollection::sum(const std::string& field) const {
    double total = 0.0;
    Node* cur = head;
    while (cur) {
        const Student& s = cur->data;
        if (field == "gpa") {
            total += s.gpa;
        } else if (field == "id") {
            total += static_cast<double>(s.id);
        }
        cur = cur->next;
    }
    return total;
}

double LinkedListCollection::average(const std::string& field) const {
    if (size == 0) return 0.0;
    double s = sum(field);
    return s / static_cast<double>(size);
}

bool LinkedListCollection::importFromCsv(const std::string& filename) {
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
