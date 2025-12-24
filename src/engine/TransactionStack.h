#ifndef TRANSACTION_STACK_H
#define TRANSACTION_STACK_H
using namespace std;
#include <vector>
#include "Command.h"

class TransactionStack {
public:
    void beginTransaction();
    bool inTransaction() const;

    void pushUndo(const Command& undoCommand);
    bool canRollback() const;

    vector<Command> popAllUndo();

    void commit();
    void rollbackClear();

private:
    bool active = false;
    vector<Command> undoLog;
};

#endif
