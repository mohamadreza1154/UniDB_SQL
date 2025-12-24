#ifndef ARRAY_COLLECTION_H
#define ARRAY_COLLECTION_H

#include <vector>
#include <string>
#include "Collection.h"

using namespace std;

class ArrayCollection : public Collection {
public:
    ArrayCollection() = default;
    ~ArrayCollection() override = default;

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
    vector<Student> students;
};

#endif // ARRAY_COLLECTION_H
