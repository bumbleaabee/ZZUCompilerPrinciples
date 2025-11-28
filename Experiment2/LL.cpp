//
// Created by lenovo on 25-11-21.
//
#include <bits/stdc++.h>

using namespace std;

// 产生式结构体
struct Production {
    int index;
    string left;
    vector<string> right;
};

// 文法类
class Grammar {
public:
    set<string> nonTerminals;
    set<string> terminals;
    vector<Production> productions;
    string startSymbol;

    map<string, set<string>> firstSet;
    map<string, set<string>> followSet;
    map<int, set<string>> selectSet;
    map<string, map<string, int>> parseTable;

    // 读取文法信息
    void readGrammar(const string& filename);
    void computeFirstSet();
    void computeFollowSet();
    void computeSelectSet();
    void buildParseTable();
    void printGrammar();
    void printFirstSet();
    void printFollowSet();
    void printSelectSet();
    void printParseTable();
    // 使用预测分析表解析输入
    bool parseInput(const vector<string>& input);
private:
    // 判断一个符号是否为非终结符
    bool isNonTerminal(const string& symbol);
    // 计算 FIRST 集合（用于推导右部 alpha 的 FIRST 集合）
    bool first(const vector<string>& alpha, set<string>& result);
};

void Grammar::readGrammar(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return;
    }

    string line;
    // Read number of non-terminals
    getline(file, line);
    int numNonTerminals = stoi(line);
    // Read non-terminals
    getline(file, line);
    istringstream iss(line);
    string symbol;
    while (iss >> symbol) {
        nonTerminals.insert(symbol);
    }
    // Read number of terminals
    getline(file, line);
    int numTerminals = stoi(line);
    // Read terminals
    getline(file, line);
    iss.clear();
    iss.str(line);
    while (iss >> symbol) {
        terminals.insert(symbol);
    }
    // Read number of productions
    getline(file, line);
    int numProductions = stoi(line);
    // Read productions
    for (int i = 0; i < numProductions; ++i) {
        getline(file, line);
        size_t pos = line.find("->");
        string left = line.substr(0, pos - 1);
        string rightStr = line.substr(pos + 2);
        iss.clear();
        iss.str(rightStr);
        vector<string> right;
        while (iss >> symbol) {
            // If the symbol is '蔚', replace it with 'ε'
            if (symbol == "蔚") {
                symbol = "ε";
            }
            right.push_back(symbol);
        }
        Production prod;
        prod.index = i;
        prod.left = left;
        prod.right = right;
        productions.push_back(prod);
    }
    // Read start symbol
    getline(file, startSymbol);

    file.close();
}

// 判断一个符号是否是非终结符
bool Grammar::isNonTerminal(const string& symbol) {
    return nonTerminals.find(symbol) != nonTerminals.end();
}

// 计算所有非终结符的 First 集合
void Grammar::computeFirstSet() {
    // 初始化每个非终结符的 First 集合为空
    for (const auto& nt : nonTerminals) {
        firstSet[nt] = set<string>();
    }

    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& prod : productions) {
            set<string> temp;
            if (first(prod.right, temp)) {
                size_t oldSize = firstSet[prod.left].size();
                firstSet[prod.left].insert(temp.begin(), temp.end());
                if (firstSet[prod.left].size() != oldSize) {
                    changed = true;
                }
            }
        }
    }
}

// 计算右边符号的 First 集合，结果存储在 result 中
bool Grammar::first(const vector<string>& alpha, set<string>& result) {
    bool hasEpsilon = true;
    for (const auto& symbol : alpha) {
        if (isNonTerminal(symbol)) {
            set<string> tempSet = firstSet[symbol];
            if (tempSet.find("ε") == tempSet.end()) {
                hasEpsilon = false;
            }
            else {
                tempSet.erase("ε");
            }
            result.insert(tempSet.begin(), tempSet.end());
            if (!hasEpsilon) {
                break;
            }
        }
        else {
            result.insert(symbol);
            hasEpsilon = false;
            break;
        }
    }
    if (hasEpsilon) {
        result.insert("ε");
    }
    return true;
}

// 计算Follow集合
void Grammar::computeFollowSet() {
    for (const auto& nt : nonTerminals) {
        followSet[nt] = set<string>();
    }
    followSet[startSymbol].insert("#");
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& prod : productions) {
            for (size_t i = 0; i < prod.right.size(); ++i) {
                string B = prod.right[i];
                if (isNonTerminal(B)) {
                    set<string> betaFirst;
                    if (i + 1 < prod.right.size()) {
                        vector<string> beta(prod.right.begin() + i + 1, prod.right.end());
                        first(beta, betaFirst);
                        size_t oldSize = followSet[B].size();
                        for (const auto& sym : betaFirst) {
                            if (sym != "ε") {
                                followSet[B].insert(sym);
                            }
                        }
                        if (betaFirst.find("ε") != betaFirst.end()) {
                            followSet[B].insert(followSet[prod.left].begin(), followSet[prod.left].end());
                        }
                        if (followSet[B].size() != oldSize) {
                            changed = true;
                        }
                    }
                    else {
                        size_t oldSize = followSet[B].size();
                        followSet[B].insert(followSet[prod.left].begin(), followSet[prod.left].end());
                        if (followSet[B].size() != oldSize) {
                            changed = true;
                        }
                    }
                }
            }
        }
    }
}

// 计算Select集合
void Grammar::computeSelectSet() {
    for (const auto& prod : productions) {
        set<string> select;
        set<string> firstAlpha;
        first(prod.right, firstAlpha);
        if (firstAlpha.find("ε") == firstAlpha.end()) {
            select.insert(firstAlpha.begin(), firstAlpha.end());
        }
        else {
            firstAlpha.erase("ε");
            select.insert(firstAlpha.begin(), firstAlpha.end());
            select.insert(followSet[prod.left].begin(), followSet[prod.left].end());
        }
        selectSet[prod.index] = select;
    }
}

// 构造预测分析表
void Grammar::buildParseTable() {
    for (const auto& prod : productions) {
        for (const auto& terminal : selectSet[prod.index]) {
            parseTable[prod.left][terminal] = prod.index;
        }
    }
}

// 打印文法
void Grammar::printGrammar() {
    cout << "\nCFG=(VN,VT,P,S)\n" << endl;
    cout << "VN: ";
    for (const auto& nt : nonTerminals) {
        cout << nt << " ";
    }
    cout << endl;
    cout << "VT: ";
    for (const auto& t : terminals) {
        cout << t << " ";
    }
    cout << endl;
    cout << "Production:" << endl;
    for (const auto& prod : productions) {
        cout << prod.index << ": " << prod.left << " -> ";
        for (const auto& sym : prod.right) {
            cout << sym << " ";
        }
        cout << endl;
    }
    cout << "StartSymbol: " << startSymbol << endl;
}

// 打印First集合
void Grammar::printFirstSet() {
    cout << "\n[First Set]" << endl;
    for (const auto& nt : nonTerminals) {
        cout << nt << " : ";
        for (const auto& sym : firstSet[nt]) {
            cout << sym << " ";
        }
        cout << endl;
    }
}

// 打印Follow集合
void Grammar::printFollowSet() {
    cout << "\n[Follow Set]" << endl;
    for (const auto& nt : nonTerminals) {
        cout << nt << " : ";
        for (const auto& sym : followSet[nt]) {
            cout << sym << " ";
        }
        cout << endl;
    }
}

// 打印Select集合
void Grammar::printSelectSet() {
    cout << "\n[Select Set]" << endl;
    for (const auto& prod : productions) {
        cout << prod.index << ": " << prod.left << " -> ";
        for (const auto& sym : prod.right) {
            cout << sym << " ";
        }
        cout << " : ";
        for (const auto& sym : selectSet[prod.index]) {
            cout << sym << " ";
        }
        cout << endl;
    }
}

// 解析输入符号串（总控程序）
bool Grammar::parseInput(const vector<string>& input) {
    vector<string> stack = { "#" };
    size_t inputIndex = 0;
    stack.push_back(startSymbol);

    cout << "预测分析过程:\n";
    while (!stack.empty()) {
        string X = stack.back();
        string c = input[inputIndex];

        cout << "栈顶: " << X << ", 当前输入符号: " << c << endl;

        if (X == c) {
            cout << "匹配: " << X << endl;
            stack.pop_back();
            inputIndex++;
        }
        else if (isNonTerminal(X)) {
            if (parseTable[X].count(c)) {
                int productionIndex = parseTable[X][c];
                const Production& prod = productions[productionIndex];
                stack.pop_back();
                if (prod.right != vector<string>{"ε"}) {
                    for (auto it = prod.right.rbegin(); it != prod.right.rend(); ++it) {
                        stack.push_back(*it);
                    }
                }
                cout << "应用产生式: 出栈 " << X << ", 输入符号: " << c << endl;
            }
            else {
                cout << "语法错误: " << X << " 与 " << c << " 不匹配。\n";
                return false;
            }
        }
        else {
            cout << "语法错误: " << X << " 与 " << c << " 不匹配。\n";
            return false;
        }
    }
    cout << "输入符号串匹配成功!" << endl;
    return true;
}

// 打印预测分析表
void Grammar::printParseTable() {
    cout << "\n预测分析表" << endl;
    cout << "     ";
    for (const auto& terminal : terminals) {
        cout << setw(5) << terminal;
    }
    cout << endl;

    cout << "-----+";
    for (size_t i = 0; i < terminals.size(); ++i) {
        cout << "-----";
    }
    cout << endl;

    for (const auto& nonTerminal : nonTerminals) {
        cout << setw(5) << nonTerminal;
        for (const auto& terminal : terminals) {
            if (parseTable[nonTerminal].count(terminal)) {
                cout << setw(4) << "p"<<parseTable[nonTerminal][terminal];
            }
            else {
                cout << setw(5) << " ";
            }
        }
        cout << endl;
    }
}

vector<string> readInputFromFile(const string& filename) {
    vector<string> input;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return input;
    }

    char ch;
    while (file.get(ch)) {
        // 跳过空白字符（空格、换行、制表符等）
        if (!isspace(ch)) {
            // 将字符转换为字符串并添加到向量中
            input.push_back(string(1, ch));
        }
    }

    file.close();

    // 在末尾添加结束符 #
    if (!input.empty()) {
        input.push_back("#");
    }

    return input;
}

int main(int argc, char *argv[]) {

    string path;
    cin >> path;
    Grammar grammar;
    grammar.readGrammar(path);
    grammar.computeFirstSet();
    grammar.computeFollowSet();
    grammar.computeSelectSet();
    grammar.buildParseTable();

    grammar.printGrammar();
    grammar.printFirstSet();
    grammar.printFollowSet();
    grammar.printSelectSet();
    grammar.printParseTable();

    string inputPath;
    cin >> inputPath;
    vector<string> input = readInputFromFile(inputPath);
    // for (auto i : input) {
    //     cout << i << endl;
    // }

    // vector<string> input = { "(", "a", ",", "a", ")" ,"#" };
    grammar.parseInput(input);

    return 0;
}