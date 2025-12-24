#include "TransactionStack.h"
#include <vector>
using namespace std;
void TransactionStack::beginTransaction() {
    active = true;
    undoLog.clear();
}

bool TransactionStack::inTransaction() const {
    return active;
}

void TransactionStack::pushUndo(const Command& undoCommand) {
    if (!active) return;
    undoLog.push_back(undoCommand);
}

bool TransactionStack::canRollback() const {
    return active && !undoLog.empty();
}

vector<Command> TransactionStack::popAllUndo() {
    vector<Command> copy = undoLog;
    undoLog.clear();
    active = false;
    return copy;
}

void TransactionStack::commit() {
    undoLog.clear();
    active = false;
}

void TransactionStack::rollbackClear() {
    undoLog.clear();
    active = false;
}
