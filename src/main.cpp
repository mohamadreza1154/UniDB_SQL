#include <iostream>
#include <string>

#include "models/studente.h"
#include "storage/ArrayCollection.h"
#include "storage/LinkedListCollection.h"
#include "engine/ExecutionEngine.h"
#include "parser/QueryParser.h"
using namespace std;
int main() {
    ArrayCollection arrayStudents;
    LinkedListCollection listStudents;

    ExecutionEngine engine;
    engine.registerCollection("students", &arrayStudents);
    engine.registerCollection("students_list", &listStudents);

    QueryParser parser;

    cout << "UniDB shell. Type 'exit' to quit.\n";

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) {
            break;
        }
        if (line == "exit" || line == "quit") {
            break;
        }
        if (line.empty()) {
            continue;
        }

        Command cmd = parser.parse(line);
        if (cmd.type == CommandType::Invalid) {
            cout << "Invalid command.\n";
            continue;
        }

        if (cmd.collectionName.empty()) {
            cmd.collectionName = "students";
        }

        ExecutionResult res = engine.execute(cmd);

        if (!res.ok) {
            cout << "Error: " << res.message << "\n";
            continue;
        }

        if (res.hasStudent) {
            cout << "id=" << res.student.id
                      << " name=" << res.student.name
                      << " gpa=" << res.student.gpa << "\n";
        } else if (res.hasStudents) {
            for (const auto& s : res.students) {
                cout << "id=" << s.id
                          << " name=" << s.name
                          << " gpa=" << s.gpa << "\n";
            }
        } else if (res.hasNumber) {
            cout << res.number << "\n";
        } else {
            cout << res.message << "\n";
        }
    }

    return 0;
}
