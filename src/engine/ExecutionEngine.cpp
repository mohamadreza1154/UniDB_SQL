#include "ExecutionEngine.h"
using namespace std ;
Collection* ExecutionEngine::getCollectionOrNull(const string& name) const {
    auto it = collections.find(name);
    if (it == collections.end()) return nullptr;
    return it->second;
}

void ExecutionEngine::registerCollection(const string& name, Collection* collection) {
    collections[name] = collection;
}

ExecutionResult ExecutionEngine::execute(const Command& command) {
    if (command.type == CommandType::BeginTransaction ||
        command.type == CommandType::Commit ||
        command.type == CommandType::Rollback ||
        command.type == CommandType::BatchStart ||
        command.type == CommandType::BatchExecute) {
        return executeEngineCommand(command);
    }

    if (batchMode) {
        batchQueue.push(command);
        ExecutionResult r;
        r.ok = true;
        r.message = "Queued in batch.";
        return r;
    }

    Collection* col = getCollectionOrNull(command.collectionName);
    if (col == nullptr) {
        ExecutionResult r;
        r.ok = false;
        r.message = "Collection not found: " + command.collectionName;
        return r;
    }

    return executeCollectionCommand(command, *col);
}

ExecutionResult ExecutionEngine::executeEngineCommand(const Command& command) {
    ExecutionResult r;

    if (command.type == CommandType::BatchStart) {
        batchMode = true;
        while (!batchQueue.empty()) batchQueue.pop();
        r.ok = true;
        r.message = "Batch mode started.";
        return r;
    }

    if (command.type == CommandType::BatchExecute) {
        batchMode = false;

        while (!batchQueue.empty()) {
            Command c = batchQueue.front();
            batchQueue.pop();

            Collection* col = getCollectionOrNull(c.collectionName);
            if (col == nullptr) {
                r.ok = false;
                r.message = "Collection not found in batch: " + c.collectionName;
                return r;
            }

            ExecutionResult res = executeCollectionCommand(c, *col);
            if (!res.ok) {
                return res;
            }
        }

        r.ok = true;
        r.message = "Batch executed.";
        return r;
    }

    if (command.type == CommandType::BeginTransaction) {
        transactionStack.beginTransaction();
        r.ok = true;
        r.message = "Transaction started.";
        return r;
    }

    if (command.type == CommandType::Commit) {
        transactionStack.commit();
        r.ok = true;
        r.message = "Committed.";
        return r;
    }

    if (command.type == CommandType::Rollback) {
        if (!transactionStack.inTransaction()) {
            r.ok = false;
            r.message = "No active transaction.";
            return r;
        }

        vector<Command> undo = transactionStack.popAllUndo();
        for (auto it = undo.rbegin(); it != undo.rend(); ++it) {
            const Command& undoCmd = *it;
            Collection* col = getCollectionOrNull(undoCmd.collectionName);
            if (col == nullptr) {
                r.ok = false;
                r.message = "Collection not found in rollback: " + undoCmd.collectionName;
                return r;
            }

            ExecutionResult res = executeCollectionCommand(undoCmd, *col);
            if (!res.ok) {
                return res;
            }
        }

        r.ok = true;
        r.message = "Rolled back.";
        return r;
    }

    r.ok = false;
    r.message = "Unknown engine command.";
    return r;
}

void ExecutionEngine::pushUndoIfNeeded(const Command& executed, Collection& collection) {
    if (!transactionStack.inTransaction()) return;

    if (executed.type == CommandType::InsertOne) {
        Command undo;
        undo.type = CommandType::DeleteOne;
        undo.collectionName = executed.collectionName;
        undo.id = executed.student.id;
        transactionStack.pushUndo(undo);
        return;
    }

    if (executed.type == CommandType::DeleteOne) {
        Student* before = collection.findById(executed.id);
        if (before == nullptr) {
            return;
        }
        Student snapshot = *before;

        Command undo;
        undo.type = CommandType::InsertOne;
        undo.collectionName = executed.collectionName;
        undo.student = snapshot;
        transactionStack.pushUndo(undo);
        return;
    }
}

ExecutionResult ExecutionEngine::executeCollectionCommand(const Command& command, Collection& collection) {
    ExecutionResult result;

    switch (command.type) {
    case CommandType::InsertOne: {
        collection.insertOne(command.student);
        pushUndoIfNeeded(command, collection);
        result.ok = true;
        result.message = "Inserted.";
        return result;
    }

    case CommandType::DeleteOne: {
        if (transactionStack.inTransaction()) {
            Student* before = collection.findById(command.id);
            if (before != nullptr) {
                Command undo;
                undo.type = CommandType::InsertOne;
                undo.collectionName = command.collectionName;
                undo.student = *before;
                transactionStack.pushUndo(undo);
            }
        }

        bool deleted = collection.deleteOneById(command.id);
        result.ok = deleted;
        result.message = deleted ? "Deleted." : "Not found.";
        return result;
    }

    case CommandType::FindById: {
        Student* s = collection.findById(command.id);
        if (s == nullptr) {
            result.ok = false;
            result.message = "Not found.";
            return result;
        }
        result.ok = true;
        result.hasStudent = true;
        result.student = *s;
        result.message = "Found.";
        return result;
    }

    case CommandType::FindAll: {
        result.ok = true;
        result.hasStudents = true;
        result.students = collection.findAll();
        result.message = "OK.";
        return result;
    }

    case CommandType::Filter: {
        result.ok = true;
        result.hasStudents = true;
        result.students = collection.filter(command.field, command.value);
        result.message = "OK.";
        return result;
    }

    case CommandType::Count: {
        result.ok = true;
        result.hasNumber = true;
        result.number = static_cast<double>(collection.count());
        result.message = "OK.";
        return result;
    }

    case CommandType::Sum: {
        result.ok = true;
        result.hasNumber = true;
        result.number = collection.sum(command.field);
        result.message = "OK.";
        return result;
    }

    case CommandType::Average: {
        result.ok = true;
        result.hasNumber = true;
        result.number = collection.average(command.field);
        result.message = "OK.";
        return result;
    }
    case CommandType::ImportCsv: {
        bool ok = collection.importFromCsv(command.filename);
        result.ok = ok;
        result.message = ok ? "Imported." : "Import failed.";
        return result;
    }

    default:
        result.ok = false;
        result.message = "Unsupported command type for collection.";
        return result;
    }
}
