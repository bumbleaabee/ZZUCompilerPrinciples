//
// Created by lenovo on 25-11-13.
//
#include <bits/stdc++.h>
using namespace std;

// 关键字映射表：字符串到种别码
unordered_map<string, int> keywords = {
    {"main", 1}, {"if", 2}, {"then", 3}, {"while", 4}, {"do", 5},
    {"static", 6}, {"int", 7}, {"double", 8}, {"struct", 9}, {"else", 10},
    {"long", 11},{"switch", 12}, {"case", 13},{"typedef", 14}, {"char", 15},
    {"return", 16}, {"const", 17}, {"float", 18}, {"break", 19}, {"short", 20},
    {"sizeof", 21}, {"for", 22}, {"void", 23}, {"continue", 24}
};

// 运算符和分隔符映射表：字符串到种别码
unordered_map<string, int> operators = {
    {"<=", 26}, {">", 27}, {">=", 28}, {"=", 29},
    {"[", 30}, {"]", 31}, {";", 32}, {"(", 33}, {")", 34},
    {"+", 35}, {"-", 36}, {"*", 37}, {"/", 38}, {"**", 39},
    {"==", 40}, {"<", 41}, {"<>", 42}, {",", 43}, {":", 44},
    {"{", 45}, {"}", 46}, {"!=", 47}
};

// 自动机状态枚举，初始，关键字，数字，<，>，!，error
enum State { S0, S1, S2, S3, S4, S5, S_ERROR };

// 判断字符类型
bool isLetter(char c) { return isalpha(c); }
bool isDigit(char c) { return isdigit(c); }
bool isWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n'; }

// 词法分析
vector<pair<int, string>> lexer(const vector<string>& lines) {
    vector<pair<int, string>> tokens;
    int line_num = 1;

    for (const auto& line : lines) {
        int i = 0, n = line.size();
        State state = S0;
        string token = "";

        while (i < n) {
            char ch = line[i];

            switch (state) {
                case S0: // 初始状态
                    if (isWhitespace(ch)) {
                        i++;
                        break;
                    }
                    token = "";
                    if (isLetter(ch)) {
                        state = S1;
                        token += ch;
                        i++;  // 消耗字符
                    }
                    else if (isDigit(ch)) {
                        state = S2;
                        token += ch;
                        i++;  // 消耗字符
                    }
                    else if (ch == '<') {
                        state = S3;
                        token += ch;
                        i++;
                    }
                    else if (ch == '>') {
                        state = S4;
                        token += ch;
                        i++;
                    }
                    else if (ch == '!') {
                        state = S5;
                        token += ch;
                        i++;
                    }
                    else {
                        // 单字符运算符
                        string op(1, ch);
                        if (operators.count(op)) {
                            tokens.push_back({operators[op], op});
                        } else {
                            // 处理未知字符
                            cerr << "Error: Unknown character '" << ch << "' at line " << line_num << endl;
                        }
                        i++;
                    }
                    break;

                case S1: // 识别标识符/关键字
                    if (isLetter(ch) || isDigit(ch)) {
                        token += ch;
                        i++;
                    } else {
                        // 识别完成，判断是关键字还是标识符
                        if (keywords.count(token))
                            tokens.push_back({keywords[token], token});
                        else
                            tokens.push_back({48, token}); // ID种别码48
                        state = S0;
                        // 注意：这里不消耗字符，让S0重新处理当前字符
                    }
                    break;

                case S2: // 识别数字
                    if (isDigit(ch)) {
                        token += ch;
                        i++;
                    } else {
                        tokens.push_back({49, token}); // NUM种别码49
                        state = S0;
                        // 注意：这里不消耗字符，让S0重新处理当前字符
                    }
                    break;

                case S3: // 处理 < 或 <=
                    if (ch == '=') {
                        tokens.push_back({26, "<="});
                        i++;
                    } else {
                        tokens.push_back({41, "<"});
                        // 不消耗字符，让S0处理当前字符
                    }
                    state = S0;
                    break;

                case S4: // 处理 > 或 >=
                    if (ch == '=') {
                        tokens.push_back({28, ">="});
                        i++;
                    } else {
                        tokens.push_back({27, ">"});
                        // 不消耗字符，让S0处理当前字符
                    }
                    state = S0;
                    break;

                case S5: // 处理 !=
                    if (ch == '=') {
                        tokens.push_back({47, "!="});
                        i++;
                    } else {
                        // 错误处理：!后面必须跟=
                        cerr << "Error: '!' must be followed by '=' at line " << line_num << endl;
                        // 不消耗字符，让S0处理当前字符
                    }
                    state = S0;
                    break;
            }
        }

        // 处理行末未完成的token
        if (state == S1 && !token.empty()) {
            if (keywords.count(token))
                tokens.push_back({keywords[token], token});
            else
                tokens.push_back({48, token});
        } else if (state == S2 && !token.empty()) {
            tokens.push_back({49, token});
        }

        line_num++;
    }
    return tokens;
}

// 读取源文件：逐行读取到vector
vector<string> readin(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file " << filename << endl;
        exit(1);
    }

    vector<string> ret;
    string line;
    while (getline(file, line)) {
        ret.push_back(line);
    }
    file.close();
    return ret;
}

int main(int argc, char *argv[]) {
    // if (argc != 2) {
    //     cerr << "Usage: " << argv[0] << " <filename>" << endl;
    //     return 1;
    // }

    string filename;
    cin >> filename;
    vector<string> lines = readin(filename);
    vector<pair<int, string>> tokens = lexer(lines);

    // 输出token序列
    for (auto& token : tokens) {
        // cout << "<" << token.first << ", \"" << token.second << "\">" << endl;
        cout << "<" << token.first << ", \"";
        if (token.first == 48) {
            cout << "ID" << "\", " << token.second << ">" << " " << endl;
        }else if (token.first == 49) {
            cout << "NUM" << "\", " << token.second << ">" << " " << endl;
        }else {
            cout << token.second << "\", " << "-" << ">" << endl;
        }
    }

    return 0;
}