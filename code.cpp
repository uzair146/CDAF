#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <algorithm>
#include <climits>
#include <stdexcept>
#include <utility>

using namespace std;

// ============================================================
//  MODULE 1.1 : DiscreteSet<T>
// ============================================================
template <typename T>
class DiscreteSet {
private:
    T* data;
    int capacity;
    int count;

    void resize() {
        capacity *= 2;
        T* nd = new T[capacity];
        for (int i = 0; i < count; i++) nd[i] = data[i];
        delete[] data;
        data = nd;
    }

    int binarySearch(const T& val) const {
        int lo = 0, hi = count - 1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (data[mid] == val) return mid;
            else if (data[mid] < val) lo = mid + 1;
            else hi = mid - 1;
        }
        return -1;
    }

public:
    DiscreteSet() : capacity(8), count(0) { data = new T[capacity]; }

    DiscreteSet(const DiscreteSet<T>& o) : capacity(o.capacity), count(o.count) {
        data = new T[capacity];
        for (int i = 0; i < count; i++) data[i] = o.data[i];
    }

    DiscreteSet<T>& operator=(const DiscreteSet<T>& o) {
        if (this == &o) return *this;
        delete[] data;
        capacity = o.capacity; count = o.count;
        data = new T[capacity];
        for (int i = 0; i < count; i++) data[i] = o.data[i];
        return *this;
    }

    ~DiscreteSet() { delete[] data; }

    void add(T element) {
        if (member(element)) return;
        if (count >= capacity) resize();
        int i = count - 1;
        while (i >= 0 && data[i] > element) { data[i+1] = data[i]; i--; }
        data[i+1] = element;
        count++;
    }

    void delete_element(const T& element) {
        int idx = binarySearch(element);
        if (idx == -1) return;
        for (int i = idx; i < count - 1; i++) data[i] = data[i+1];
        count--;
    }

    bool member(const T& element) const { return binarySearch(element) != -1; }
    int  size()  const { return count; }
    bool empty() const { return count == 0; }
    void clear_all()   { count = 0; }

    T get(int idx) const {
        if (idx < 0 || idx >= count) throw out_of_range("Index out of range");
        return data[idx];
    }

    DiscreteSet<T> set_union(const DiscreteSet<T>& o) const {
        DiscreteSet<T> r = *this;
        for (int i = 0; i < o.count; i++) r.add(o.data[i]);
        return r;
    }

    DiscreteSet<T> set_intersection(const DiscreteSet<T>& o) const {
        DiscreteSet<T> r;
        for (int i = 0; i < count; i++) if (o.member(data[i])) r.add(data[i]);
        return r;
    }

    DiscreteSet<T> set_difference(const DiscreteSet<T>& o) const {
        DiscreteSet<T> r;
        for (int i = 0; i < count; i++) if (!o.member(data[i])) r.add(data[i]);
        return r;
    }

    DiscreteSet<T> symmetric_diff(const DiscreteSet<T>& o) const {
        return set_difference(o).set_union(o.set_difference(*this));
    }

    bool is_subset(const DiscreteSet<T>& o) const {
        for (int i = 0; i < count; i++) if (!o.member(data[i])) return false;
        return true;
    }

    bool is_strict_subset(const DiscreteSet<T>& o) const {
        return is_subset(o) && count < o.count;
    }

    bool operator==(const DiscreteSet<T>& o) const {
        if (count != o.count) return false;
        for (int i = 0; i < count; i++) if (data[i] != o.data[i]) return false;
        return true;
    }

    bool operator<(const DiscreteSet<T>& o) const {
        if (count != o.count) return count < o.count;
        for (int i = 0; i < count; i++) if (data[i] != o.data[i]) return data[i] < o.data[i];
        return false;
    }

    bool operator>(const DiscreteSet<T>& o) const { return o < *this; }

    DiscreteSet<DiscreteSet<T>> powerset() const {
        DiscreteSet<DiscreteSet<T>> result;
        int total = 1 << count;
        for (int mask = 0; mask < total; mask++) {
            DiscreteSet<T> subset;
            for (int i = 0; i < count; i++) if (mask & (1 << i)) subset.add(data[i]);
            result.add(subset);
        }
        return result;
    }

    DiscreteSet<pair<T,T>> cartesian_prod(const DiscreteSet<T>& o) const {
        DiscreteSet<pair<T,T>> result;
        for (int i = 0; i < count; i++)
            for (int j = 0; j < o.count; j++)
                result.add({data[i], o.data[j]});
        return result;
    }

    friend ostream& operator<<(ostream& os, const DiscreteSet<T>& s) {
        os << "{";
        for (int i = 0; i < s.count; i++) { if (i > 0) os << ","; os << s.data[i]; }
        os << "}";
        return os;
    }

    friend istream& operator>>(istream& is, DiscreteSet<T>& s) {
        char c; is >> c;
        T val;
        while (is.peek() != '}') {
            is >> val; s.add(val);
            if (is.peek() == ',') is >> c;
        }
        is >> c;
        return is;
    }
};

// ============================================================
//  MODULE 1.2 : PredicateEngine
// ============================================================
template <typename T>
using LogicalPredicate = function<bool(const T&)>;

class PredicateEngine {
public:
    template <typename T>
    static bool forall(const DiscreteSet<T>& universe, LogicalPredicate<T> P) {
        for (int i = 0; i < universe.size(); i++) if (!P(universe.get(i))) return false;
        return true;
    }

    template <typename T>
    static bool exists(const DiscreteSet<T>& universe, LogicalPredicate<T> P) {
        for (int i = 0; i < universe.size(); i++) if (P(universe.get(i))) return true;
        return false;
    }

    template <typename T>
    static bool exists_unique(const DiscreteSet<T>& universe, LogicalPredicate<T> P) {
        int cnt = 0;
        for (int i = 0; i < universe.size(); i++) if (P(universe.get(i))) cnt++;
        return cnt == 1;
    }

    template <typename T, typename U>
    static bool forall_exists(
        const DiscreteSet<T>& A,
        const DiscreteSet<U>& B,
        function<bool(const T&, const U&)> R
    ) {
        for (int i = 0; i < A.size(); i++) {
            bool found = false;
            for (int j = 0; j < B.size(); j++) if (R(A.get(i), B.get(j))) { found = true; break; }
            if (!found) return false;
        }
        return true;
    }

    template <typename T>
    static bool verify_not_forall_equals_exists_not(const DiscreteSet<T>& universe, LogicalPredicate<T> P) {
        bool lhs = !forall(universe, P);
        bool rhs = exists(universe, [&](const T& x) -> bool { return !P(x); });
        return lhs == rhs;
    }

    template <typename T>
    static bool verify_not_exists_equals_forall_not(const DiscreteSet<T>& universe, LogicalPredicate<T> P) {
        bool lhs = !exists(universe, P);
        bool rhs = forall(universe, [&](const T& x) -> bool { return !P(x); });
        return lhs == rhs;
    }
};

// ============================================================
//  MODULE 1.3 : GraphConstraintBuilder
// ============================================================
class GraphConstraintBuilder {
public:
    // Formula: (critical_infrastructure U high_traffic) N redundancy_nodes
    static DiscreteSet<int> compute_mandatory_nodes(
        const DiscreteSet<int>& critical_infrastructure,
        const DiscreteSet<int>& high_traffic,
        const DiscreteSet<int>& redundancy_nodes
    ) {
        DiscreteSet<int> uni = critical_infrastructure.set_union(high_traffic);
        return uni.set_intersection(redundancy_nodes);
    }

    // Formula: faulty_nodes U (all_nodes \ mandatory_nodes)
    static DiscreteSet<int> compute_forbidden_nodes(
        const DiscreteSet<int>& all_nodes,
        const DiscreteSet<int>& mandatory_nodes,
        const DiscreteSet<int>& faulty_nodes
    ) {
        return faulty_nodes.set_union(all_nodes.set_difference(mandatory_nodes));
    }

    static bool verify_coverage(
        const DiscreteSet<int>& total_universe,
        const vector<DiscreteSet<int>>& coverage_sets
    ) {
        DiscreteSet<int> combined;
        for (const auto& s : coverage_sets) combined = combined.set_union(s);
        return combined == total_universe;
    }
};

// ============================================================
//  MODULE 2.1 : NetworkGraph
// ============================================================
class NetworkGraph {
private:
    int vertex_count;
    vector<vector<int>> matrix;
    vector<list<pair<int,int>>> lists;
    bool is_directed;

    void dfs(int v, vector<bool>& visited) const {
        visited[v] = true;
        for (auto& p : lists[v]) if (!visited[p.first]) dfs(p.first, visited);
    }

    bool isConnected() const {
        vector<bool> visited(vertex_count, false);
        int start = 0;
        dfs(start, visited);
        for (int i = 0; i < vertex_count; i++) {
            if (!visited[i]) {
                bool isolated = true;
                for (int j = 0; j < vertex_count; j++)
                    if (matrix[i][j] != 0 || matrix[j][i] != 0) { isolated = false; break; }
                if (!isolated) return false;
            }
        }
        return true;
    }

    bool hamiltonianUtil(int v, vector<bool>& visited, int pos) const {
        if (pos == vertex_count) {
            for (auto& p : lists[v]) if (p.first == 0) return true;
            return false;
        }
        for (auto& p : lists[v]) {
            if (!visited[p.first]) {
                visited[p.first] = true;
                if (hamiltonianUtil(p.first, visited, pos + 1)) return true;
                visited[p.first] = false;
            }
        }
        return false;
    }

public:
    NetworkGraph(int n, bool directed = false) : vertex_count(n), is_directed(directed) {
        matrix.assign(n, vector<int>(n, 0));
        lists.resize(n);
    }

    int  getVertexCount() const { return vertex_count; }
    bool getDirected()    const { return is_directed; }
    const vector<vector<int>>&           getMatrix() const { return matrix; }
    const vector<list<pair<int,int>>>&   getLists()  const { return lists; }

    void insert_edge(int from, int to, int weight) {
        matrix[from][to] = weight;
        lists[from].push_back({to, weight});
        if (!is_directed) {
            matrix[to][from] = weight;
            lists[to].push_back({from, weight});
        }
    }

    void delete_edge(int from, int to) {
        matrix[from][to] = 0;
        lists[from].remove_if([to](const pair<int,int>& p){ return p.first == to; });
        if (!is_directed) {
            matrix[to][from] = 0;
            lists[to].remove_if([from](const pair<int,int>& p){ return p.first == from; });
        }
    }

    bool edge_exists(int from, int to) const { return matrix[from][to] != 0; }
    int  edge_weight(int from, int to) const { return matrix[from][to]; }

    int getDegree(int v) const {
        int deg = 0;
        for (int j = 0; j < vertex_count; j++) if (matrix[v][j] != 0) deg++;
        return deg;
    }

    int countEdges() const {
        int edges = 0;
        for (int i = 0; i < vertex_count; i++)
            for (int j = (is_directed ? 0 : i+1); j < vertex_count; j++)
                if (matrix[i][j] != 0) edges++;
        return edges;
    }

    bool has_euler_circuit() const {
        if (!isConnected()) return false;
        for (int i = 0; i < vertex_count; i++) if (getDegree(i) % 2 != 0) return false;
        return true;
    }

    bool has_euler_path() const {
        if (!isConnected()) return false;
        int odd = 0;
        for (int i = 0; i < vertex_count; i++) if (getDegree(i) % 2 != 0) odd++;
        return odd == 0 || odd == 2;
    }

    bool contains_hamiltonian_cycle() const {
        vector<bool> visited(vertex_count, false);
        visited[0] = true;
        return hamiltonianUtil(0, visited, 1);
    }

    bool is_complete_graph() const {
        for (int i = 0; i < vertex_count; i++)
            for (int j = 0; j < vertex_count; j++)
                if (i != j && matrix[i][j] == 0) return false;
        return true;
    }

    int connected_component_count() const {
        vector<bool> visited(vertex_count, false);
        int comp = 0;
        for (int i = 0; i < vertex_count; i++) { if (!visited[i]) { dfs(i, visited); comp++; } }
        return comp;
    }

    void generate_property_report() const {
        cout << "\nNETWORK GRAPH PROPERTIES\n";
        cout << "========================\n";
        cout << "Total Vertices: " << vertex_count << "\n";
        cout << "Total Edges: "    << countEdges()  << "\n";
        cout << "Graph Type: "     << (is_directed ? "DIRECTED" : "UNDIRECTED") << "\n";
        cout << "Euler Circuit Exists: "     << (has_euler_circuit()          ? "TRUE" : "FALSE") << "\n";
        cout << "Euler Path Exists: "        << (has_euler_path()             ? "TRUE" : "FALSE") << "\n";
        cout << "Hamiltonian Cycle Exists: " << (contains_hamiltonian_cycle() ? "TRUE" : "FALSE") << "\n";
        cout << "Complete Graph: "           << (is_complete_graph()          ? "TRUE" : "FALSE") << "\n";
        cout << "Connected Components: "     << connected_component_count()   << "\n";
    }
};

// ============================================================
//  MODULE 2.2 : Kruskal's MST
// ============================================================
struct SpanningTreeResult {
    vector<pair<int,int>> tree_edges;
    int  total_cost;
    bool success;
};

class UnionFind {
    vector<int> parent, rnk;
public:
    UnionFind(int n) : parent(n), rnk(n, 0) { for (int i = 0; i < n; i++) parent[i] = i; }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        if (rnk[px] < rnk[py]) swap(px, py);
        parent[py] = px;
        if (rnk[px] == rnk[py]) rnk[px]++;
        return true;
    }

    bool connected(int x, int y) { return find(x) == find(y); }
};

SpanningTreeResult kruskals_mst(
    const NetworkGraph&   network,
    const DiscreteSet<int>& mandatory_vertices,
    int root_vertex
) {
    SpanningTreeResult result;
    result.total_cost = 0;
    result.success    = false;

    int n = network.getVertexCount();
    const auto& mat = network.getMatrix();

    // Extract edges from adjacency matrix
    vector<tuple<int,int,int>> edges; // weight, u, v
    for (int i = 0; i < n; i++)
        for (int j = i+1; j < n; j++)
            if (mat[i][j] != 0) edges.push_back({mat[i][j], i, j});

    sort(edges.begin(), edges.end());

    UnionFind uf(n);
    for (auto& [w, u, v] : edges) {
        if (uf.unite(u, v)) {
            result.tree_edges.push_back({u, v});
            result.total_cost += w;
        }
    }

    // Verify all mandatory vertices are reachable from root
    for (int i = 0; i < mandatory_vertices.size(); i++) {
        if (!uf.connected(root_vertex, mandatory_vertices.get(i))) {
            result.success = false;
            return result;
        }
    }
    result.success = true;
    return result;
}

// ============================================================
//  MODULE 2.3 : Dijkstra's Shortest Path
// ============================================================
struct ShortestPathResult {
    vector<int> vertex_sequence;
    int  path_cost;
    bool found;
};

ShortestPathResult dijkstra_path(
    const NetworkGraph& network,
    int source_vertex,
    int target_vertex
) {
    ShortestPathResult result;
    result.found     = false;
    result.path_cost = INT_MAX;

    int n = network.getVertexCount();
    const auto& lists = network.getLists();

    vector<int> dist(n, INT_MAX);
    vector<int> prev(n, -1);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;

    dist[source_vertex] = 0;
    pq.push({0, source_vertex});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto& [v, w] : lists[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[target_vertex] == INT_MAX) return result;

    result.found     = true;
    result.path_cost = dist[target_vertex];

    for (int at = target_vertex; at != -1; at = prev[at])
        result.vertex_sequence.push_back(at);
    reverse(result.vertex_sequence.begin(), result.vertex_sequence.end());
    return result;
}

// ============================================================
//  MODULE 3.1 : BinaryRelation
// ============================================================
class BinaryRelation {
private:
    DiscreteSet<int>        base_set;
    set<pair<int,int>>      relation_pairs;

public:
    BinaryRelation() : base_set() {}
    BinaryRelation(const DiscreteSet<int>& base) : base_set(base) {}

    void insert_pair(int x, int y) { relation_pairs.insert({x, y}); }
    void remove_pair(int x, int y) { relation_pairs.erase({x, y}); }
    bool has_pair(int x, int y)  const { return relation_pairs.count({x, y}) > 0; }

    bool test_reflexive() const {
        for (int i = 0; i < base_set.size(); i++) { int a = base_set.get(i); if (!has_pair(a,a)) return false; }
        return true;
    }

    bool test_symmetric() const {
        for (auto& [a,b] : relation_pairs) if (!has_pair(b,a)) return false;
        return true;
    }

    bool test_antisymmetric() const {
        for (auto& [a,b] : relation_pairs) if (a != b && has_pair(b,a)) return false;
        return true;
    }

    bool test_transitive() const {
        for (auto& [a,b] : relation_pairs)
            for (auto& [c,d] : relation_pairs)
                if (b == c && !has_pair(a,d)) return false;
        return true;
    }

    bool is_equivalence()   const { return test_reflexive() && test_symmetric()     && test_transitive(); }
    bool is_partial_order() const { return test_reflexive() && test_antisymmetric() && test_transitive(); }

    bool is_total_order() const {
        if (!is_partial_order()) return false;
        for (int i = 0; i < base_set.size(); i++)
            for (int j = 0; j < base_set.size(); j++) {
                int a = base_set.get(i), b = base_set.get(j);
                if (!has_pair(a,b) && !has_pair(b,a)) return false;
            }
        return true;
    }

    BinaryRelation compute_reflexive_closure() const {
        BinaryRelation r(base_set); r.relation_pairs = relation_pairs;
        for (int i = 0; i < base_set.size(); i++) r.insert_pair(base_set.get(i), base_set.get(i));
        return r;
    }

    BinaryRelation compute_symmetric_closure() const {
        BinaryRelation r(base_set); r.relation_pairs = relation_pairs;
        for (auto& [a,b] : relation_pairs) r.insert_pair(b,a);
        return r;
    }

    BinaryRelation compute_transitive_closure() const {
        // Floyd-Warshall
        int n = base_set.size();
        vector<vector<bool>> reach(n, vector<bool>(n, false));
        for (int i = 0; i < n; i++) reach[i][i] = true;
        for (auto& [a,b] : relation_pairs) {
            int ai=-1, bi=-1;
            for (int i = 0; i < n; i++) { if (base_set.get(i)==a) ai=i; if (base_set.get(i)==b) bi=i; }
            if (ai!=-1 && bi!=-1) reach[ai][bi] = true;
        }
        for (int k = 0; k < n; k++)
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (reach[i][k] && reach[k][j]) reach[i][j] = true;

        BinaryRelation result(base_set);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (reach[i][j]) result.insert_pair(base_set.get(i), base_set.get(j));
        return result;
    }

    vector<DiscreteSet<int>> partition_equivalence_classes() const {
        vector<DiscreteSet<int>> classes;
        vector<bool> assigned(base_set.size(), false);
        for (int i = 0; i < base_set.size(); i++) {
            if (assigned[i]) continue;
            DiscreteSet<int> cls; cls.add(base_set.get(i)); assigned[i] = true;
            for (int j = i+1; j < base_set.size(); j++) {
                if (!assigned[j] && has_pair(base_set.get(i), base_set.get(j))) {
                    cls.add(base_set.get(j)); assigned[j] = true;
                }
            }
            classes.push_back(cls);
        }
        return classes;
    }

    void printAnalysis(const string& name) const {
        cout << "\nRELATION " << name << " ANALYSIS:\n";
        cout << "Reflexive: "     << (test_reflexive()     ? "TRUE" : "FALSE") << "\n";
        cout << "Symmetric: "     << (test_symmetric()     ? "TRUE" : "FALSE") << "\n";
        cout << "Transitive: "    << (test_transitive()    ? "TRUE" : "FALSE") << "\n";
        cout << "Partial Order: " << (is_partial_order()   ? "TRUE" : "FALSE") << "\n";
    }
};

// ============================================================
//  MODULE 3.2 : DiscreteFunction
// ============================================================
class DiscreteFunction {
private:
    DiscreteSet<int> input_set;
    DiscreteSet<int> output_set;
    map<int,int>     function_map;

public:
    DiscreteFunction() {}
    DiscreteFunction(const DiscreteSet<int>& inputs, const DiscreteSet<int>& outputs)
        : input_set(inputs), output_set(outputs) {}

    void set_mapping(int x, int y) {
        if (!input_set.member(x))  throw invalid_argument("x not in domain");
        if (!output_set.member(y)) throw invalid_argument("y not in codomain");
        function_map[x] = y;
    }

    bool has_mapping(int x) const { return function_map.count(x) > 0; }

    int apply(int x) const {
        if (!has_mapping(x)) throw runtime_error("Undefined at " + to_string(x));
        return function_map.at(x);
    }

    bool is_total_function() const {
        for (int i = 0; i < input_set.size(); i++) if (!has_mapping(input_set.get(i))) return false;
        return true;
    }

    bool is_injection() const {
        set<int> seen;
        for (auto& [k,v] : function_map) { if (seen.count(v)) return false; seen.insert(v); }
        return true;
    }

    bool is_surjection() const {
        for (int i = 0; i < output_set.size(); i++) {
            bool found = false;
            for (auto& [k,v] : function_map) if (v == output_set.get(i)) { found = true; break; }
            if (!found) return false;
        }
        return true;
    }

    bool is_bijection() const { return is_injection() && is_surjection(); }

    DiscreteFunction compose_with(const DiscreteFunction& g) const {
        DiscreteFunction result(g.input_set, output_set);
        for (int i = 0; i < g.input_set.size(); i++) {
            int x = g.input_set.get(i);
            if (g.has_mapping(x)) {
                int mid = g.apply(x);
                if (has_mapping(mid)) result.set_mapping(x, apply(mid));
            }
        }
        return result;
    }

    DiscreteFunction compute_inverse() const {
        if (!is_bijection()) throw runtime_error("Function is not bijective");
        DiscreteFunction inv(output_set, input_set);
        for (auto& [k,v] : function_map) inv.set_mapping(v, k);
        return inv;
    }

    DiscreteSet<int> compute_image() const {
        DiscreteSet<int> img;
        for (auto& [k,v] : function_map) img.add(v);
        return img;
    }

    DiscreteSet<int> compute_preimage(int y) const {
        DiscreteSet<int> pre;
        for (auto& [k,v] : function_map) if (v == y) pre.add(k);
        return pre;
    }
};

// ============================================================
//  MODULE 3.3 : CDAFController  (menu-driven)
// ============================================================
class CDAFController {
private:
    NetworkGraph*                   infrastructure;
    vector<BinaryRelation>          system_relations;
    vector<DiscreteFunction>        system_functions;
    map<string, DiscreteSet<int>>   named_collections;
    vector<string>                  relation_names;
    vector<string>                  function_names;
    bool                            configLoaded;

    // ---- helpers ----
    DiscreteSet<int> parseSet(const string& s) {
        DiscreteSet<int> result;
        string inner = s.substr(1, s.size()-2);
        stringstream ss(inner);
        string token;
        while (getline(ss, token, ',')) if (!token.empty()) result.add(stoi(token));
        return result;
    }

public:
    CDAFController() : infrastructure(nullptr), configLoaded(false) {}
    ~CDAFController() { delete infrastructure; }

    // ---- load config ----
    void load_system_config(const string& config_file) {
        ifstream fin(config_file);
        if (!fin) { cerr << "Cannot open file: " << config_file << "\n"; return; }

        string line, section;
        int vertices = 0; bool directed = false;
        vector<tuple<int,int,int>> edges;

        struct RelData { string name, base; vector<pair<int,int>> pairs; };
        vector<RelData> relList;
        RelData curRel; bool inRel = false;

        struct FuncData { string name, dom, codom; vector<pair<int,int>> maps; };
        vector<FuncData> funcList;
        FuncData curFunc; bool inFunc = false;

        while (getline(fin, line)) {
            if (line.empty()) continue;
            if (line == "COLLECTIONS") { section="COLLECTIONS"; continue; }
            if (line == "NETWORK")     { section="NETWORK";     continue; }
            if (line == "RELATIONS")   { section="RELATIONS";   continue; }
            if (line == "FUNCTIONS")   { section="FUNCTIONS";   continue; }

            if (section == "COLLECTIONS") {
                size_t c = line.find(':');
                named_collections[line.substr(0,c)] = parseSet(line.substr(c+1));
            }
            else if (section == "NETWORK") {
                if      (line.find("vertices:") == 0) vertices = stoi(line.substr(9));
                else if (line.find("directed:") == 0) directed = (line.substr(9)=="true");
                else if (line == "connections:") {}
                else {
                    stringstream ss(line); string t; vector<int> nums;
                    while (getline(ss,t,',')) nums.push_back(stoi(t));
                    if (nums.size()==3) edges.push_back({nums[0],nums[1],nums[2]});
                }
            }
            else if (section == "RELATIONS") {
                if (line.find("pairs:") == 0) {
                    string inner = line.substr(6);
                    stringstream ss(inner); string tok;
                    while (getline(ss,tok,')')) {
                        if (tok.empty()||tok==",") continue;
                        if (tok[0]==',') tok=tok.substr(1);
                        if (tok[0]=='(') tok=tok.substr(1);
                        size_t comma = tok.find(',');
                        if (comma==string::npos) continue;
                        curRel.pairs.push_back({stoi(tok.substr(0,comma)), stoi(tok.substr(comma+1))});
                    }
                } else {
                    if (inRel) relList.push_back(curRel);
                    curRel = RelData(); inRel = true;
                    size_t c = line.find(':');
                    curRel.name = line.substr(0,c);
                    size_t bp = line.find("base_set:");
                    if (bp!=string::npos) curRel.base = line.substr(bp+9);
                }
            }
            else if (section == "FUNCTIONS") {
                if (line.find("maps:") == 0) {
                    string mstr = line.substr(5);
                    stringstream ss(mstr); string tok;
                    while (getline(ss,tok,',')) {
                        size_t arrow = tok.find("->");
                        if (arrow==string::npos) continue;
                        curFunc.maps.push_back({stoi(tok.substr(0,arrow)), stoi(tok.substr(arrow+2))});
                    }
                } else {
                    if (inFunc) funcList.push_back(curFunc);
                    curFunc = FuncData(); inFunc = true;
                    size_t c = line.find(':');
                    curFunc.name = line.substr(0,c);
                    size_t ip = line.find("input:");
                    size_t op = line.find(":output:");
                    if (ip!=string::npos && op!=string::npos) {
                        curFunc.dom   = line.substr(ip+6, op-(ip+6));
                        curFunc.codom = line.substr(op+8);
                    }
                }
            }
        }
        if (inRel)  relList.push_back(curRel);
        if (inFunc) funcList.push_back(curFunc);

        // Build graph
        delete infrastructure;
        infrastructure = new NetworkGraph(vertices, directed);
        for (auto& [u,v,w] : edges) infrastructure->insert_edge(u,v,w);

        // Build relations
        system_relations.clear(); relation_names.clear();
        for (auto& rd : relList) {
            DiscreteSet<int> base;
            if (named_collections.count(rd.base)) base = named_collections[rd.base];
            BinaryRelation br(base);
            for (auto& [a,b] : rd.pairs) br.insert_pair(a,b);
            system_relations.push_back(br);
            relation_names.push_back(rd.name);
        }

        // Build functions
        system_functions.clear(); function_names.clear();
        for (auto& fd : funcList) {
            DiscreteSet<int> dom, codom;
            if (named_collections.count(fd.dom))   dom   = named_collections[fd.dom];
            if (named_collections.count(fd.codom)) codom = named_collections[fd.codom];
            DiscreteFunction df(dom, codom);
            for (auto& [a,b] : fd.maps) try { df.set_mapping(a,b); } catch (...) {}
            system_functions.push_back(df);
            function_names.push_back(fd.name);
        }

        configLoaded = true;
        cout << "Configuration loaded successfully from: " << config_file << "\n";
    }

    // ---- individual analysis functions ----
    void run_set_analysis() {
        if (!named_collections.count("critical_infrastructure") ||
            !named_collections.count("high_traffic") ||
            !named_collections.count("redundancy_nodes")) {
            cout << "Required sets (critical_infrastructure / high_traffic / redundancy_nodes) not found.\n";
            return;
        }
        auto& ci = named_collections["critical_infrastructure"];
        auto& ht = named_collections["high_traffic"];
        auto& rn = named_collections["redundancy_nodes"];

        DiscreteSet<int> uni       = ci.set_union(ht);
        DiscreteSet<int> mandatory = GraphConstraintBuilder::compute_mandatory_nodes(ci, ht, rn);

        cout << "\nSET OPERATIONS:\n";
        cout << "(critical_infrastructure U high_traffic) = " << uni << "\n";
        cout << "(critical_infrastructure U high_traffic) n redundancy_nodes = " << mandatory << "\n";
        cout << "Mandatory Nodes = " << mandatory << "\n";
    }

    void run_graph_analysis() {
        if (!infrastructure) { cout << "No graph loaded.\n"; return; }
        infrastructure->generate_property_report();
    }

    void run_relation_analysis() {
        if (system_relations.empty()) { cout << "No relations loaded.\n"; return; }
        for (int i = 0; i < (int)system_relations.size(); i++)
            system_relations[i].printAnalysis(relation_names[i]);
    }

    void find_optimal_paths() {
        if (!infrastructure) { cout << "No graph loaded.\n"; return; }
        int n = infrastructure->getVertexCount();
        int src, tgt;
        cout << "Enter source vertex (0-" << n-1 << "): "; cin >> src;
        cout << "Enter target vertex (0-" << n-1 << "): "; cin >> tgt;

        ShortestPathResult sp = dijkstra_path(*infrastructure, src, tgt);
        cout << "\nSHORTEST PATH (" << src << " to " << tgt << "):\n";
        if (!sp.found) {
            cout << "SHORTEST PATH: [NO PATH]\n";
            cout << "TOTAL COST: INFINITY\n";
        } else {
            cout << "SHORTEST PATH: [";
            for (int i = 0; i < (int)sp.vertex_sequence.size(); i++) {
                if (i > 0) cout << "->";
                cout << sp.vertex_sequence[i];
            }
            cout << "]\nTOTAL COST: " << sp.path_cost << "\n";
        }
    }

    void build_constrained_mst() {
        if (!infrastructure) { cout << "No graph loaded.\n"; return; }

        DiscreteSet<int> mandatory;
        if (named_collections.count("critical_infrastructure") &&
            named_collections.count("high_traffic") &&
            named_collections.count("redundancy_nodes")) {
            mandatory = GraphConstraintBuilder::compute_mandatory_nodes(
                named_collections["critical_infrastructure"],
                named_collections["high_traffic"],
                named_collections["redundancy_nodes"]
            );
        }

        SpanningTreeResult mst = kruskals_mst(*infrastructure, mandatory, 0);
        const auto& mat = infrastructure->getMatrix();

        cout << "\nMINIMUM SPANNING TREE (Kruskal's):\n";
        cout << "Root Vertex: 0\n";
        cout << "Mandatory Vertices: " << mandatory << "\n";
        int totalW = 0;
        for (auto& [u,v] : mst.tree_edges) {
            int w = mat[u][v];
            cout << "Edge (" << u << ", " << v << "): Weight " << w << "\n";
            totalW += w;
        }
        cout << "TOTAL COST: " << totalW << "\n";
        cout << "RESULT: " << (mst.success ? "SUCCESS" : "FAILED") << "\n";
    }

    void map_relation_onto_graph(const BinaryRelation& R) {
        if (!infrastructure) { cout << "No graph loaded.\n"; return; }
        cout << "\nMapping relation onto graph — edges that satisfy the relation:\n";
        int n = infrastructure->getVertexCount();
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (infrastructure->edge_exists(i,j) && R.has_pair(i,j))
                    cout << "  Edge (" << i << "," << j << ") satisfies relation\n";
    }

    // ---- full pipeline ----
    void execute_full_analysis() {
        cout << "=== CDAF ANALYSIS REPORT ===\n\n";
        run_set_analysis();

        if (!infrastructure) { cout << "No graph.\n"; return; }

        // Network Analysis
        cout << "\nNETWORK ANALYSIS:\n";
        cout << "Total Vertices: " << infrastructure->getVertexCount() << "\n";
        cout << "Total Edges: "    << infrastructure->countEdges()     << "\n";
        cout << "Euler Circuit Exists: "     << (infrastructure->has_euler_circuit()          ? "TRUE":"FALSE") << "\n";
        cout << "Hamiltonian Cycle Exists: " << (infrastructure->contains_hamiltonian_cycle() ? "TRUE":"FALSE") << "\n";
        cout << "Complete Graph: "           << (infrastructure->is_complete_graph()           ? "TRUE":"FALSE") << "\n";

        // MST
        build_constrained_mst();

        // Shortest path 0 -> last
        int last = infrastructure->getVertexCount() - 1;
        ShortestPathResult sp = dijkstra_path(*infrastructure, 0, last);
        cout << "\nSHORTEST PATH (0 to " << last << "):\n";
        if (!sp.found) {
            cout << "SHORTEST PATH: [NO PATH]\nTOTAL COST: INFINITY\n";
        } else {
            cout << "SHORTEST PATH: [";
            for (int i = 0; i < (int)sp.vertex_sequence.size(); i++) {
                if (i > 0) cout << "->";
                cout << sp.vertex_sequence[i];
            }
            cout << "]\nTOTAL COST: " << sp.path_cost << "\n";
        }

        // Relations
        run_relation_analysis();

        // Predicate check
        DiscreteSet<int> nodeSet;
        for (int i = 0; i < infrastructure->getVertexCount(); i++) nodeSet.add(i);
        LogicalPredicate<int> highDeg = [this](const int& v) -> bool {
            return infrastructure->getDegree(v) > 4;
        };
        bool anyHigh = PredicateEngine::exists<int>(nodeSet, highDeg);
        cout << "\nPREDICATE CHECK:\n";
        cout << "Exists node with degree > 4: " << (anyHigh ? "TRUE" : "FALSE") << "\n";

        cout << "\n=== END REPORT ===\n";
    }

    void write_results(const string& output_file) {
        cout << "Results written to: " << output_file << "\n";
    }

    bool isLoaded() const { return configLoaded; }

    // ---- interactive set operations ----
    void interactiveSetOps() {
        cout << "\n--- Interactive Set Operations ---\n";
        cout << "Available collections:\n";
        for (auto& [k,v] : named_collections) cout << "  " << k << " = " << v << "\n";

        string a, b;
        cout << "Enter name of set A: "; cin >> a;
        cout << "Enter name of set B: "; cin >> b;
        if (!named_collections.count(a) || !named_collections.count(b)) {
            cout << "One or both sets not found.\n"; return;
        }
        auto& A = named_collections[a];
        auto& B = named_collections[b];
        cout << "Union        : " << A.set_union(B)        << "\n";
        cout << "Intersection : " << A.set_intersection(B) << "\n";
        cout << "Difference   : " << A.set_difference(B)   << "\n";
        cout << "Sym Diff     : " << A.symmetric_diff(B)   << "\n";
        cout << "A subset B?  : " << (A.is_subset(B) ? "YES":"NO") << "\n";
    }

    // ---- menu ----
    void showMenu() {
        cout << "\n========================================\n";
        cout << "  CDAF - Computational Discrete Analysis\n";
        cout << "========================================\n";
        cout << "1. Load Configuration File\n";
        cout << "2. Run Full Analysis (All Modules)\n";
        cout << "3. Set Operations Only\n";
        cout << "4. Graph Property Analysis\n";
        cout << "5. Find Shortest Path (Dijkstra)\n";
        cout << "6. Build Constrained MST (Kruskal's)\n";
        cout << "7. Relation Analysis\n";
        cout << "8. Interactive Set Operations\n";
        cout << "9. Exit\n";
        cout << "Enter choice: ";
    }

    void run() {
        int choice;
        do {
            showMenu();
            cin >> choice;
            switch (choice) {
                case 1: {
                    string fname;
                    cout << "Enter config filename: "; cin >> fname;
                    load_system_config(fname);
                    break;
                }
                case 2:
                    if (!isLoaded()) cout << "Load config first (option 1).\n";
                    else execute_full_analysis();
                    break;
                case 3:
                    if (!isLoaded()) cout << "Load config first (option 1).\n";
                    else run_set_analysis();
                    break;
                case 4:
                    if (!isLoaded()) cout << "Load config first (option 1).\n";
                    else run_graph_analysis();
                    break;
                case 5:
                    if (!isLoaded()) cout << "Load config first (option 1).\n";
                    else find_optimal_paths();
                    break;
                case 6:
                    if (!isLoaded()) cout << "Load config first (option 1).\n";
                    else build_constrained_mst();
                    break;
                case 7:
                    if (!isLoaded()) cout << "Load config first (option 1).\n";
                    else run_relation_analysis();
                    break;
                case 8:
                    if (!isLoaded()) cout << "Load config first (option 1).\n";
                    else interactiveSetOps();
                    break;
                case 9:
                    cout << "Exiting CDAF. Goodbye!\n";
                    break;
                default:
                    cout << "Invalid choice.\n";
            }
        } while (choice != 9);
    }
};

// ============================================================
//  MAIN
// ============================================================
void createTestInputB() {
    ofstream f("test_input_b.txt");
    f << "COLLECTIONS\n";
    f << "universe:{0,1,2,3,4,5,6,7}\n";
    f << "critical_infrastructure:{1,3,5}\n";
    f << "high_traffic:{2,3,4}\n";
    f << "redundancy_nodes:{3,5}\n\n";
    f << "NETWORK\n";
    f << "vertices:8\n";
    f << "directed:false\n";
    f << "connections:\n";
    f << "0,1,4\n0,2,3\n1,2,2\n1,3,6\n2,3,5\n2,4,4\n";
    f << "3,4,2\n3,5,8\n4,5,3\n5,6,5\n6,7,6\n\n";
    f << "RELATIONS\n";
    f << "R1:base_set:universe\n";
    f << "pairs:(0,1),(0,2),(0,3),(0,4),(0,5),(5,6),(3,5)\n\n";
    f << "FUNCTIONS\n";
    f << "F1:input:universe:output:universe\n";
    f << "maps:0->1,1->2,2->3,3->4,4->5\n";
    f.close();
    cout << "Test file 'test_input_b.txt' created.\n";
}

int main() {
    createTestInputB();
    CDAFController cdaf;
    cdaf.run();
    return 0;
}
