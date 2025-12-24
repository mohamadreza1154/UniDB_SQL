#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "storage/ArrayCollection.h"
#include "storage/LinkedListCollection.h"
#include "models/studente.h"

using namespace std;
using namespace std::chrono;

Student makeRandomStudent(int id) {
    static const string names[] = {"Ali", "Reza", "Sara", "Neda", "Omid", "Hassan", "Mina", "Sina"};
    static mt19937 gen(1729);
    uniform_int_distribution<int> nameDist(0, 7);
    uniform_real_distribution<double> gpaDist(10.0, 20.0);

    Student s;
    s.id = id;
    s.name = names[nameDist(gen)];
    s.gpa = gpaDist(gen);
    return s;
}

int main() {
    ArrayCollection arrayCol;
    LinkedListCollection listCol;

    const int N = 50000;

    auto start1 = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        Student s = makeRandomStudent(i + 1);
        arrayCol.insertOne(s);
    }
    auto end1 = high_resolution_clock::now();
    auto arrayInsertTime = duration_cast<milliseconds>(end1 - start1).count();

    auto start2 = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        Student s = makeRandomStudent(i + 1);
        listCol.insertOne(s);
    }
    auto end2 = high_resolution_clock::now();
    auto listInsertTime = duration_cast<milliseconds>(end2 - start2).count();

    cout << "ArrayCollection insert 50000: " << arrayInsertTime << " ms\n";
    cout << "LinkedListCollection insert 50000: " << listInsertTime << " ms\n";

    return 0;
}