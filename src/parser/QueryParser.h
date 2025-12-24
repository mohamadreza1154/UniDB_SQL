#ifndef QUERY_PARSER_H
#define QUERY_PARSER_H
#include <string>
#include <cctype>
#include <optional>
#include "../engine/Command.h"
using namespace std;
class QueryParser {
public:
    Command parse(const string& rawQuery) const {
         string q = trim(rawQuery);


        if (!q.empty() && q.back() == ';') {
            q.pop_back();
            q = trim(q);
        }


        if (startsWith(q, "db.beginTransaction()")) {
            Command c;
            c.type = CommandType::BeginTransaction;
            return c;
        }
        if (startsWith(q, "db.commit()")) {
            Command c;
            c.type = CommandType::Commit;
            return c;
        }
        if (startsWith(q, "db.rollback()")) {
            Command c;
            c.type = CommandType::Rollback;
            return c;
        }
        if (startsWith(q, "db.batch.start()")) {
            Command c;
            c.type = CommandType::BatchStart;
            return c;
        }
        if (startsWith(q, "db.batch.execute()")) {
            Command c;
            c.type = CommandType::BatchExecute;
            return c;
        }

        if (!startsWith(q, "db.")) {
            return invalid("Query must start with db.");
        }


        size_t afterDb = 3;
        size_t dot1 = q.find('.', afterDb);
        if (dot1 == string::npos) return invalid("Invalid format (collection).");

        string collection = q.substr(afterDb, dot1 - afterDb);


        size_t paren = q.find('(', dot1 + 1);
        if (paren == string::npos) return invalid("Invalid format (method).");

        string method = q.substr(dot1 + 1, paren - (dot1 + 1));
        string args = extractInsideParentheses(q, paren);

        Command cmd;
        cmd.collectionName = collection;

        if (method == "insertOne") {
            cmd.type = CommandType::InsertOne;
            cmd.student = parseStudentObject(args);
            cmd.id = cmd.student.id;
            return cmd;
        }

        if (method == "deleteOne") {

            cmd.type = CommandType::DeleteOne;
            cmd.id = parseIntField(args, "_id", "id");
            return cmd;
        }

        if (method == "findByID" || method == "findById") {

            cmd.type = CommandType::FindById;
            cmd.id = parseFirstIntArg(args);
            return cmd;
        }

        if (method == "findAll") {
            cmd.type = CommandType::FindAll;
            return cmd;
        }

        if (method == "filter") {

            cmd.type = CommandType::Filter;
            parseTwoStringArgs(args, cmd.field, cmd.value);
            return cmd;
        }

        if (method == "count") {
            cmd.type = CommandType::Count;
            return cmd;
        }

        if (method == "sum") {

            cmd.type = CommandType::Sum;
            cmd.field = parseOneStringArg(args);
            return cmd;
        }

        if (method == "average") {

            cmd.type = CommandType::Average;
            cmd.field = parseOneStringArg(args);
            return cmd;
        }

        if (method == "import") {

            cmd.type = CommandType::ImportCsv;
            cmd.filename = parseOneStringArg(args);
            return cmd;
        }

        return invalid("Unknown method: " + method);
    }

private:
    static Command invalid(const string& msg) {
        Command c;
        c.type = CommandType::Invalid;
        c.value = msg;
        return c;
    }

    static bool startsWith(const string& s, const string& pref) {
        return s.rfind(pref, 0) == 0;
    }

    static string trim(const string& s) {
        size_t b = 0;
        while (b < s.size() && isspace(static_cast<unsigned char>(s[b]))) ++b;

        size_t e = s.size();
        while (e > b && isspace(static_cast<unsigned char>(s[e - 1]))) --e;

        return s.substr(b, e - b);
    }

    static string extractInsideParentheses(const string& q, size_t openParenPos) {
        size_t closeParenPos = q.rfind(')');
        if (closeParenPos == string::npos || closeParenPos <= openParenPos) return "";
        return trim(q.substr(openParenPos + 1, closeParenPos - openParenPos - 1));
    }

    static int parseFirstIntArg(const string& args) {
        string a = trim(args);
        if (a.empty()) return -1;

        size_t comma = a.find(',');
        string first = (comma == string::npos) ? a : a.substr(0, comma);
        first = trim(first);

        return stoi(first);
    }

    static string stripQuotes(const string& s) {
        string t = trim(s);
        if (t.size() >= 2 && ((t.front() == '"' && t.back() == '"') || (t.front() == '\'' && t.back() == '\''))) {
            return t.substr(1, t.size() - 2);
        }
        return t;
    }

    static string parseOneStringArg(const string& args) {
        return stripQuotes(args);
    }

    static void parseTwoStringArgs(const string& args, string& out1, string& out2) {
        string a = args;
        size_t comma = a.find(',');
        if (comma == string::npos) {
            out1.clear();
            out2.clear();
            return;
        }
        out1 = stripQuotes(a.substr(0, comma));
        out2 = stripQuotes(a.substr(comma + 1));
    }

    static int parseIntField(const string& obj, const string& key1, const string& key2) {

        auto v1 = tryParseIntByKey(obj, key1);
        if (v1.has_value()) return *v1;

        auto v2 = tryParseIntByKey(obj, key2);
        if (v2.has_value()) return *v2;

        return -1;
    }

    static Student parseStudentObject(const string& obj) {
        Student s;
        s.id = parseIntField(obj, "_id", "id");

        auto name = tryParseStringByKey(obj, "name");
        if (name.has_value()) s.name = *name;

        auto gpa = tryParseDoubleByKey(obj, "gpa");
        if (gpa.has_value()) s.gpa = *gpa;

        return s;
    }

    static optional<int> tryParseIntByKey(const string& obj, const string& key) {
        size_t k = obj.find(key);
        if (k == string::npos) return nullopt;

        size_t colon = obj.find(':', k);
        if (colon == string::npos) return nullopt;

        string v = trim(obj.substr(colon + 1));
        size_t comma = v.find(',');
        if (comma != string::npos) v = v.substr(0, comma);

        v = trim(v);
        if (!v.empty() && v.back() == '}') v.pop_back();
        v = trim(v);

        try { return stoi(v); } catch (...) { return nullopt; }
    }

    static optional<double> tryParseDoubleByKey(const string& obj, const string& key) {
        size_t k = obj.find(key);
        if (k == string::npos) return nullopt;

        size_t colon = obj.find(':', k);
        if (colon == string::npos) return nullopt;

        string v = trim(obj.substr(colon + 1));
        size_t comma = v.find(',');
        if (comma != string::npos) v = v.substr(0, comma);

        v = trim(v);
        if (!v.empty() && v.back() == '}') v.pop_back();
        v = trim(v);

        try { return stod(v); } catch (...) { return nullopt; }
    }

    static optional<string> tryParseStringByKey(const string& obj, const string& key) {
        size_t k = obj.find(key);
        if (k == string::npos) return nullopt;

        size_t colon = obj.find(':', k);
        if (colon == string::npos) return nullopt;

        string v = trim(obj.substr(colon + 1));
        size_t comma = v.find(',');
        if (comma != string::npos) v = v.substr(0, comma);

        v = trim(v);
        if (!v.empty() && v.back() == '}') v.pop_back();
        v = trim(v);

        return stripQuotes(v);
    }
};

#endif
