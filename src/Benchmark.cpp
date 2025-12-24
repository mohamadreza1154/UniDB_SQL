#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include "storage/ArrayCollection.h"
#include "storage/LinkedListCollection.h"
#include "models/studente.h"

using namespace std;
using namespace std::chrono;


long long now_ms() {
    return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

int main() {
    ArrayCollection arrayCol;
    LinkedListCollection listCol;
    const int N = 50000;
    const int M = 500;


    for (int i = 0; i < N; ++i) {
        Student s = {i + 1, "Name", 15.0};
        arrayCol.insertOne(s);
        listCol.insertOne(s);
    }


    auto start = high_resolution_clock::now();
    for (int i = 1; i <= M; ++i) arrayCol.deleteOneById(i);
    auto end = high_resolution_clock::now();
    cout << "Array delete from Start: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    start = high_resolution_clock::now();
    for (int i = 1; i <= M; ++i) listCol.deleteOneById(i);
    end = high_resolution_clock::now();
    cout << "List delete from Start: " << duration_cast<milliseconds>(end - start).count() << " ms\n";


    start = high_resolution_clock::now();
    for (int i = N; i > N - M; --i) arrayCol.deleteOneById(i);
    end = high_resolution_clock::now();
    cout << "Array delete from End: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    start = high_resolution_clock::now();
    for (int i = N; i > N - M; --i) listCol.deleteOneById(i);
    end = high_resolution_clock::now();
    cout << "List delete from End: " << duration_cast<milliseconds>(end - start).count() << " ms\n";


    mt19937 gen(42);
    uniform_int_distribution<int> dist(M + 1, N - M);

    start = high_resolution_clock::now();
    for (int i = 0; i < M; ++i) arrayCol.findById(dist(gen));
    end = high_resolution_clock::now();
    cout << "Array Random Search: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    start = high_resolution_clock::now();
    for (int i = 0; i < M; ++i) listCol.findById(dist(gen));
    end = high_resolution_clock::now();
    cout << "List Random Search: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    return 0;
}
