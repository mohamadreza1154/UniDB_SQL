#ifndef LINKED_LIST_COLLECTION_H
#define LINKED_LIST_COLLECTION_H

#include <vector>
#include <string>
#include "Collection.h"

class LinkedListCollection : public Collection {
public:
    LinkedListCollection();
    ~LinkedListCollection() override;

    void insertOne(const Student& student) override;
    bool deleteOneById(int id) override;
    Student* findById(int id) override;
    std::vector<Student> findAll() const override;
    std::vector<Student> filter(const std::string& field,
                                const std::string& value) const override;
    std::size_t count() const override;
    double sum(const std::string& field) const override;
    double average(const std::string& field) const override;

    bool importFromCsv(const std::string& filename) override;

private:
    struct Node {
        Student data;
        Node* prev;
        Node* next;
        explicit Node(const Student& s) : data(s), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    std::size_t size;

    Node* findNodeById(int id) const;
    void clear();
};

#endif
