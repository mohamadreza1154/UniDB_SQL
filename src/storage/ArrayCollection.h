#ifndef LINKED_LIST_COLLECTION_H
#define LINKED_LIST_COLLECTION_H
#include <vector>
#include <string>
#include "Collection.h"
using namespace std;
class LinkedListCollection : public Collection {
public:
    LinkedListCollection();
    ~LinkedListCollection() override;

    void insertOne(const Student& student) override;
    bool deleteOneById(int id) override;
    Student* findById(int id) override;
    vector<Student> findAll() const override;
    vector<Student> filter(const string& field,
                                const string& value) const override;
    size_t count() const override;
    double sum(const string& field) const override;
    double average(const string& field) const override;

    bool importFromCsv(const string& filename) override;

private:
    struct Node {
        Student data;
        Node* prev;
        Node* next;
        explicit Node(const Student& s) : data(s), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t size;

    Node* findNodeById(int id) const;
    void clear();
};

#endif
