#pragma once 
#include <string> 
#include <unordered_map> 

#include <vector> 
#include <stdexcept> 
#include <cctype> 

#include <sstream> 
using namespace std; 
#include <cmath> 

namespace MiniJson{
    struct Value
    {
        enum Type{
            OBJECT, 
            ARRAY, 
            STRING, 
            NUMBER, 
            BOOL, 
            NIL
        } type = NIL; 

        unordered_map<string, Value> object; 
        vector<Value> array; 
        string str; 
        double number = 0.0; 
        bool boolean = false; 

        // typechecks;; 

        bool is_object() const {
            return type == OBJECT; 
        }

        bool is_array() const 
        {
            return type == ARRAY; 
        }

        bool is_string() const 
        {
            return type == STRING;
        }

        bool is_number() const{
            return type == NUMBER; 
        }

        bool is_bool() const{
            return type == BOOL ; 
        }

        bool is_null() const {
            return type == NULL; 
        }

        // Accessor 
        const unordered_map<string, Value> &as_object() const 
        {
            if (!is_object()) 
            throw runtime_error("Not an object"); 
            return object; 
        }

        const vector<Value>& as_array() const {
        if (!is_array()) throw runtime_error("Not an array");
        return array;
    }

    string as_string() const 
    {
        if (!is_string()) throw runtime_error("Not a string");
        return str;
    }

    int as_int() const 
    {
        if (!is_number()) throw runtime_error("Not a number");
        return (int)number;
    }

    double as_float() const 
    {
        if (!is_number()) throw runtime_error("Not a number");
        return number;
    }

    bool as_bool() const 
    {
        if (!is_bool()) throw runtime_error("Not a bool");
        return boolean;
    }

    bool contains(const string &k) const 
    {
        return is_object() && object.count(k);
    }

    const Value& operator[](const string &k) const 
    {
        auto it = object.find(k);
        if (it == object.end())
            throw runtime_error("Key not found: " + k);
        return it->second;
    }

    const Value& operator[](size_t idx) const 
    {
        if (!is_array()) throw runtime_error("Not an array");
        return array[idx];
    }
    }; 

    // ----------------------------------------------- // 
    // =========== Parser Implementation========= // 
    class Parser
    {
        public: 
        Parser(istream &in)
        {
            stringstream buffer; 
            buffer << in.rdbuf(); 
            text = buffer.str(); 
        }

        Value parse()
        {
            skip_ws();
            Value v = parse_value();
            skip_ws(); 
            return v; 
        }

        private: 
        string text; 
        size_t pos =0; 

        // Helpers -------------------------------
        void skip_ws()
        {
            while(pos < text.size() and isspace(text[pos]))
            pos++; 
        }

        char peek() const
        {
            if (pos >= text.size()) return '\0'; 
            return text[pos] ; 
        }

        char get(){
            if (pos >= text.size()) return '\0'; 
            return text[pos++]; 
        } 

        // Parse values ------------------------- // 
        Value parse_value()
        {
            skip_ws(); 

            char c = peek();
            
        if (c == '{') return parse_object();
        if (c == '[') return parse_array();
        if (c == '"') return parse_string();
        if (c == '-' || isdigit(c)) return parse_number();
        if (match("true"))  return make_bool(true);
        if (match("false")) return make_bool(false);
        if (match("null"))  return make_null();

        throw runtime_error("Invalid JSON value at pos " + to_string(pos));
        }

        // Parsing primitives -------------------- // 
        bool match(const string &kw) {
        if (text.compare(pos, kw.size(), kw) == 0) {
            pos += kw.size();
            return true;
        }
        return false;
    }

    Value make_bool(bool b) {
        Value v; v.type = Value::BOOL; v.boolean = b; return v;
    }

    Value make_null() {
        Value v; v.type = Value::NIL; return v;
    }

    // Object --------------------------------------------
    Value parse_object() {
        Value obj;
        obj.type = Value::OBJECT;

        get(); // {
        skip_ws();

        if (peek() == '}') {
            get();
            return obj;
        }

        while (true) {
            skip_ws();
            string key = parse_string().as_string();
            skip_ws();

            if (get() != ':')
                throw runtime_error("Expected ':' in object");

            skip_ws();
            obj.object[key] = parse_value();
            skip_ws();

            char c = get();
            if (c == '}') break;
            if (c != ',')
                throw runtime_error("Expected ',' or '}' in object");
        }

        return obj;
    }

    // Array ---------------------------------------------
    Value parse_array() {
        Value arr;
        arr.type = Value::ARRAY;

        get(); // [
        skip_ws();

        if (peek() == ']') {
            get();
            return arr;
        }

        while (true) {
            arr.array.push_back(parse_value());
            skip_ws();

            char c = get();
            if (c == ']') break;
            if (c != ',')
                throw runtime_error("Expected ',' or ']' in array");
        }

        return arr;
    }

    // String --------------------------------------------
    Value parse_string() {
        Value v;
        v.type = Value::STRING;

        if (get() != '"')
            throw runtime_error("Expected '\"' at beginning of string");

        string out;
        while (true) {
            char c = get();
            if (c == '"') break;

            if (c == '\\') {
                char esc = get();
                if (esc == 'n') out.push_back('\n');
                else if (esc == 't') out.push_back('\t');
                else if (esc == '"') out.push_back('"');
                else if (esc == '\\') out.push_back('\\');
                else out.push_back(esc); // basic escapes only
            } else {
                out.push_back(c);
            }
        }

        v.str = out;
        return v;
    }

    // Number --------------------------------------------
    Value parse_number() {
        Value v;
        v.type = Value::NUMBER;

        size_t start = pos;

        if (peek() == '-') get();

        while (isdigit(peek())) get();
        if (peek() == '.') {
            get();
            while (isdigit(peek())) get();
        }

        double num = stod(text.substr(start, pos - start));
        v.number = num;
        return v;
    }
};

/* ======================================================
   Public helper
   ====================================================== */

inline Value parse(istream &in) {
    Parser p(in);
    return p.parse();
}

inline Value parse_string(const string &s) {
    istringstream ss(s);
    return parse(ss);
}
 
}

