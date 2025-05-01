#include <iostream>
#include <string>
#include <cctype>
#include <unordered_map>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <sstream>

using std::string;
using std::vector;
using std::cerr;
using std::cout;
using std::endl;

class SimpleLang {
    size_t pos = 0;
    string code;
    std::unordered_map<string, int> constants;
    std::unordered_map<string, int> variables;
    int recursion_depth = 0;  // 防止递归过深
    const int MAX_RECURSION = 10;

    // 辅助函数声明
    void skip_whitespace();
    bool match(const string& word);
    string parse_identifier();
    int parse_expression();
    int parse_term();
    int parse_factor();
    int parse_number();
    vector<string> read_lines_from_file(const string& filename);

public:
    void run(const string& input) {
        try {
            if (recursion_depth > MAX_RECURSION) {
                throw std::runtime_error("递归深度超过限制");
            }
            recursion_depth++;
            
            code = input;
            pos = 0;
            
            if (match("print")) {
                handle_print();
            }
            else if (match("var")) {
                handle_var();
            }
            else if (match("const")) {
                handle_const();
            }
            else if (match("run")) {
                handle_run();
            }
            else {
                throw std::runtime_error("未知命令");
            }
            
            recursion_depth--;
        } catch (const std::exception& e) {
            cerr << "错误: " << e.what() << endl;
            recursion_depth--;
        }
    }

private:
    void handle_print() {
        skip_whitespace();
        int result = parse_expression();
        cout << result << endl;
    }

    void handle_var() {
        string name = parse_identifier();
        skip_whitespace();
        if (!match("=")) throw std::runtime_error("var 需要初始化值");
        int value = parse_expression();
        if (constants.count(name)) throw std::runtime_error(name + " 是常量");
        variables[name] = value;
    }

    void handle_const() {
        string name = parse_identifier();
        skip_whitespace();
        if (!match("=")) throw std::runtime_error("const 需要初始化值");
        int value = parse_expression();
        if (variables.count(name) || constants.count(name)) {
            throw std::runtime_error(name + " 已存在");
        }
        constants[name] = value;
    }

    void handle_run() {
        skip_whitespace();
        string filename = parse_string_literal();
        auto lines = read_lines_from_file(filename);
        
        for (const auto& line : lines) {
            if (!line.empty()) {
                run(line);  // 递归执行
            }
        }
    }

    string parse_string_literal() {
        skip_whitespace();
        if (code[pos] != '"') throw std::runtime_error("需要文件名");
        pos++;
        string filename;
        while (pos < code.size() && code[pos] != '"') {
            filename += code[pos++];
        }
        if (pos >= code.size()) throw std::runtime_error("缺少闭合引号");
        pos++;
        return filename;
    }
};

// 新增文件读取函数
std::vector<string> SimpleLang::read_lines_from_file(const string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }
    
    std::vector<string> lines;
    string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

// 辅助函数实现
void SimpleLang::skip_whitespace() {
    while (pos < code.size() && isspace(code[pos])) pos++;
}

bool SimpleLang::match(const string& word) {
    skip_whitespace();
    if (code.substr(pos, word.size()) == word) {
        pos += word.size();
        return true;
    }
    return false;
}

string SimpleLang::parse_identifier() {
    skip_whitespace();
    if (pos >= code.size() || !isalpha(code[pos])) {
        throw std::runtime_error("无效标识符");
    }
    string ident;
    while (pos < code.size() && (isalnum(code[pos]) || code[pos] == '_')) {
        ident += code[pos++];
    }
    return ident;
}

int SimpleLang::parse_expression() {
    int left = parse_term();
    while (true) {
        if (match("+")) {
            left += parse_term();
        } else if (match("-")) {
            left -= parse_term();
        } else {
            break;
        }
    }
    return left;
}

int SimpleLang::parse_term() {
    int left = parse_factor();
    while (true) {
        if (match("*")) {
            left *= parse_factor();
        } else if (match("/")) {
            int divisor = parse_factor();
            if (divisor == 0) throw std::runtime_error("除以零");
            left /= divisor;
        } else {
            break;
        }
    }
    return left;
}

int SimpleLang::parse_factor() {
    skip_whitespace();
    if (isalpha(code[pos])) {
        string name = parse_identifier();
        if (constants.count(name)) return constants[name];
        if (variables.count(name)) return variables[name];
        throw std::runtime_error("未定义的标识符: " + name);
    }
    if (isdigit(code[pos])) {
        return parse_number();
    }
    if (code[pos] == '(') {
        pos++;
        int val = parse_expression();
        if (!match(")")) throw std::runtime_error("缺少右括号");
        return val;
    }
    throw std::runtime_error("无效表达式");
}

int SimpleLang::parse_number() {
    int num = 0;
    while (pos < code.size() && isdigit(code[pos])) {
        num = num * 10 + (code[pos] - '0');
        pos++;
    }
    return num;
}

SimpleLang lang;

void runFile(string path) {
    lang.run(R"(run "test.mylang")");
}
