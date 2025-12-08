//
// Created by lenovo on 25-12-4.
//
// merged.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <set>
using namespace std;

// 非终结符集
class VN {
private:
    int n;
    set<string> vn;
public:
    int getN() const;
    const set<string>& getVn() const;
    void setVn(const set<string>& vn);
    void setN(int n);
};

// 终结符集
class VT {
private:
    int n;
    set<string> vt;
public:
    int getN() const;
    void setN(int n);
    const set<string>& getVt() const;
    void setVt(const set<string>& vt);
};

// 产生式
class P {
private:
    int n;
    vector<string> p1;  // 存放左部
    vector<vector<string>> p2;  // 存放右部
public:
    const vector<vector<string>>& getP2() const;
    void setP2(const vector<vector<string>>& p2);
    const vector<string>& getP1() const;
    void setP1(const vector<string>& p1);
    int getN() const;
    void setN(int n);
};

// 开始符号
class S {
private:
    string s;
public:
    const string& getS() const;
    void setS(const string& s);
};

// 上下文无关文法
class CFG {
public:
    string filewenfa;  // 文法文件
    VN vn;
    VT vt;
    P p;
    S s;

public:
    void setFilewenfa(const string& filewenfa);
    void fileop();  // 读文件操作
};

// LR(1)分析程序
class Program {
private:
    string filewenfa;   // 文法文件
    string fileAG;      // Actiongoto表
    string filestr;     // 待分析串
    CFG cfg;
    vector<string> anstr;   // 储存待分析串
    map<pair<int, int>, string> action;   // action表
    map<pair<int, int>, int> Goto;    // goto表
    map<string, int> X;  // 终结符编号
    map<string, int> Y;  // 非终结符编号
public:
    void setFileAg(const string& fileAg);
    void setFilestr(const string& filestr);
    void setFilewenfa(const string& filewenfa);
    void fileAGop();    // 读分析表
    void filestrop();   // 读待分析串
    void initstr();     // 按分析表给非终结符和终结符编号
    void fileop();      // 调用所有读文件操作
    void allprogram();  // 总控分析
};

// VN类的实现
int VN::getN() const {
    return n;
}

const set<string>& VN::getVn() const {
    return vn;
}

void VN::setVn(const set<string>& vn) {
    this->vn = vn;
}

void VN::setN(int n) {
    this->n = n;
}

// VT类的实现
int VT::getN() const {
    return n;
}

void VT::setN(int n) {
    this->n = n;
}

const set<string>& VT::getVt() const {
    return vt;
}

void VT::setVt(const set<string>& vt) {
    this->vt = vt;
}

// P类的实现
const vector<vector<string>>& P::getP2() const {
    return p2;
}

void P::setP2(const vector<vector<string>>& p2) {
    this->p2 = p2;
}

const vector<string>& P::getP1() const {
    return p1;
}

void P::setP1(const vector<string>& p1) {
    this->p1 = p1;
}

int P::getN() const {
    return n;
}

void P::setN(int n) {
    this->n = n;
}

// S类的实现
const string& S::getS() const {
    return s;
}

void S::setS(const string& s) {
    this->s = s;
}

// CFG类的实现
void CFG::setFilewenfa(const string& filewenfa) {
    this->filewenfa = filewenfa;
}

void CFG::fileop() {
    string fileline;  // 用于存储每行的内容
    string str;       // 用于存储单个符号
    set<string> middle;  // 存储临时的符号集合
    vector<string> p1;   // 存储产生式左侧
    vector<string> mid;  // 存储产生式右侧
    vector<vector<string>> p2;  // 存储所有产生式的右侧
    ifstream fout(filewenfa);  // 打开文法文件

    for (int i = 1; getline(fout, fileline); i++) {  // 按行读取文件
        if (i == 1) {
            vn.setN(atoi(fileline.c_str()));  // 第一行表示非终结符的数量
        }
        else if (i == 2) {  // 第二行表示非终结符集合
            istringstream is(fileline);
            while (is >> str) {
                middle.insert(str);  // 将每个符号加入集合
            }
            vn.setVn(middle);  // 设置非终结符集合
            middle.clear();    // 清空临时集合
        }
        else if (i == 3) {  // 第三行表示终结符的数量
            vt.setN(atoi(fileline.c_str()));
        }
        else if (i == 4) {  // 第四行表示终结符集合
            istringstream is(fileline);
            while (is >> str) {
                middle.insert(str);  // 将每个符号加入集合
            }
            vt.setVt(middle);  // 设置终结符集合
            middle.clear();    // 清空临时集合
        }
        else if (i == 5) {  // 第五行表示产生式数量
            p.setN(atoi(fileline.c_str()));
        }
        else if (i == p.getN() + 6) {  // 最后一行表示开始符号
            s.setS(fileline.c_str());
        }
        else {  // 处理产生式规则
            istringstream is(fileline);
            for (int j = 0; is >> str; j++) {
                if (j == 0) {
                    p1.push_back(str);  // 产生式的左侧
                }
                else if (j == 1);  // 忽略 "->"
                else {
                    mid.push_back(str);  // 产生式的右侧
                }
            }
            p2.push_back(mid);  // 保存产生式右侧
            mid.clear();  // 清空右侧临时集合
        }
    }
    fout.close();  // 关闭文件
    p.setP1(p1);  // 设置产生式的左侧
    p.setP2(p2);  // 设置产生式的右侧
}

// Program类的实现
void Program::setFileAg(const string& fileAg) {
    fileAG = fileAg;
}

void Program::setFilestr(const string& filestr) {
    Program::filestr = filestr;
}

void Program::setFilewenfa(const string& filewenfa) {
    Program::filewenfa = filewenfa;
}

void Program::fileAGop() {
    int num;  // 用于存储分析表的行数
    string str;  // 用于存储单个符号
    pair<int, int> midp;  // 用于存储表中的状态编号和符号编号
    ifstream fout(this->fileAG);  // 打开分析表文件
    string fileline;
    for (int i = 1; getline(fout, fileline); i++) {  // 逐行读取文件
        if (i == 1) {
            num = atoi(fileline.c_str());  // 获取表的行数
            continue;
        }
        else if (i != 1 && i <= num + 1) {  // 填充 action 表
            istringstream is(fileline);
            is >> str;
            midp.first = atoi(str.c_str());  // 状态编号
            str.clear();
            is >> str;
            midp.second = atoi(str.c_str());  // 符号编号
            str.clear();
            is >> str;
            this->action.emplace(midp, str);  // 填充 action 表
            str.clear();
        }
        else if (i == num + 2) {  // 跳过空行或表头
        }
        else {  // 填充 Goto 表
            istringstream is(fileline);
            is >> str;
            midp.first = atoi(str.c_str());  // 状态编号
            str.clear();
            is >> str;
            midp.second = atoi(str.c_str());  // 符号编号
            str.clear();
            is >> str;
            this->Goto.emplace(midp, atoi(str.c_str()));  // 填充 Goto 表
            str.clear();
        }
    }
    fout.close();  // 关闭文件
}

void Program::filestrop() {
    ifstream fout(this->filestr);  // 打开待分析串文件
    stringstream buffer;
    buffer << fout.rdbuf();  // 将文件内容读入字符串流
    fout.close();
    string ss;
    string contents(buffer.str());  // 获取整个文件内容
    istringstream is(contents);
    while (is >> ss) {
        this->anstr.push_back(ss);  // 将每个符号加入待分析串
    }
}

void Program::initstr() {
    int i = 0;
    for (set<string>::iterator it = cfg.vt.getVt().begin(); it != cfg.vt.getVt().end(); it++) {
        this->X.emplace(*it, i);  // 为终结符分配编号
        i++;
    }
    this->X.emplace("#", i);  // 添加结束符号 "#" 的编号
    i = 0;
    for (set<string>::iterator it = cfg.vn.getVn().begin(); it != cfg.vn.getVn().end(); it++) {
        if (*it != cfg.s.getS()) this->Y.emplace(*it, i);  // 排除开始符号
        i++;
    }
}

void Program::fileop() {
    this->cfg.setFilewenfa(filewenfa);  // 设置文法文件
    this->cfg.fileop();  // 调用 CFG 的 fileop 方法读取文法文件
    this->filestrop();   // 读取待分析串
    this->fileAGop();    // 读取分析表
    initstr();           // 初始化终结符和非终结符编号
}

void Program::allprogram() {
    string sstr;
    string a;
    stack<int> state;  // 状态栈
    stack<string> str;  // 符号栈。
    state.push(0);  // 初始状态入栈
    str.push("#");  // 结束符号入符号栈
    int k = 1;
    pair<int, int> midp;
    midp.first = state.top();
    a = anstr[0];
    midp.second = X[a];  // 获取当前符号的编号（在 X 表中）

    cout << "栈顶    " << "输入    " << "查表    " << "动作    " << endl;

    // 开始分析过程
    while (action[midp] != "acc") {  // 一直到接受（"acc"）状态
        cout << state.top() << " " << str.top() << "     " << a << "      ";
        if (action[midp][0] == 's') {  // 进栈操作
            sstr = action[midp];
            sstr.erase(0, 1);  // 移除's'字符，得到状态编号
            state.push(atoi(sstr.c_str()));  // 将状态编号压入栈
            cout << action[midp] << "     " << "进栈" << " " << atoi(sstr.c_str()) << " " << a << endl;
            str.push(a);  // 将当前符号入栈
            a = anstr[k];  // 获取下一个待分析符号
            k++;
        }
        else if (action[midp][0] == 'r') {  // 归约操作
            sstr = action[midp];
            sstr.erase(0, 1);  // 移除'r'字符，得到规则编号
            int n = atoi(sstr.c_str());
            for (int i = 0; i < cfg.p.getP2()[n].size(); i++) {  // 归约时移除符号和状态
                state.pop();
                str.pop();
            }
            str.push(cfg.p.getP1()[n]);  // 将归约的非终结符入栈
            cout << action[midp] << "     " << "出栈" << cfg.p.getP2()[n].size() << "个符号和状态";
            midp.first = state.top();
            midp.second = Y[cfg.p.getP1()[n]];  // 获取归约后的非终结符编号
            if (Goto.count(midp) == 1) {  // 查Goto表进行状态转移
                state.push(Goto[midp]);
            }
            cout << "进栈" << state.top() << " " << str.top() << " ";
            cout << cfg.p.getP1()[n] << "->";
            for (int i = 0; i < cfg.p.getP2()[n].size(); i++) {
                cout << cfg.p.getP2()[n][i];
            }
            cout << endl;
        }
        else {  // 错误处理
            cout << "error!" << endl;
            break;
        }

        midp.first = state.top();
        midp.second = X[a];
        if (action[midp] == "acc") {  // 如果状态是接受状态
            cout << state.top() << " " << str.top() << "    " << a << "       " << action[midp] << "    " << "成功接受！" << endl;
            break;  // 接受成功，结束分析
        }
    }
}

int main() {
    string filewenfa = "D:\\Files\\ZZU\\编译原理\\experiment\\FirstClion\\experiment3\\LRExample\\a.txt";
    string fileAG = "D:\\Files\\ZZU\\编译原理\\experiment\\FirstClion\\experiment3\\LRExample\\b.txt";
    string filestr = "D:\\Files\\ZZU\\编译原理\\experiment\\FirstClion\\experiment3\\LRExample\\c.txt";

    Program program;
    program.setFilewenfa(filewenfa);
    program.setFileAg(fileAG);
    program.setFilestr(filestr);
    program.fileop();   //读文件
    program.allprogram();   //总控分析
    return 0;
}