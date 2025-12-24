#ifndef COLLECTION_H
#define COLLECTION_H
using namespace std;
#include <string>
#include <vector>
#include "../models/studente.h"

class Collection {
public:
    virtual ~Collection() = default;

    virtual void insertOne(const Student& student) = 0;
    virtual bool deleteOneById(int id) = 0;
    virtual Student* findById(int id) = 0;
    virtual vector<Student> findAll() const = 0;
    virtual vector<Student> filter(const string& field,
                                        const string& value) const = 0;
    virtual size_t count() const = 0;
    virtual double sum(const string& field) const = 0;
    virtual double average(const string& field) const = 0;

    virtual bool importFromCsv(const string& filename) = 0;
};

#endif
