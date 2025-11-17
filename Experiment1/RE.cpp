#include <bits/stdc++.h>
using namespace std;

class NFA {
public:
    set<int> states;
    set<char> alphabet;
    int start_state;
    set<int> accept_states;
    map<int, map<char, set<int>>> transitions;

    NFA(set<int> states, set<char> alphabet, int start_state, set<int> accept_states,
        map<int, map<char, set<int>>> transitions)
        : states(states), alphabet(alphabet), start_state(start_state), accept_states(accept_states),
        transitions(transitions) {}

    void print() const {
        // 创建字符到整数的映射
        map<char, int> symbol_to_int;
        int symbol_id = 0;

        // 为字母表中的每个字符分配一个整数编号（ε除外）
        for (char c : alphabet) {
            if (c != 'e') {
                symbol_to_int[c] = symbol_id++;
            }
        }
        // ε的编号为-1
        symbol_to_int['e'] = -1;

        cout << "状态集：{";
        for (auto it = states.begin(); it != states.end(); ++it) {
            if (it != states.begin()) cout << ", ";
            cout << *it;
        }
        cout << "}\n";

        cout << "符号表：{";
        bool first = true;
        for (char c : alphabet) {
            if (c != 'e') {
                if (!first) cout << ", ";
                cout << symbol_to_int[c];
                first = false;
            }
        }
        cout << "}\n";

        cout << "状态转移：\n";
        for (const auto& trans_pair : transitions) {
            int from_state = trans_pair.first;
            const auto& trans_map = trans_pair.second;
            for (const auto& symbol_pair : trans_map) {
                char symbol = symbol_pair.first;
                const set<int>& to_states = symbol_pair.second;
                int symbol_code = (symbol == 'e') ? -1 : symbol_to_int[symbol];
                for (int to_state : to_states) {
                    cout << "(" << from_state << ", " << symbol_code << ") -> " << to_state << "\n";
                }
            }
        }

        cout << "开始状态：" << start_state << "\n";

        cout << "结束状态集：{";
        for (auto it = accept_states.begin(); it != accept_states.end(); ++it) {
            if (it != accept_states.begin()) cout << ", ";
            cout << *it;
        }
        cout << "}\n";
    }
};

class REToNFAConverter {
private:
    int state_counter;
    set<char> alphabet;

public:
    REToNFAConverter() : state_counter(0) {}

    // 添加连接符号 .
    string addConcatenation(const string& re) {
        string result;
        for (size_t i = 0; i < re.length(); i++) {
            result += re[i];
            if (i < re.length() - 1) {
                char current = re[i];
                char next = re[i + 1];
                // 需要添加连接符号的情况：
                // 字母后接字母、字母后接(、)后接字母、)后接(、*后接字母、*后接(
                if ((isalnum(current) && isalnum(next)) ||
                    (isalnum(current) && next == '(') ||
                    (current == ')' && isalnum(next)) ||
                    (current == ')' && next == '(') ||
                    (current == '*' && isalnum(next)) ||
                    (current == '*' && next == '(')) {
                    result += '.';
                }
            }
        }
        return result;
    }

    // 中缀转后缀表达式
    string infixToPostfix(const string& re) {
        stack<char> operators;
        string postfix;

        // 运算符优先级
        auto precedence = [](char op) {
            if (op == '*') return 3;
            if (op == '.') return 2;
            if (op == '|') return 1;
            return 0;
        };

        for (char c : re) {
            if (isalnum(c)) {
                // 操作数直接输出
                postfix += c;
                alphabet.insert(c);
            } else if (c == '(') {
                operators.push(c);
            } else if (c == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    postfix += operators.top();
                    operators.pop();
                }
                operators.pop(); // 弹出 '('
            } else {
                // 运算符
                while (!operators.empty() && precedence(operators.top()) >= precedence(c)) {
                    postfix += operators.top();
                    operators.pop();
                }
                operators.push(c);
            }
        }

        while (!operators.empty()) {
            postfix += operators.top();
            operators.pop();
        }

        return postfix;
    }

    // 处理基本字符
    NFA handleChar(char c) {
        int start = state_counter++;
        int end = state_counter++;
        set<int> states = {start, end};
        set<char> alpha = {c};
        set<int> accept = {end};
        map<int, map<char, set<int>>> trans;
        trans[start][c].insert(end);

        return NFA(states, alpha, start, accept, trans);
    }

    // 处理连接操作
    NFA handleConcatenation(const NFA& nfa1, const NFA& nfa2) {
        // 合并状态
        set<int> states = nfa1.states;
        states.insert(nfa2.states.begin(), nfa2.states.end());

        // 合并字母表
        set<char> alpha = nfa1.alphabet;
        alpha.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

        // 开始状态是nfa1的开始状态
        int start = nfa1.start_state;

        // 接受状态是nfa2的接受状态
        set<int> accept = nfa2.accept_states;

        // 合并转移函数
        map<int, map<char, set<int>>> trans = nfa1.transitions;
        for (const auto& trans_pair : nfa2.transitions) {
            trans[trans_pair.first] = trans_pair.second;
        }

        // 添加ε转移：从nfa1的每个接受状态到nfa2的开始状态
        for (int accept_state : nfa1.accept_states) {
            trans[accept_state]['e'].insert(nfa2.start_state);
        }

        return NFA(states, alpha, start, accept, trans);
    }

    // 处理选择操作
    NFA handleUnion(const NFA& nfa1, const NFA& nfa2) {
        int start = state_counter++;
        int end = state_counter++;

        // 合并状态
        set<int> states = nfa1.states;
        states.insert(nfa2.states.begin(), nfa2.states.end());
        states.insert(start);
        states.insert(end);

        // 合并字母表
        set<char> alpha = nfa1.alphabet;
        alpha.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

        // 接受状态
        set<int> accept = {end};

        // 合并转移函数
        map<int, map<char, set<int>>> trans = nfa1.transitions;
        for (const auto& trans_pair : nfa2.transitions) {
            trans[trans_pair.first] = trans_pair.second;
        }

        // 添加新的ε转移
        trans[start]['e'].insert(nfa1.start_state);
        trans[start]['e'].insert(nfa2.start_state);

        for (int accept1 : nfa1.accept_states) {
            trans[accept1]['e'].insert(end);
        }
        for (int accept2 : nfa2.accept_states) {
            trans[accept2]['e'].insert(end);
        }

        return NFA(states, alpha, start, accept, trans);
    }

    // 处理闭包操作
    NFA handleClosure(const NFA& nfa) {
        int start = state_counter++;
        int end = state_counter++;

        // 合并状态
        set<int> states = nfa.states;
        states.insert(start);
        states.insert(end);

        // 字母表不变
        set<char> alpha = nfa.alphabet;

        // 接受状态
        set<int> accept = {end};

        // 转移函数
        map<int, map<char, set<int>>> trans = nfa.transitions;

        // 添加新的ε转移
        trans[start]['e'].insert(nfa.start_state);
        trans[start]['e'].insert(end);

        for (int accept_state : nfa.accept_states) {
            trans[accept_state]['e'].insert(nfa.start_state);
            trans[accept_state]['e'].insert(end);
        }

        return NFA(states, alpha, start, accept, trans);
    }

    // 主转换函数
    NFA convert(const string& re) {
        string re_with_concat = addConcatenation(re);
        string postfix = infixToPostfix(re_with_concat);

        stack<NFA> nfa_stack;

        for (char c : postfix) {
            if (isalnum(c)) {
                // 基本字符
                nfa_stack.push(handleChar(c));
            } else if (c == '.') {
                // 连接操作
                NFA nfa2 = nfa_stack.top(); nfa_stack.pop();
                NFA nfa1 = nfa_stack.top(); nfa_stack.pop();
                nfa_stack.push(handleConcatenation(nfa1, nfa2));
            } else if (c == '|') {
                // 选择操作
                NFA nfa2 = nfa_stack.top(); nfa_stack.pop();
                NFA nfa1 = nfa_stack.top(); nfa_stack.pop();
                nfa_stack.push(handleUnion(nfa1, nfa2));
            } else if (c == '*') {
                // 闭包操作
                NFA nfa = nfa_stack.top(); nfa_stack.pop();
                nfa_stack.push(handleClosure(nfa));
            }
        }

        return nfa_stack.top();
    }
};


class DFA {
public:
    set<int> states;
    set<char> alphabet;
    int start_state;
    set<int> accept_states;
    map<int, map<char, int>> transitions;

    DFA(set<int> states, set<char> alphabet, int start_state, set<int> accept_states,
        map<int, map<char, int>> transitions)
        : states(states), alphabet(alphabet), start_state(start_state), accept_states(accept_states),
        transitions(transitions) {}

    void print() const {
        // 创建字符到整数的映射
        map<char, int> symbol_to_int;
        int symbol_id = 0;

        // 为字母表中的每个字符分配一个整数编号
        for (char c : alphabet) {
            symbol_to_int[c] = symbol_id++;
        }

        cout << "状态集：{";
        for (auto it = states.begin(); it != states.end(); ++it) {
            if (it != states.begin()) cout << ", ";
            cout << *it;
        }
        cout << "}\n";

        cout << "符号表：{";
        bool first = true;
        for (char c : alphabet) {
            if (!first) cout << ", ";
            cout << symbol_to_int[c];
            first = false;
        }
        cout << "}\n";

        cout << "状态转换：\n";
        for (const auto& trans_pair : transitions) {
            int state = trans_pair.first;
            const auto& trans_map = trans_pair.second;
            for (const auto& symbol_pair : trans_map) {
                char symbol = symbol_pair.first;
                int target_state = symbol_pair.second;
                int symbol_code = symbol_to_int[symbol];
                cout << "(" << state << ", " << symbol_code << ") -> " << target_state << "\n";
            }
        }

        cout << "开始状态：" << start_state << "\n";
        cout << "终止状态集：{";
        for (auto it = accept_states.begin(); it != accept_states.end(); ++it) {
            if (it != accept_states.begin()) cout << ", ";
            cout << *it;
        }
        cout << "}\n";
    }
};

//转换成DFA
DFA nfa_to_dfa(const NFA& nfa) {
    struct SetComparator {
        bool operator()(const set<int>& a, const set<int>& b) const {
            return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
        }
    };

    map<set<int>, int, SetComparator> dfa_state_map;
    vector<set<int>> dfa_states_list;
    set<int> dfa_states;
    map<int, map<char, int>> dfa_transitions;
    set<int> dfa_accept_states;
    set<char> alphabet = nfa.alphabet; alphabet.erase('e');

    auto epsilon_closure = [&](const set<int>& states) -> set<int> {
        set<int> closure = states;

        stack<int> stk;
        for (int state : states) {
            stk.push(state);
        }
        while (!stk.empty()) {
            int state = stk.top();
            stk.pop();

            if (nfa.transitions.count(state) && nfa.transitions.at(state).count('e')) {
                for (int next_state : nfa.transitions.at(state).at('e')) {
                    if (closure.insert(next_state).second) {
                        stk.push(next_state);
                    }
                }
            }
        }
        return closure;
    };

    queue<set<int>> state_queue;
    set<int> start_set = epsilon_closure({ nfa.start_state });
    int state_counter = 0;
    int dfa_start_state = state_counter++;
    dfa_state_map[start_set] = dfa_start_state;

    dfa_states_list.push_back(start_set);
    dfa_states.insert(dfa_start_state);
    state_queue.push(start_set);

    if (any_of(start_set.begin(), start_set.end(), [&](int s) { return nfa.accept_states.count(s); })) {
        dfa_accept_states.insert(dfa_start_state);
    }

    while (!state_queue.empty()) {
        set<int> current_set = state_queue.front();
        state_queue.pop();
        int current_dfa_state = dfa_state_map[current_set];

        for (char symbol : alphabet) {
            set<int> move_set;

            for (int state : current_set) {
                if (nfa.transitions.count(state) && nfa.transitions.at(state).count(symbol)) {
                    const set<int>& next_states = nfa.transitions.at(state).at(symbol);
                    move_set.insert(next_states.begin(), next_states.end());
                }
            }

            set<int> next_set = epsilon_closure(move_set);

            if (!next_set.empty()) {
                if (dfa_state_map.find(next_set) == dfa_state_map.end()) {
                    int new_dfa_state = state_counter++;
                    dfa_state_map[next_set] = new_dfa_state;
                    dfa_states_list.push_back(next_set);
                    dfa_states.insert(new_dfa_state);
                    state_queue.push(next_set);

                    if (any_of(next_set.begin(), next_set.end(), [&](int s) { return nfa.accept_states.count(s); })) {
                        dfa_accept_states.insert(new_dfa_state);
                    }
                }
                int next_dfa_state = dfa_state_map[next_set];
                dfa_transitions[current_dfa_state][symbol] = next_dfa_state;
            }
        }
    }
    return DFA(dfa_states, alphabet, dfa_start_state, dfa_accept_states, dfa_transitions);
}

// 最小化 DFA
DFA minimize_dfa(const DFA& dfa) {
    vector<set<int>> partitions;
    set<int> accept_part = dfa.accept_states;
    set<int> non_accept_part;

    for (int state : dfa.states) {
        if (!dfa.accept_states.count(state)) {
            non_accept_part.insert(state);
        }
    }
    if (!accept_part.empty()) partitions.push_back(accept_part);
    if (!non_accept_part.empty()) partitions.push_back(non_accept_part);

    map<int, int> state_partition_map;
    for (size_t i = 0; i < partitions.size(); ++i) {
        for (int state : partitions[i]) {
            state_partition_map[state] = i;
        }
    }

    bool updated;
    do {
        updated = false;
        vector<set<int>> new_partitions;
        map<int, int> new_state_partition_map;

        for (const set<int>& part : partitions) {
            map<map<char, int>, set<int>> group_map;
            for (int state : part) {
                map<char, int> trans_signature;
                for (char symbol : dfa.alphabet) {
                    if (dfa.transitions.count(state) && dfa.transitions.at(state).count(symbol)) {
                        int target_state = dfa.transitions.at(state).at(symbol);
                        trans_signature[symbol] = state_partition_map[target_state];
                    }
                    else {
                        trans_signature[symbol] = -1;
                    }
                }
                group_map[trans_signature].insert(state);
            }
            for (const auto& group_pair : group_map) {
                new_partitions.push_back(group_pair.second);
            }
        }

        if (new_partitions.size() != partitions.size()) {
            updated = true;
        }
        else {
            for (size_t i = 0; i < new_partitions.size(); ++i) {
                if (new_partitions[i] != partitions[i]) {
                    break;
                }
            }
        }

        partitions = new_partitions;
        state_partition_map.clear();

        for (size_t i = 0; i < partitions.size(); ++i) {
            for (int state : partitions[i]) {
                state_partition_map[state] = i;
            }
        }
    } while (updated);

    set<int> minimized_states;
    set<int> minimized_accept_states;
    map<int, map<char, int>> minimized_transitions;

    for (size_t i = 0; i < partitions.size(); ++i) {
        minimized_states.insert(i);
        for (int state : partitions[i]) {
            if (dfa.accept_states.count(state)) {
                minimized_accept_states.insert(i);
                break;
            }
        }
    }

    for (size_t i = 0; i < partitions.size(); ++i) {
        int representative_state = *partitions[i].begin();
        for (char symbol : dfa.alphabet) {
            if (dfa.transitions.count(representative_state) && dfa.transitions.at(representative_state).count(symbol)) {
                int target_state = dfa.transitions.at(representative_state).at(symbol);
                int target_partition = state_partition_map[target_state];
                minimized_transitions[i][symbol] = target_partition;
            }
        }
    }

    int minimized_start_state = state_partition_map[dfa.start_state];

    return DFA(minimized_states, dfa.alphabet, minimized_start_state, minimized_accept_states, minimized_transitions);
}



int main(int argc, char* argv[]) {
    // if (argc != 2) {
    //     cerr << "用法: " << argv[0] << " \"正则表达式\"" << endl;
    //     return 1;
    // }
    //
    // string re = argv[1];

    string re = "a(b|c)*abc";
    REToNFAConverter converter;
    NFA nfa = converter.convert(re);
    cout << "NFA" << endl;
    nfa.print();
    return 0;
}