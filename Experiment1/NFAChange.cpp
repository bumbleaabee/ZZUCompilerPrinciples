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
        cout << "状态集：{";
        for (auto it = states.begin(); it != states.end(); ++it) {
            if (it != states.begin()) cout << ", ";
            cout << *it;
        }
        cout << "}\n";

        cout << "符号表：{";
        for (auto it = alphabet.begin(); it != alphabet.end(); ++it) {
            if (it != alphabet.begin()) cout << ", ";
            cout << *it;
        }
        cout << "}\n";

        cout << "状态转移：\n";
        for (const auto& trans_pair : transitions) {
            int from_state = trans_pair.first;
            const auto& trans_map = trans_pair.second;
            for (const auto& symbol_pair : trans_map) {
                char symbol = symbol_pair.first;
                const set<int>& to_states = symbol_pair.second;
                for (int to_state : to_states) {
                    cout << "(" << from_state << ", " << symbol << ") -> " << to_state << "\n";
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
        cout << "状态集：{";
        for (auto it = states.begin(); it != states.end(); ++it) {
            if (it != states.begin()) cout << ", ";
            cout << *it;
        }
        cout << "}\n";

        cout << "符号表：{";
        for (auto it = alphabet.begin(); it != alphabet.end(); ++it) {
            if (it != alphabet.begin()) cout << ", ";
            cout << *it;
        }
        cout << "}\n";

        cout << "状态转换：\n";
        for (const auto& trans_pair : transitions) {
            int state = trans_pair.first;
            const auto& trans_map = trans_pair.second;
            for (const auto& symbol_pair : trans_map) {
                char symbol = symbol_pair.first;
                int target_state = symbol_pair.second;
                cout << "(" << state << ", " << symbol << ") -> " << target_state << "\n";
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

//读取NFA
NFA read_nfa_from_file(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件 " << filename << endl;
        exit(1);
    }

    set<int> states;
    set<char> alphabet;
    int start_state = -1;
    set<int> accept_states;
    map<int, map<char, set<int>>> transitions;

    string line;
    vector<string> lines;
    while (getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    file.close();

    size_t index = 0;

    if (index >= lines.size()) {
        cerr << "文件 " << filename << " 中缺少状态行" << endl;
        exit(1);
    }
    istringstream states_stream(lines[index++]);
    int state;
    while (states_stream >> state) {
        states.insert(state);
    }

    if (index >= lines.size()) {
        cerr << "文件 " << filename << " 中缺少字母表行" << endl;
        exit(1);
    }
    istringstream alphabet_stream(lines[index++]);
    string symbol_str;
    while (alphabet_stream >> symbol_str) {
        if (symbol_str != "-1") {
            alphabet.insert(symbol_str[0]);
        }
    }

    if (index >= lines.size()) {
        cerr << "文件 " << filename << " 中缺少转换数量行" << endl;
        exit(1);
    }
    int transition_count;
    istringstream transition_count_stream(lines[index++]);
    if (!(transition_count_stream >> transition_count)) {
        cerr << "文件 " << filename << " 中的转换数量无效" << endl;
        exit(1);
    }

    for (int i = 0; i < transition_count; ++i) {
        if (index >= lines.size()) {
            cerr << "文件 " << filename << " 中第 " << index + 1 << " 行缺少转换" << endl;
            exit(1);
        }
        istringstream transition_stream(lines[index++]);
        int from_state;
        string symbol_str;
        if (!(transition_stream >> from_state >> symbol_str)) {
            cerr << "文件 " << filename << " 中第 " << index << " 行的转换格式无效" << endl;
            exit(1);
        }
        char trans_symbol = (symbol_str == "-1") ? 'e' : symbol_str[0];
        int to_state;
        while (transition_stream >> to_state) {
            transitions[from_state][trans_symbol].insert(to_state);
        }
    }

    if (index >= lines.size()) {
        cerr << "文件 " << filename << " 中缺少开始状态" << endl;
        exit(1);
    }
    istringstream start_state_stream(lines[index++]);
    if (!(start_state_stream >> start_state)) {
        cerr << "文件 " << filename << " 中的开始状态无效" << endl;
        exit(1);
    }

    if (index >= lines.size()) {
        cerr << "文件 " << filename << " 中缺少结束状态" << endl;
        exit(1);
    }
    istringstream accept_states_stream(lines[index++]);
    while (accept_states_stream >> state) {
        accept_states.insert(state);
    }

    if (start_state == -1) {
        cerr << "文件 " << filename << " 中未定义开始状态" << endl;
        exit(1);
    }

    return NFA(states, alphabet, start_state, accept_states, transitions);
}

//合并NFA
NFA merge_nfas(const NFA& nfa1, const NFA& nfa2) {
    set<int> merged_states = nfa1.states;
    merged_states.insert(nfa2.states.begin(), nfa2.states.end());

    set<char> merged_alphabet = nfa1.alphabet;
    merged_alphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

    int merged_start_state = 0;

    set<int> merged_accept_states = nfa1.accept_states;
    merged_accept_states.insert(nfa2.accept_states.begin(), nfa2.accept_states.end());

    map<int, map<char, set<int>>> merged_transitions = nfa1.transitions;

    for (const auto& trans1 : nfa2.transitions) {
        int from_state = trans1.first;
        for (const auto& trans2 : trans1.second) {
            char symbol = trans2.first;
            const set<int>& to_states = trans2.second;
            merged_transitions[from_state][symbol].insert(to_states.begin(), to_states.end());
        }
    }

    map<char, set<int>> start_transitions;
    start_transitions['e'] = { nfa1.start_state, nfa2.start_state };
    merged_transitions[merged_start_state] = start_transitions;

    return NFA(merged_states, merged_alphabet, merged_start_state, merged_accept_states, merged_transitions);
}

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

int main(int argc,char *argv[]) {
    int n;
    cin >> n;

    vector<string> files(n);
    for (int i = 0; i < n; ++i) {
        cin >> files[i];
    }
    NFA merged_nfa = read_nfa_from_file(files[0]);
    for (int i = 1; i < n; ++i) {
        NFA nfa = read_nfa_from_file(files[i]);
        merged_nfa = merge_nfas(merged_nfa, nfa);
    }

    cout << "合并之后的NFA为:" << endl;
    merged_nfa.print();

    cout << endl;
    cout << "转换之后的DFA为:" << endl;
    DFA dfa = nfa_to_dfa(merged_nfa);

    DFA minimized_dfa = minimize_dfa(dfa);

    dfa.print();
    cout << endl;
    cout << "最小化之后的DFA为:" << endl;
    minimized_dfa.print();

    return 0;
}