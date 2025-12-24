#ifndef EXECUTION_ENGINE_H
#define EXECUTION_ENGINE_H
using namespace std;
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include "Command.h"
#include "TransactionStack.h"
#include "../storage/Collection.h"

struct ExecutionResult {
    bool ok = true;
    string message;

    bool hasStudent = false;
    Student student{};

    bool hasStudents = false;
    vector<Student> students{};

    bool hasNumber = false;
    double number = 0.0;
};

class ExecutionEngine {
public:
    ExecutionEngine() = default;

    void registerCollection(const string& name, Collection* collection);
    ExecutionResult execute(const Command& command);

private:
    unordered_map<string, Collection*> collections;
    bool batchMode = false;
    queue<Command> batchQueue;
    TransactionStack transactionStack;

    Collection* getCollectionOrNull(const string& name) const;
    ExecutionResult executeCollectionCommand(const Command& command, Collection& collection);
    ExecutionResult executeEngineCommand(const Command& command);
    void pushUndoIfNeeded(const Command& executed, Collection& collection);
};

#endif
