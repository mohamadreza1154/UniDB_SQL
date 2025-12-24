#ifndef COMMAND_H
#define COMMAND_H
using namespace std;
#include <string>
#include "../models/studente.h"

enum class CommandType {
    InsertOne,
    DeleteOne,
    FindById,
    FindAll,
    Filter,
    Count,
    Sum,
    Average,
    ImportCsv,
    BeginTransaction,
    Commit,
    Rollback,
    BatchStart,
    BatchExecute,
    Invalid
};

struct Command {
    CommandType type = CommandType::Invalid;
    string collectionName;
    int id = -1;
    string field;
    string value;
    Student student{};
    string filename;
};

#endif
