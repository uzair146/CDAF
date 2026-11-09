#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
//  ANSI COLOR CODES
#define RESET        "\033[0m"
#define RED          "\033[31m"
#define GREEN        "\033[32m"
#define YELLOW       "\033[33m"
#define BLUE         "\033[34m"
#define MAGENTA      "\033[35m"
#define CYAN         "\033[36m"
#define WHITE        "\033[37m"
#define BOLD_RED     "\033[1;31m"
#define BOLD_GREEN   "\033[1;32m"
#define BOLD_YELLOW  "\033[1;33m"
#define BOLD_BLUE    "\033[1;34m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_CYAN    "\033[1;36m"
// Print TRUE in green, FALSE in red
void printBool(bool val) {
    if (val) {
        cout << BOLD_GREEN << "TRUE" << RESET;
    }
    else {
        cout << BOLD_RED << "FALSE" << RESET;
    }
}
//  CUSTOM DYNAMIC ARRAY
template <typename T>
class DynArray {
private:
    T*  data;
    int cap;
    int sz;
    // Double the capacity when full
    void resize() {
        cap = cap * 2;
        T* newData = new T[cap];
        for (int i = 0; i < sz; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }
public:
    DynArray() : cap(8), sz(0) {
        data = new T[cap];
    }
    DynArray(const DynArray<T>& other) : cap(other.cap), sz(other.sz) {
        data = new T[cap];
        for (int i = 0; i < sz; i++) {
            data[i] = other.data[i];
        }
    }
    DynArray<T>& operator=(const DynArray<T>& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data;
        cap  = other.cap;
        sz   = other.sz;
        data = new T[cap];
        for (int i = 0; i < sz; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }
    ~DynArray() {
        delete[] data;
    }
    // Add element at end
    void push(const T& val) {
        if (sz >= cap) {
            resize();
        }
        data[sz] = val;
        sz++;
    }
    // Remove element at index
    void removeAt(int idx) {
        for (int i = idx; i < sz - 1; i++) {
            data[i] = data[i + 1];
        }
        sz--;
    }
    T& get(int i) { 
        return data[i]; 
    }
    const T& get(int i) const {
        return data[i]; 
    }
    int size() const { 
        return sz; 
    }
    bool empty() const { 
        return sz == 0; 
    }
    void clear() {
        sz = 0;
    }
};
//  CUSTOM LINKED LIST NODE
template <typename T>
struct ListNode {
    T  data;
    ListNode* next;
    ListNode(const T& val) : data(val), next(NULL) {
        
    }
};
//  CUSTOM SINGLY LINKED LIST
//  Used for adjacency list
template <typename T>
class LinkedList {
private:
    ListNode<T>* head;
    int  sz;
public:
    LinkedList() : head(NULL), sz(0) {
        
    }
    LinkedList(const LinkedList<T>& other) : head(NULL), sz(0) {
        ListNode<T>* cur = other.head;
        while (cur != NULL) {
            push(cur->data);
            cur = cur->next;
        }
    }
    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this == &other) {
            return *this;
        }
        clear();
        ListNode<T>* cur = other.head;
        while (cur != NULL) {
            push(cur->data);
            cur = cur->next;
        }
        return *this;
    }
    ~LinkedList() {
        clear();
    }
    // Add at end
    void push(const T& val) {
        ListNode<T>* node = new ListNode<T>(val);
        if (head == NULL) {
            head = node;
        }
        else {
            ListNode<T>* cur = head;
            while (cur->next != NULL) {
                cur = cur->next;
            }
            cur->next = node;
        }
        sz++;
    }
    // Remove node where data matches val
    void remove(const T& val) {
        while (head != NULL && head->data == val) {
            ListNode<T>* tmp = head;
            head = head->next;
            delete tmp;
            sz--;
        }
        ListNode<T>* cur = head;
        while (cur != NULL && cur->next != NULL) {
            if (cur->next->data == val) {
                ListNode<T>* tmp = cur->next;
                cur->next = tmp->next;
                delete tmp;
                sz--;
            }
            else {
                cur = cur->next;
            }
        }
    }
    ListNode<T>* getHead() const { 
        return head;
    }
    int size() const { 
        return sz; 
    }
    bool empty() const {
        return sz == 0;
    }
    void clear() {
        while (head != NULL) {
            ListNode<T>* tmp = head;
            head = head->next;
            delete tmp;
        }
        sz = 0;
    }
};
//  CUSTOM PAIR
template <typename A, typename B>
struct Pair {
    A first;
    B second;
    Pair() {
        
    }
    Pair(const A& a, const B& b) : first(a), second(b) {
        
    }
    bool operator==(const Pair<A,B>& other) const {
        return first == other.first && second == other.second;
    }
    bool operator<(const Pair<A,B>& other) const {
        if (first != other.first) {
            return first < other.first;
        }
        return second < other.second;
    }
};
//  CUSTOM MIN-HEAP (priority queue)
//  Used for Dijkstra
class MinHeap {
private:
    DynArray< Pair<int,int> > data;  // stores (distance, vertex)
    // Move node up to fix heap property
    void heapifyUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data.get(parent).first > data.get(i).first) {
                Pair<int,int> tmp = data.get(parent);
                data.get(parent) = data.get(i);
                data.get(i) = tmp;
                i = parent;
            }
            else {
                break;
            }
        }
    }
    // Move node down to fix heap property
    void heapifyDown(int i) {
        int n = data.size();
        while (true) {
            int left  = 2 * i + 1;
            int right = 2 * i + 2;
            int small = i;
            if (left < n && data.get(left).first < data.get(small).first) {
                small = left;
            }
            if (right < n && data.get(right).first < data.get(small).first) {
                small = right;
            }
            if (small == i) {
                break;
            }
            Pair<int,int> tmp = data.get(i);
            data.get(i)     = data.get(small);
            data.get(small) = tmp;
            i = small;
        }
    }
public:
    // Push (distance, vertex) pair
    void push(int dist, int vertex) {
        data.push(Pair<int,int>(dist, vertex));
        heapifyUp(data.size() - 1);
    }
    // Get min element
    Pair<int,int> top() const {
        return data.get(0);
    }
    // Remove min element
    void pop() {
        int last = data.size() - 1;
        data.get(0) = data.get(last);
        data.removeAt(last);
        if (data.size() > 0) {
            heapifyDown(0);
        }
    }
    bool empty() const {
        return data.empty();
    }
};
//  MODULE 1.1 : DiscreteSet<T>
template <typename T>
class DiscreteSet {
private:
    DynArray<T> data;
    // Binary search - returns index or -1
    int binarySearch(const T& val) const {
        int lo = 0;
        int hi = data.size() - 1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (data.get(mid) == val) {
                return mid;
            }
            else if (data.get(mid) < val) {
                lo = mid + 1;
            }
            else {
                hi = mid - 1;
            }
        }
        return -1;
    }
public:
    DiscreteSet() {
        
    }
    // Add element in sorted order, no duplicates
    void add(const T& element) {
        if (member(element)) {
            return;
        }
        int i = data.size() - 1;
        data.push(element);
        while (i >= 0 && data.get(i) > element) {
            data.get(i + 1) = data.get(i);
            i--;
        }
        data.get(i + 1) = element;
    }
    // Remove element if exists
    void delete_element(const T& element) {
        int idx = binarySearch(element);
        if (idx == -1) {
            return;
        }
        data.removeAt(idx);
    }
    bool member(const T& element)  const { 
        return binarySearch(element) != -1;
    }
    int  size() const { 
        return data.size(); 
    }
    bool empty() const { 
        return data.empty();
    }
    void clear_all() { 
        data.clear(); 
    }
    T  get(int idx) const {
        return data.get(idx); 
    }
    // Check if two sets are equal
    bool equals(const DiscreteSet<T>& other) const {
        if (data.size() != other.data.size()) {
            return false;
        }
        for (int i = 0; i < data.size(); i++) {
            if (data.get(i) != other.data.get(i)) {
                return false;
            }
        }
        return true;
    }
    // Returns new set: all elements from both
    DiscreteSet<T> set_union(const DiscreteSet<T>& other) const {
        DiscreteSet<T> result = *this;
        for (int i = 0; i < other.size(); i++) {
            result.add(other.get(i));
        }
        return result;
    }
    // Returns new set: only common elements
    DiscreteSet<T> set_intersection(const DiscreteSet<T>& other) const {
        DiscreteSet<T> result;
        for (int i = 0; i < data.size(); i++) {
            if (other.member(data.get(i))) {
                result.add(data.get(i));
            }
        }
        return result;
    }
    // Returns new set: elements in this but not in other
    DiscreteSet<T> set_difference(const DiscreteSet<T>& other) const {
        DiscreteSet<T> result;
        for (int i = 0; i < data.size(); i++) {
            if (!other.member(data.get(i))) {
                result.add(data.get(i));
            }
        }
        return result;
    }
    // Returns new set: elements in one but not both
    DiscreteSet<T> symmetric_diff(const DiscreteSet<T>& other) const {
        DiscreteSet<T> onlyA = set_difference(other);
        DiscreteSet<T> onlyB = other.set_difference(*this);
        return onlyA.set_union(onlyB);
    }
    // Check if every element of this exists in other
    bool is_subset(const DiscreteSet<T>& other) const {
        for (int i = 0; i < data.size(); i++) {
            if (!other.member(data.get(i))) {
                return false;
            }
        }
        return true;
    }
    // Subset but not equal
    bool is_strict_subset(const DiscreteSet<T>& other) const {
        return is_subset(other) && data.size() < other.size();
    }
    // Print set in {a,b,c} format
    void print() const {
        cout << "{";
        for (int i = 0; i < data.size(); i++) {
            if (i > 0) {
                cout << ",";
            }
            cout << data.get(i);
        }
        cout << "}";
    }
};
//  POWERSET
//  Stores all subsets of a DiscreteSet<int>
class PowerSet {
private:
    DynArray< DiscreteSet<int> > subsets;
    // Recursively build all subsets
    void buildHelper(const DiscreteSet<int>& src, int index, DiscreteSet<int> current) {
        if (index == src.size()) {
            subsets.push(current);
            return;
        }
        // Exclude element at index
        buildHelper(src, index + 1, current);
        // Include element at index
        current.add(src.get(index));
        buildHelper(src, index + 1, current);
    }
public:
    PowerSet() {
        
    }
    void build(const DiscreteSet<int>& src) {
        subsets.clear();
        DiscreteSet<int> empty;
        buildHelper(src, 0, empty);
    }
    int size() const { 
        return subsets.size();
    }
    DiscreteSet<int> get(int i) const { 
        return subsets.get(i); 
    }
    void print() const {
        cout << "{ ";
        for (int i = 0; i < subsets.size(); i++) {
            subsets.get(i).print();
            if (i < subsets.size() - 1) {
                cout << ", ";
            }
        }
        cout << " }";
    }
};
//  CARTESIAN PRODUCT
//  Stores ordered pairs from two DiscreteSet<int>
class CartesianProduct {
private:
    DynArray< Pair<int,int> > pairs;
public:
    CartesianProduct() {
        
    }
    void build(const DiscreteSet<int>& A, const DiscreteSet<int>& B) {
        pairs.clear();
        for (int i = 0; i < A.size(); i++) {
            for (int j = 0; j < B.size(); j++) {
                pairs.push(Pair<int,int>(A.get(i), B.get(j)));
            }
        }
    }
    int  size() const { 
        return pairs.size(); 
    }
    Pair<int,int> get(int i) const { 
        return pairs.get(i);
    }
    void print() const {
        cout << "{ ";
        for (int i = 0; i < pairs.size(); i++) {
            cout << "(" << pairs.get(i).first << "," << pairs.get(i).second << ")";
            if (i < pairs.size() - 1) {
                cout << ", ";
            }
        }
        cout << " }";
    }
};
//  MODULE 1.2 : PredicateEngine
class PredicateEngine {
public:
    // Returns true if P(x) holds for every element
    static bool forall(const DiscreteSet<int>& universe, bool (*P)(int)) {
        for (int i = 0; i < universe.size(); i++) {
            if (!P(universe.get(i))) {
                return false;
            }
        }
        return true;
    }
    // Returns true if P(x) holds for at least one element
    static bool exists(const DiscreteSet<int>& universe, bool (*P)(int)) {
        for (int i = 0; i < universe.size(); i++) {
            if (P(universe.get(i))) {
                return true;
            }
        }
        return false;
    }
    // Returns true if P(x) holds for exactly one element
    static bool exists_unique(const DiscreteSet<int>& universe, bool (*P)(int)) {
        int count = 0;
        for (int i = 0; i < universe.size(); i++) {
            if (P(universe.get(i))) {
                count++;
            }
        }
        return count == 1;
    }
    // Verify: NOT forall P  ==  exists NOT P
    static bool verify_not_forall_equals_exists_not(const DiscreteSet<int>& universe, bool (*P)(int)) {
        bool lhs = !forall(universe, P);
        bool rhs = false;
        for (int i = 0; i < universe.size(); i++) {
            if (!P(universe.get(i))) {
                rhs = true;
                break;
            }
        }
        return lhs == rhs;
    }
    // Verify: NOT exists P  ==  forall NOT P
    static bool verify_not_exists_equals_forall_not(const DiscreteSet<int>& universe, bool (*P)(int)) {
        bool lhs = !exists(universe, P);
        bool rhs = true;
        for (int i = 0; i < universe.size(); i++) {
            if (P(universe.get(i))) {
                rhs = false;
                break;
            }
        }
        return lhs == rhs;
    }
};
// Forward declaration for predicate function
bool degreeOver4(int v);
//  MODULE 1.3 : GraphConstraintBuilder
class GraphConstraintBuilder {
public:
    // Formula: (critical U high_traffic) ∩ redundancy
    static DiscreteSet<int> compute_mandatory_nodes(
        const DiscreteSet<int>& critical_infrastructure,
        const DiscreteSet<int>& high_traffic,
        const DiscreteSet<int>& redundancy_nodes)
    {
        DiscreteSet<int> uni = critical_infrastructure.set_union(high_traffic);
        return uni.set_intersection(redundancy_nodes);
    }
    // Formula: faulty U (all \ mandatory)
    static DiscreteSet<int> compute_forbidden_nodes(
        const DiscreteSet<int>& all_nodes,
        const DiscreteSet<int>& mandatory_nodes,
        const DiscreteSet<int>& faulty_nodes)
    {
        DiscreteSet<int> diff = all_nodes.set_difference(mandatory_nodes);
        return faulty_nodes.set_union(diff);
    }
    // Check if union of all coverage sets equals total universe
    static bool verify_coverage(
        const DiscreteSet<int>& total_universe,
        const DynArray< DiscreteSet<int> >& coverage_sets)
    {
        DiscreteSet<int> combined;
        for (int i = 0; i < coverage_sets.size(); i++) {
            combined = combined.set_union(coverage_sets.get(i));
        }
        return combined.equals(total_universe);
    }
};
//  MODULE 2.1 : NetworkGraph
class NetworkGraph {
private:
    int vertex_count;
    int** matrix;                           // adjacency matrix (weights)
    DynArray< LinkedList< Pair<int,int> > > lists;  // adjacency list (neighbor, weight)
    bool is_directed;
    // DFS to mark all reachable nodes
    void dfs(int v, bool* visited) const {
        visited[v] = true;
        ListNode< Pair<int,int> >* cur = lists.get(v).getHead();
        while (cur != NULL) {
            if (!visited[cur->data.first]) {
                dfs(cur->data.first, visited);
            }
            cur = cur->next;
        }
    }
    // Check if graph is connected
    bool isConnected() const {
        bool* visited = new bool[vertex_count];
        for (int i = 0; i < vertex_count; i++) {
            visited[i] = false;
        }
        dfs(0, visited);
        bool result = true;
        for (int i = 0; i < vertex_count; i++) {
            if (!visited[i]) {
                // Check if this node has any edges
                bool isolated = true;
                for (int j = 0; j < vertex_count; j++) {
                    if (matrix[i][j] != 0 || matrix[j][i] != 0) {
                        isolated = false;
                        break;
                    }
                }
                if (!isolated) {
                    result = false;
                    break;
                }
            }
        }
        delete[] visited;
        return result;
    }
    // Backtracking helper for Hamiltonian cycle
    bool hamiltonianUtil(int v, bool* visited, int pos) const {
        if (pos == vertex_count) {
            // Check if there is an edge back to start
            ListNode< Pair<int,int> >* cur = lists.get(v).getHead();
            while (cur != NULL) {
                if (cur->data.first == 0) {
                    return true;
                }
                cur = cur->next;
            }
            return false;
        }
        ListNode< Pair<int,int> >* cur = lists.get(v).getHead();
        while (cur != NULL) {
            int next = cur->data.first;
            if (!visited[next]) {
                visited[next] = true;
                if (hamiltonianUtil(next, visited, pos + 1)) {
                    return true;
                }
                visited[next] = false;
            }
            cur = cur->next;
        }
        return false;
    }
public:
    NetworkGraph(int n, bool directed = false) : vertex_count(n), is_directed(directed) {
        // Allocate adjacency matrix
        matrix = new int*[n];
        for (int i = 0; i < n; i++) {
            matrix[i] = new int[n];
            for (int j = 0; j < n; j++) {
                matrix[i][j] = 0;
            }
        }
        // Allocate adjacency list slots
        for (int i = 0; i < n; i++) {
            lists.push(LinkedList< Pair<int,int> >());
        }
    }
    ~NetworkGraph() {
        for (int i = 0; i < vertex_count; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
    int getVertexCount() const { 
        return vertex_count; 
    }
    bool getDirected() const {
        return is_directed; 
    }
    int** getMatrix() const { 
        return matrix;
    }
    // Add edge to both matrix and list
    void insert_edge(int from, int to, int weight) {
        matrix[from][to] = weight;
        lists.get(from).push(Pair<int,int>(to, weight));
        if (!is_directed) {
            matrix[to][from] = weight;
            lists.get(to).push(Pair<int,int>(from, weight));
        }
    }
    // Remove edge from both matrix and list
    void delete_edge(int from, int to) {
        matrix[from][to] = 0;
        lists.get(from).remove(Pair<int,int>(to, matrix[from][to]));
        if (!is_directed) {
            matrix[to][from] = 0;
            lists.get(to).remove(Pair<int,int>(from, matrix[to][from]));
        }
    }
    bool edge_exists(int from, int to) const {
        return matrix[from][to] != 0; 
    }
    int  edge_weight(int from, int to) const {
        return matrix[from][to];
    }
    // Count edges of a vertex
    int getDegree(int v) const {
        int deg = 0;
        for (int j = 0; j < vertex_count; j++) {
            if (matrix[v][j] != 0) {
                deg++;
            }
        }
        return deg;
    }
    // Count total edges in graph
    int countEdges() const {
        int edges = 0;
        int start = 0;
        for (int i = 0; i < vertex_count; i++) {
            if (!is_directed) {
                start = i + 1;
            }
            for (int j = start; j < vertex_count; j++) {
                if (matrix[i][j] != 0) {
                    edges++;
                }
            }
        }
        return edges;
    }
    // Euler circuit: connected + all degrees even
    bool has_euler_circuit() const {
        if (!isConnected()) {
            return false;
        }
        for (int i = 0; i < vertex_count; i++) {
            if (getDegree(i) % 2 != 0) {
                return false;
            }
        }
        return true;
    }
    // Euler path: connected + exactly 0 or 2 odd-degree vertices
    bool has_euler_path() const {
        if (!isConnected()) {
            return false;
        }
        int odd = 0;
        for (int i = 0; i < vertex_count; i++) {
            if (getDegree(i) % 2 != 0) {
                odd++;
            }
        }
        return odd == 0 || odd == 2;
    }
    // Hamiltonian cycle using backtracking
    bool contains_hamiltonian_cycle() const {
        bool* visited = new bool[vertex_count];
        for (int i = 0; i < vertex_count; i++) {
            visited[i] = false;
        }
        visited[0] = true;
        bool result = hamiltonianUtil(0, visited, 1);
        delete[] visited;
        return result;
    }
    // Complete graph: every pair of vertices connected
    bool is_complete_graph() const {
        for (int i = 0; i < vertex_count; i++) {
            for (int j = 0; j < vertex_count; j++) {
                if (i != j && matrix[i][j] == 0) {
                    return false;
                }
            }
        }
        return true;
    }
    // Count connected components using DFS
    int connected_component_count() const {
        bool* visited = new bool[vertex_count];
        for (int i = 0; i < vertex_count; i++) {
            visited[i] = false;
        }
        int count = 0;
        for (int i = 0; i < vertex_count; i++) {
            if (!visited[i]) {
                dfs(i, visited);
                count++;
            }
        }
        delete[] visited;
        return count;
    }
    // Get adjacency list for a vertex
    const LinkedList< Pair<int,int> >& getList(int v) const {
        return lists.get(v);
    }
    void generate_property_report() const {
        cout << "\n" << BOLD_CYAN  << "NETWORK GRAPH PROPERTIES" << RESET << "\n";
        cout << CYAN << "========================" << RESET << "\n";
        cout << WHITE  << "Total Vertices: " << RESET << BOLD_YELLOW << vertex_count << RESET << "\n";
        cout << WHITE  << "Total Edges: "  << RESET << BOLD_YELLOW << countEdges() << RESET << "\n";
        cout << WHITE  << "Graph Type: "  << RESET << BOLD_YELLOW;
        if (is_directed) { 
            cout << "DIRECTED";
        } else { 
            cout << "UNDIRECTED";
        }
        cout << RESET << "\n";
        cout << WHITE  << "Euler Circuit Exists: "  << RESET;
        printBool(has_euler_circuit());
        cout << "\n";
        cout << WHITE  << "Euler Path Exists: " << RESET;
        printBool(has_euler_path());
        cout << "\n";
        cout << WHITE  << "Hamiltonian Cycle Exists: " << RESET;
        printBool(contains_hamiltonian_cycle());
        cout << "\n";
        cout << WHITE  << "Complete Graph: "  << RESET;
        printBool(is_complete_graph());
        cout << "\n";
        cout << WHITE  << "Connected Components: " << RESET << BOLD_YELLOW << connected_component_count() << RESET << "\n";
    }
};
// Global pointer for predicate function (used in PredicateEngine)
static const NetworkGraph* g_graph = NULL;
bool degreeOver4(int v) {
    if (g_graph == NULL) {
        return false;
    }
    return g_graph->getDegree(v) > 4;
}
//  MODULE 2.2 : Union-Find (Disjoint Set Union)
class UnionFind {
private:
    int* parent;
    int* rank;
    int  sz;
public:
    UnionFind(int n) : sz(n) {
        parent = new int[n];
        rank   = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i]   = 0;
        }
    }
    ~UnionFind() {
        delete[] parent;
        delete[] rank;
    }
    // Find with path compression
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    // Union by rank
    bool unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) {
            return false;
        }
        if (rank[px] < rank[py]) {
            int tmp = px;
            px = py;
            py = tmp;
        }
        parent[py] = px;
        if (rank[px] == rank[py]) {
            rank[px]++;
        }
        return true;
    }
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};
//  MST RESULT STRUCTURE
struct SpanningTreeResult {
    DynArray< Pair<int,int> > tree_edges;
    int  total_cost;
    bool success;
};
//  MODULE 2.2 : Kruskal's MST
SpanningTreeResult kruskals_mst(
    const NetworkGraph&     network,
    const DiscreteSet<int>& mandatory_vertices,
    int root_vertex)
{
    SpanningTreeResult result;
    result.total_cost = 0;
    result.success    = false;
    int   n   = network.getVertexCount();
    int** mat = network.getMatrix();
    // Extract all edges as (weight, u, v) using simple struct
    struct Edge {
        int weight, u, v;
    };
    DynArray<Edge> edges;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (mat[i][j] != 0) {
                Edge e;
                e.weight = mat[i][j];
                e.u = i;
                e.v = j;
                edges.push(e);
            }
        }
    }
    // Simple insertion sort by weight
    for (int i = 1; i < edges.size(); i++) {
        Edge key = edges.get(i);
        int  j   = i - 1;
        while (j >= 0 && edges.get(j).weight > key.weight) {
            edges.get(j + 1) = edges.get(j);
            j--;
        }
        edges.get(j + 1) = key;
    }
    // Build MST using Union-Find
    UnionFind uf(n);
    for (int i = 0; i < edges.size(); i++) {
        int w = edges.get(i).weight;
        int u = edges.get(i).u;
        int v = edges.get(i).v;
        if (uf.unite(u, v)) {
            result.tree_edges.push(Pair<int,int>(u, v));
            result.total_cost += w;
        }
    }
    // Check all mandatory vertices are connected to root
    for (int i = 0; i < mandatory_vertices.size(); i++) {
        if (!uf.connected(root_vertex, mandatory_vertices.get(i))) {
            result.success = false;
            return result;
        }
    }
    result.success = true;
    return result;
}
//  SHORTEST PATH RESULT STRUCTURE
struct ShortestPathResult {
    DynArray<int> vertex_sequence;
    int  path_cost;
    bool found;
};
//  MODULE 2.3 : Dijkstra's Shortest Path
ShortestPathResult dijkstra_path(
    const NetworkGraph& network,
    int source_vertex,
    int target_vertex)
{
    ShortestPathResult result;
    result.found     = false;
    result.path_cost = 2147483647; // INT_MAX
    int n = network.getVertexCount();
    int* dist = new int[n];
    int* prev = new int[n];
    for (int i = 0; i < n; i++) {
        dist[i] = 2147483647;
        prev[i] = -1;
    }
    dist[source_vertex] = 0;
    MinHeap pq;
    pq.push(0, source_vertex);
    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (d > dist[u]) {
            continue;
        }
        ListNode< Pair<int,int> >* cur = network.getList(u).getHead();
        while (cur != NULL) {
            int v = cur->data.first;
            int w = cur->data.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push(dist[v], v);
            }
            cur = cur->next;
        }
    }
    if (dist[target_vertex] == 2147483647) {
        delete[] dist;
        delete[] prev;
        return result;
    }
    result.found     = true;
    result.path_cost = dist[target_vertex];
    // Build path by backtracking from target to source
    DynArray<int> reversed;
    for (int at = target_vertex; at != -1; at = prev[at]) {
        reversed.push(at);
    }
    // Reverse the path
    for (int i = reversed.size() - 1; i >= 0; i--) {
        result.vertex_sequence.push(reversed.get(i));
    }
    delete[] dist;
    delete[] prev;
    return result;
}
//  RELATION PAIR STRUCTURE
struct RelPair {
    int first, second;
    RelPair() : first(0), second(0) {}
    RelPair(int a, int b) : first(a), second(b) {}
    bool operator==(const RelPair& other) const {
        return first == other.first && second == other.second;
    }
};
//  MODULE 3.1 : BinaryRelation
class BinaryRelation {
private:
    DiscreteSet<int>  base_set;
    DynArray<RelPair> relation_pairs;
    // Check if pair already exists
    bool pairExists(int x, int y) const {
        for (int i = 0; i < relation_pairs.size(); i++) {
            if (relation_pairs.get(i).first  == x &&
                relation_pairs.get(i).second == y) {
                return true;
            }
        }
        return false;
    }
public:
    BinaryRelation() {}
    BinaryRelation(const DiscreteSet<int>& base) : base_set(base) {
        
    }
    void insert_pair(int x, int y) {
        if (!pairExists(x, y)) {
            relation_pairs.push(RelPair(x, y));
        }
    }
    void remove_pair(int x, int y) {
        for (int i = 0; i < relation_pairs.size(); i++) {
            if (relation_pairs.get(i).first  == x &&
                relation_pairs.get(i).second == y) {
                relation_pairs.removeAt(i);
                return;
            }
        }
    }
    bool has_pair(int x, int y) const {
        return pairExists(x, y);
    }
    // Every element relates to itself
    bool test_reflexive() const {
        for (int i = 0; i < base_set.size(); i++) {
            int a = base_set.get(i);
            if (!has_pair(a, a)) {
                return false;
            }
        }
        return true;
    }
    // If (a,b) exists then (b,a) must also exist
    bool test_symmetric() const {
        for (int i = 0; i < relation_pairs.size(); i++) {
            int a = relation_pairs.get(i).first;
            int b = relation_pairs.get(i).second;
            if (!has_pair(b, a)) {
                return false;
            }
        }
        return true;
    }
    // If (a,b) and (b,a) both exist then a must equal b
    bool test_antisymmetric() const {
        for (int i = 0; i < relation_pairs.size(); i++) {
            int a = relation_pairs.get(i).first;
            int b = relation_pairs.get(i).second;
            if (a != b && has_pair(b, a)) {
                return false;
            }
        }
        return true;
    }
    // If (a,b) and (b,c) exist then (a,c) must also exist
    bool test_transitive() const {
        for (int i = 0; i < relation_pairs.size(); i++) {
            for (int j = 0; j < relation_pairs.size(); j++) {
                int a = relation_pairs.get(i).first;
                int b = relation_pairs.get(i).second;
                int c = relation_pairs.get(j).second;
                if (b == relation_pairs.get(j).first) {
                    if (!has_pair(a, c)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    bool is_equivalence()   const { 
        return test_reflexive() && test_symmetric() && test_transitive();
    }
    bool is_partial_order() const { 
        return test_reflexive() && test_antisymmetric() && test_transitive();
    }
    bool is_total_order() const {
        if (!is_partial_order()) {
            return false;
        }
        for (int i = 0; i < base_set.size(); i++) {
            for (int j = 0; j < base_set.size(); j++) {
                int a = base_set.get(i);
                int b = base_set.get(j);
                if (!has_pair(a, b) && !has_pair(b, a)) {
                    return false;
                }
            }
        }
        return true;
    }
    // Add reflexive pairs that are missing
    BinaryRelation compute_reflexive_closure() const {
        BinaryRelation r(base_set);
        for (int i = 0; i < relation_pairs.size(); i++) {
            r.insert_pair(relation_pairs.get(i).first, relation_pairs.get(i).second);
        }
        for (int i = 0; i < base_set.size(); i++) {
            r.insert_pair(base_set.get(i), base_set.get(i));
        }
        return r;
    }
    // Add reverse of each pair
    BinaryRelation compute_symmetric_closure() const {
        BinaryRelation r(base_set);
        for (int i = 0; i < relation_pairs.size(); i++) {
            r.insert_pair(relation_pairs.get(i).first,  relation_pairs.get(i).second);
            r.insert_pair(relation_pairs.get(i).second, relation_pairs.get(i).first);
        }
        return r;
    }
    // Floyd-Warshall to compute transitive closure
    BinaryRelation compute_transitive_closure() const {
        int n = base_set.size();
        // Allocate 2D reachability matrix
        bool** reach = new bool*[n];
        for (int i = 0; i < n; i++) {
            reach[i] = new bool[n];
            for (int j = 0; j < n; j++) {
                reach[i][j] = false;
            }
            reach[i][i] = true;
        }
        // Fill from existing pairs
        for (int p = 0; p < relation_pairs.size(); p++) {
            int a = relation_pairs.get(p).first;
            int b = relation_pairs.get(p).second;
            int ai = -1, bi = -1;
            for (int i = 0; i < n; i++) {
                if (base_set.get(i) == a) { 
                    ai = i;
                }
                if (base_set.get(i) == b) {
                    bi = i;
                }
            }
            if (ai != -1 && bi != -1) {
                reach[ai][bi] = true;
            }
        }
        // Floyd-Warshall
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (reach[i][k] && reach[k][j]) {
                        reach[i][j] = true;
                    }
                }
            }
        }
        BinaryRelation result(base_set);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (reach[i][j]) {
                    result.insert_pair(base_set.get(i), base_set.get(j));
                }
            }
        }
        for (int i = 0; i < n; i++) {
            delete[] reach[i];
        }
        delete[] reach;
        return result;
    }
    void printAnalysis(const string& name) const {
        cout << "\n" << BOLD_CYAN << "RELATION " << name << " ANALYSIS:" << RESET << "\n";
        cout << WHITE << "Reflexive: "  << RESET; 
        printBool(test_reflexive());  
        cout << "\n";
        cout << WHITE << "Symmetric: " << RESET; 
        printBool(test_symmetric()); 
        cout << "\n";
        cout << WHITE << "Transitive: "<< RESET; 
        printBool(test_transitive());   
        cout << "\n";
        cout << WHITE << "Partial Order: " << RESET; 
        printBool(is_partial_order());  
        cout << "\n";
    }
};
//  MODULE 3.2 : DiscreteFunction
// Simple key-value pair for function map
struct FuncEntry {
    int key, val;
    FuncEntry() : key(0), val(0) {}
    FuncEntry(int k, int v) : key(k), val(v) {}
};
class DiscreteFunction {
private:
    DiscreteSet<int>   input_set;
    DiscreteSet<int>   output_set;
    DynArray<FuncEntry> function_map;
    // Find value for a key, returns -1 if not found
    int findVal(int x) const {
        for (int i = 0; i < function_map.size(); i++) {
            if (function_map.get(i).key == x) {
                return function_map.get(i).val;
            }
        }
        return -1;
    }
public:
    DiscreteFunction() {
        
    }
    DiscreteFunction(const DiscreteSet<int>& inputs, const DiscreteSet<int>& outputs)
        : input_set(inputs), output_set(outputs) {}
    void set_mapping(int x, int y) {
        // Update if key exists, else add new entry
        for (int i = 0; i < function_map.size(); i++) {
            if (function_map.get(i).key == x) {
                function_map.get(i).val = y;
                return;
            }
        }
        function_map.push(FuncEntry(x, y));
    }
    bool has_mapping(int x) const {
        return findVal(x) != -1;
    }
    int apply(int x) const {
        int val = findVal(x);
        if (val == -1) {
            return -1;
        }
        return val;
    }
    // Defined for all domain elements
    bool is_total_function() const {
        for (int i = 0; i < input_set.size(); i++) {
            if (!has_mapping(input_set.get(i))) {
                return false;
            }
        }
        return true;
    }
    // No two elements map to same value
    bool is_injection() const {
        for (int i = 0; i < function_map.size(); i++) {
            for (int j = i + 1; j < function_map.size(); j++) {
                if (function_map.get(i).val == function_map.get(j).val) {
                    return false;
                }
            }
        }
        return true;
    }
    // Every output element is mapped to
    bool is_surjection() const {
        for (int i = 0; i < output_set.size(); i++) {
            bool found = false;
            for (int j = 0; j < function_map.size(); j++) {
                if (function_map.get(j).val == output_set.get(i)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        return true;
    }
    bool is_bijection() const {
        return is_injection() && is_surjection();
    }
    // Returns image (set of all output values)
    DiscreteSet<int> compute_image() const {
        DiscreteSet<int> img;
        for (int i = 0; i < function_map.size(); i++) {
            img.add(function_map.get(i).val);
        }
        return img;
    }
    // Returns all inputs that map to y
    DiscreteSet<int> compute_preimage(int y) const {
        DiscreteSet<int> pre;
        for (int i = 0; i < function_map.size(); i++) {
            if (function_map.get(i).val == y) {
                pre.add(function_map.get(i).key);
            }
        }
        return pre;
    }
};
//  SIMPLE STRING MAP
struct NamedSet {
    string  name;
    DiscreteSet<int> data;
};
class StringSetMap {
private:
    DynArray<NamedSet> entries;
    int findIndex(const string& name) const {
        for (int i = 0; i < entries.size(); i++) {
            if (entries.get(i).name == name) {
                return i;
            }
        }
        return -1;
    }
public:
    void set(const string& name, const DiscreteSet<int>& val) {
        int idx = findIndex(name);
        if (idx != -1) {
            entries.get(idx).data = val;
        }
        else {
            NamedSet ns;
            ns.name = name;
            ns.data = val;
            entries.push(ns);
        }
    }
    bool has(const string& name) const {
        return findIndex(name) != -1;
    }
    DiscreteSet<int>& get(const string& name) {
        int idx = findIndex(name);
        return entries.get(idx).data;
    }
    int  size() const { 
        return entries.size();
    }
    string getName(int i) const { 
        return entries.get(i).name;
    }
    DiscreteSet<int> getData(int i) const { 
        return entries.get(i).data; 
    }
};
//  MODULE 3.3 : CDAFController
class CDAFController {
private:
    NetworkGraph*  infrastructure;
    DynArray<BinaryRelation> system_relations;
    DynArray<DiscreteFunction> system_functions;
    StringSetMap  named_collections;
    DynArray<string> relation_names;
    DynArray<string> function_names;
    bool configLoaded;
    // Parse {1,2,3} format into DiscreteSet<int>
    DiscreteSet<int> parseSet(const string& s) {
        DiscreteSet<int> result;
        // Remove { and }
        string inner = s.substr(1, s.size() - 2);
        stringstream ss(inner);
        string token;
        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                result.add(atoi(token.c_str()));
            }
        }
        return result;
    }
public:
    CDAFController() : infrastructure(NULL), configLoaded(false) {
        
    }
    ~CDAFController() {
        delete infrastructure;
    }
    void load_system_config(const string& config_file) {
        ifstream fin(config_file.c_str());
        if (!fin) {
            cout << BOLD_RED << "Cannot open file: " << config_file << RESET << "\n";
            return;
        }
        string line;
        string section = "";
        int  vertices = 0;
        bool directed = false;
        // Store edge data while reading
        struct EdgeData { int u, v, w; };
        DynArray<EdgeData> edgeList;
        // Store relation data while reading
        struct RelData {
            string name;
            string baseName;
            DynArray<RelPair> pairs;
        };
        DynArray<RelData> relList;
        RelData curRel;
        bool inRel = false;
        // Store function data while reading
        struct FuncData {
            string name;
            string domName;
            string codomName;
            DynArray< Pair<int,int> > maps;
        };
        DynArray<FuncData> funcList;
        FuncData curFunc;
        bool inFunc = false;
        while (getline(fin, line)) {
            if (line.empty()) {
                continue;
            }
            if (line == "COLLECTIONS") { 
                section = "COLLECTIONS";
                continue;
            }
            if (line == "NETWORK") { 
                section = "NETWORK";  
                continue;
            }
            if (line == "RELATIONS") { 
                section = "RELATIONS";
                continue; 
            }
            if (line == "FUNCTIONS") { 
                section = "FUNCTIONS"; 
                continue;
            }
            if (section == "COLLECTIONS") {
                size_t c = line.find(':');
                if (c != string::npos) {
                    string key = line.substr(0, c);
                    string val = line.substr(c + 1);
                    named_collections.set(key, parseSet(val));
                }
            }
            else if (section == "NETWORK") {
                if (line.find("vertices:") == 0) {
                    vertices = atoi(line.substr(9).c_str());
                }
                else if (line.find("directed:") == 0) {
                    directed = (line.substr(9) == "true");
                }
                else if (line == "connections:") {
                    // Skip header
                }
                else {
                    // Parse u,v,w
                    stringstream ss(line);
                    string t;
                    DynArray<int> nums;
                    while (getline(ss, t, ',')) {
                        nums.push(atoi(t.c_str()));
                    }
                    if (nums.size() == 3) {
                        EdgeData e;
                        e.u = nums.get(0);
                        e.v = nums.get(1);
                        e.w = nums.get(2);
                        edgeList.push(e);
                    }
                }
            }
            else if (section == "RELATIONS") {
                if (line.find("pairs:") == 0) {
                    // Parse (a,b),(c,d),...
                    string inner = line.substr(6);
                    stringstream ss(inner);
                    string tok;
                    while (getline(ss, tok, ')')) {
                        if (tok.empty() || tok == ",") {
                            continue;
                        }
                        if (tok[0] == ',') {
                            tok = tok.substr(1);
                        }
                        if (tok[0] == '(') {
                            tok = tok.substr(1);
                        }
                        size_t comma = tok.find(',');
                        if (comma == string::npos) {
                            continue;
                        }
                        int a = atoi(tok.substr(0, comma).c_str());
                        int b = atoi(tok.substr(comma + 1).c_str());
                        curRel.pairs.push(RelPair(a, b));
                    }
                }
                else {
                    if (inRel) {
                        relList.push(curRel);
                    }
                    curRel = RelData();
                    inRel  = true;
                    size_t c  = line.find(':');
                    curRel.name = line.substr(0, c);
                    size_t bp = line.find("base_set:");
                    if (bp != string::npos) {
                        curRel.baseName = line.substr(bp + 9);
                    }
                }
            }
            else if (section == "FUNCTIONS") {
                if (line.find("maps:") == 0) {
                    // Parse x->y,x->y,...
                    string mstr = line.substr(5);
                    stringstream ss(mstr);
                    string tok;
                    while (getline(ss, tok, ',')) {
                        size_t arrow = tok.find("->");
                        if (arrow == string::npos) {
                            continue;
                        }
                        int a = atoi(tok.substr(0, arrow).c_str());
                        int b = atoi(tok.substr(arrow + 2).c_str());
                        curFunc.maps.push(Pair<int,int>(a, b));
                    }
                }
                else {
                    if (inFunc) {
                        funcList.push(curFunc);
                    }
                    curFunc = FuncData();
                    inFunc  = true;
                    size_t c  = line.find(':');
                    curFunc.name = line.substr(0, c);
                    size_t ip = line.find("input:");
                    size_t op = line.find(":output:");
                    if (ip != string::npos && op != string::npos) {
                        curFunc.domName   = line.substr(ip + 6, op - (ip + 6));
                        curFunc.codomName = line.substr(op + 8);
                    }
                }
            }
        }
        if (inRel)  {
            relList.push(curRel);
        }
        if (inFunc) { 
            funcList.push(curFunc);
        }
        // Build graph
        delete infrastructure;
        infrastructure = new NetworkGraph(vertices, directed);
        for (int i = 0; i < edgeList.size(); i++) {
            infrastructure->insert_edge(edgeList.get(i).u, edgeList.get(i).v, edgeList.get(i).w);
        }
        // Build relations
        system_relations.clear();
        relation_names.clear();
        for (int i = 0; i < relList.size(); i++) {
            DiscreteSet<int> base;
            if (named_collections.has(relList.get(i).baseName)) {
                base = named_collections.get(relList.get(i).baseName);
            }
            BinaryRelation br(base);
            for (int j = 0; j < relList.get(i).pairs.size(); j++) {
                br.insert_pair(relList.get(i).pairs.get(j).first, relList.get(i).pairs.get(j).second);
            }
            system_relations.push(br);
            relation_names.push(relList.get(i).name);
        }
        // Build functions
        system_functions.clear();
        function_names.clear();
        for (int i = 0; i < funcList.size(); i++) {
            DiscreteSet<int> dom, codom;
            if (named_collections.has(funcList.get(i).domName)) {
                dom = named_collections.get(funcList.get(i).domName);
            }
            if (named_collections.has(funcList.get(i).codomName)) {
                codom = named_collections.get(funcList.get(i).codomName);
            }
            DiscreteFunction df(dom, codom);
            for (int j = 0; j < funcList.get(i).maps.size(); j++) {
                df.set_mapping(funcList.get(i).maps.get(j).first, funcList.get(i).maps.get(j).second);
            }
            system_functions.push(df);
            function_names.push(funcList.get(i).name);
        }
        configLoaded = true;
        cout << BOLD_GREEN << "Configuration loaded from: " << config_file << RESET << "\n";
    }
    void run_set_analysis() {
        if (!named_collections.has("critical_infrastructure") ||
            !named_collections.has("high_traffic")           ||
            !named_collections.has("redundancy_nodes")) {
            cout << BOLD_RED << "Required sets not found in config." << RESET << "\n";
            return;
        }
        DiscreteSet<int> ci = named_collections.get("critical_infrastructure");
        DiscreteSet<int> ht = named_collections.get("high_traffic");
        DiscreteSet<int> rn = named_collections.get("redundancy_nodes");
        DiscreteSet<int> uni  = ci.set_union(ht);
        DiscreteSet<int> mandatory = GraphConstraintBuilder::compute_mandatory_nodes(ci, ht, rn);
        cout << "\n" << BOLD_CYAN << "SET OPERATIONS:" << RESET << "\n";
        cout << WHITE << "(critical_infrastructure U high_traffic) = " << RESET << BOLD_YELLOW;
        uni.print();
        cout << RESET << "\n";
        cout << WHITE << "(critical_infrastructure U high_traffic) n redundancy_nodes = " << RESET << BOLD_YELLOW;
        mandatory.print();
        cout << RESET << "\n";
        cout << WHITE << "Mandatory Nodes = " << RESET << BOLD_YELLOW;
        mandatory.print();
        cout << RESET << "\n";
    }
    void run_graph_analysis() {
        if (!infrastructure) {
            cout << BOLD_RED << "No graph loaded." << RESET << "\n";
            return;
        }
        infrastructure->generate_property_report();
    }
    void run_relation_analysis() {
        if (system_relations.empty()) {
            cout << BOLD_RED << "No relations loaded." << RESET << "\n";
            return;
        }
        for (int i = 0; i < system_relations.size(); i++) {
            system_relations.get(i).printAnalysis(relation_names.get(i));
        }
    }
    void find_optimal_paths() {
        if (!infrastructure) {
            cout << BOLD_RED << "No graph loaded." << RESET << "\n";
            return;
        }
        int n = infrastructure->getVertexCount();
        int src, tgt;
        cout << "Enter source vertex (0-" << n - 1 << "): ";
        cin  >> src;
        cout << "Enter target vertex (0-" << n - 1 << "): ";
        cin  >> tgt;
        ShortestPathResult sp = dijkstra_path(*infrastructure, src, tgt);
        cout << "\n" << BOLD_CYAN << "SHORTEST PATH (" << src << " to " << tgt << "):" << RESET << "\n";
        if (!sp.found) {
            cout << BOLD_RED << "SHORTEST PATH: [NO PATH]" << RESET << "\n";
            cout << BOLD_RED << "TOTAL COST: INFINITY"     << RESET << "\n";
        }
        else {
            cout << WHITE << "SHORTEST PATH: " << RESET << BOLD_GREEN << "[";
            for (int i = 0; i < sp.vertex_sequence.size(); i++) {
                if (i > 0) {
                    cout << "->";
                }
                cout << sp.vertex_sequence.get(i);
            }
            cout << "]" << RESET << "\n";
            cout << WHITE << "TOTAL COST: " << RESET << BOLD_YELLOW << sp.path_cost << RESET << "\n";
        }
    }
    void build_constrained_mst() {
        if (!infrastructure) {
            cout << BOLD_RED << "No graph loaded." << RESET << "\n";
            return;
        }
        DiscreteSet<int> mandatory;
        if (named_collections.has("critical_infrastructure") &&
            named_collections.has("high_traffic")  &&
            named_collections.has("redundancy_nodes")) {
            mandatory = GraphConstraintBuilder::compute_mandatory_nodes(
                named_collections.get("critical_infrastructure"),
                named_collections.get("high_traffic"),
                named_collections.get("redundancy_nodes"));
        }
        SpanningTreeResult mst = kruskals_mst(*infrastructure, mandatory, 0);
        int** mat  = infrastructure->getMatrix();
        cout << "\n" << BOLD_CYAN << "MINIMUM SPANNING TREE (Kruskal's):" << RESET << "\n";
        cout << WHITE << "Root Vertex: " << RESET << BOLD_YELLOW << "0" << RESET << "\n";
        cout << WHITE << "Mandatory Vertices: " << RESET << BOLD_YELLOW;
        mandatory.print();
        cout << RESET << "\n";
        int totalW = 0;
        for (int i = 0; i < mst.tree_edges.size(); i++) {
            int u = mst.tree_edges.get(i).first;
            int v = mst.tree_edges.get(i).second;
            int w = mat[u][v];
            cout << GREEN << "Edge (" << u << ", " << v << "): Weight " << w << RESET << "\n";
            totalW += w;
        }
        cout << WHITE << "TOTAL COST: " << RESET << BOLD_YELLOW << totalW << RESET << "\n";
        cout << WHITE << "RESULT: " << RESET;
        if (mst.success) {
            cout << BOLD_GREEN << "SUCCESS" << RESET << "\n";
        }
        else {
            cout << BOLD_RED << "FAILED" << RESET << "\n";
        }
    }
    void execute_full_analysis() {
        cout << "\n" << BOLD_MAGENTA << "=== CDAF ANALYSIS REPORT ===" << RESET << "\n\n";
        run_set_analysis();
        if (!infrastructure) {
            cout << BOLD_RED << "No graph loaded." << RESET << "\n";
            return;
        }
        cout << "\n" << BOLD_CYAN << "NETWORK ANALYSIS:" << RESET << "\n";
        cout << WHITE << "Total Vertices: "  << RESET << BOLD_YELLOW << infrastructure->getVertexCount() << RESET << "\n";
        cout << WHITE << "Total Edges: "  << RESET << BOLD_YELLOW << infrastructure->countEdges() << RESET << "\n";
        cout << WHITE << "Euler Circuit Exists: " << RESET; printBool(infrastructure->has_euler_circuit());         
        cout << "\n";
        cout << WHITE << "Hamiltonian Cycle Exists: "<< RESET; printBool(infrastructure->contains_hamiltonian_cycle()); 
        cout << "\n";
        cout << WHITE << "Complete Graph: " << RESET; printBool(infrastructure->is_complete_graph());  
        cout << "\n";
        build_constrained_mst();
        int last = infrastructure->getVertexCount() - 1;
        ShortestPathResult sp = dijkstra_path(*infrastructure, 0, last);
        cout << "\n" << BOLD_CYAN << "SHORTEST PATH (0 to " << last << "):" << RESET << "\n";
        if (!sp.found) {
            cout << BOLD_RED << "SHORTEST PATH: [NO PATH]" << RESET << "\n";
            cout << BOLD_RED << "TOTAL COST: INFINITY"     << RESET << "\n";
        }
        else {
            cout << WHITE << "SHORTEST PATH: " << RESET << BOLD_GREEN << "[";
            for (int i = 0; i < sp.vertex_sequence.size(); i++) {
                if (i > 0) {
                    cout << "->";
                }
                cout << sp.vertex_sequence.get(i);
            }
            cout << "]" << RESET << "\n";
            cout << WHITE << "TOTAL COST: " << RESET << BOLD_YELLOW << sp.path_cost << RESET << "\n";
        }
        run_relation_analysis();
        // Use plain function pointer predicate to check graph property
        g_graph = infrastructure;
        DiscreteSet<int> nodeSet;
        for (int i = 0; i < infrastructure->getVertexCount(); i++) {
            nodeSet.add(i);
        }
        bool anyHigh = PredicateEngine::exists(nodeSet, degreeOver4);
        g_graph = NULL;
        cout << "\n" << BOLD_CYAN << "PREDICATE CHECK:" << RESET << "\n";
        cout << WHITE << "Exists node with degree > 4: " << RESET;
        printBool(anyHigh);
        cout << "\n";
        cout << "\n" << BOLD_MAGENTA << "=== END REPORT ===" << RESET << "\n";
    }
    bool isLoaded() const {
        return configLoaded; 
    }
    void interactiveSetOps() {
        cout << "\n" << BOLD_CYAN << "--- Interactive Set Operations ---" << RESET << "\n";
        cout << WHITE << "Available collections:" << RESET << "\n";
        for (int i = 0; i < named_collections.size(); i++) {
            cout << "  " << YELLOW << named_collections.getName(i) << RESET << " = " << BOLD_YELLOW;
            named_collections.getData(i).print();
            cout << RESET << "\n";
        }
        string a, b;
        cout << "Enter name of set A: ";
        cin  >> a;
        cout << "Enter name of set B: ";
        cin  >> b;
        if (!named_collections.has(a) || !named_collections.has(b)) {
            cout << BOLD_RED << "One or both sets not found." << RESET << "\n";
            return;
        }
        DiscreteSet<int> A = named_collections.get(a);
        DiscreteSet<int> B = named_collections.get(b);
        cout << WHITE << "Union : " << RESET << BOLD_YELLOW;
        A.set_union(B).print();
        cout << RESET << "\n";
        cout << WHITE << "Intersection : " << RESET << BOLD_YELLOW;
        A.set_intersection(B).print();
        cout << RESET << "\n";
        cout << WHITE << "Difference : " << RESET << BOLD_YELLOW;
        A.set_difference(B).print();
        cout << RESET << "\n";
        cout << WHITE << "Sym Diff : " << RESET << BOLD_YELLOW;
        A.symmetric_diff(B).print();
        cout << RESET << "\n";
        cout << WHITE << "A subset B?  : " << RESET;
        if (A.is_subset(B)) {
            cout << BOLD_GREEN << "YES" << RESET << "\n";
        }
        else {
            cout << BOLD_RED << "NO" << RESET << "\n";
        }
    }
    void showMenu() {
        cout << "\n" << BOLD_BLUE    << "========================================" << RESET << "\n";
        cout <<  BOLD_MAGENTA << "  CDAF - Computational Discrete Analysis" << RESET << "\n";
        cout <<  BOLD_BLUE    << "========================================" << RESET << "\n";
        cout << CYAN << "1." << RESET << " Load Configuration File\n";
        cout << CYAN << "2." << RESET << " Run Full Analysis (All Modules)\n";
        cout << CYAN << "3." << RESET << " Set Operations Only\n";
        cout << CYAN << "4." << RESET << " Graph Property Analysis\n";
        cout << CYAN << "5." << RESET << " Find Shortest Path (Dijkstra)\n";
        cout << CYAN << "6." << RESET << " Build Constrained MST (Kruskal's)\n";
        cout << CYAN << "7." << RESET << " Relation Analysis\n";
        cout << CYAN << "8." << RESET << " Interactive Set Operations\n";
        cout << BOLD_RED << "9." << RESET << " Exit\n";
        cout << BOLD_YELLOW << "Enter choice: " << RESET;
    }
    void run() {
        int choice;
        do {
            showMenu();
            cin >> choice;
            switch (choice) {
                case 1: {
                    string fname;
                    cout << "Enter config filename: ";
                    cin  >> fname;
                    load_system_config(fname);
                    break;
                }
                case 2:
                    if (!isLoaded()) {
                        cout << BOLD_RED << "Load config first (option 1)." << RESET << "\n";
                    }
                    else {
                        execute_full_analysis();
                    }
                    break;
                case 3:
                    if (!isLoaded()) {
                        cout << BOLD_RED << "Load config first (option 1)." << RESET << "\n";
                    }
                    else {
                        run_set_analysis();
                    }
                    break;
                case 4:
                    if (!isLoaded()) {
                        cout << BOLD_RED << "Load config first (option 1)." << RESET << "\n";
                    }
                    else {
                        run_graph_analysis();
                    }
                    break;
                case 5:
                    if (!isLoaded()) {
                        cout << BOLD_RED << "Load config first (option 1)." << RESET << "\n";
                    }
                    else {
                        find_optimal_paths();
                    }
                    break;
                case 6:
                    if (!isLoaded()) {
                        cout << BOLD_RED << "Load config first (option 1)." << RESET << "\n";
                    }
                    else {
                        build_constrained_mst();
                    }
                    break;
                case 7:
                    if (!isLoaded()) {
                        cout << BOLD_RED << "Load config first (option 1)." << RESET << "\n";
                    }
                    else {
                        run_relation_analysis();
                    }
                    break;
                case 8:
                    if (!isLoaded()) {
                        cout << BOLD_RED << "Load config first (option 1)." << RESET << "\n";
                    }
                    else {
                        interactiveSetOps();
                    }
                    break;
                case 9:
                    cout << BOLD_MAGENTA << "Exiting CDAF. Goodbye!" << RESET << "\n";
                    break;
                default:
                    cout << BOLD_RED << "Invalid choice." << RESET << "\n";
            }
        } while (choice != 9);
    }
};
//  Create test input file 
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
    f << "0,1,4\n";
    f << "0,2,3\n";
    f << "1,2,2\n";
    f << "1,3,6\n";
    f << "2,3,5\n";
    f << "2,4,4\n";
    f << "3,4,2\n";
    f << "3,5,8\n";
    f << "4,5,3\n";
    f << "5,6,5\n";
    f << "6,7,6\n\n";
    f << "RELATIONS\n";
    f << "R1:base_set:universe\n";
    f << "pairs:(0,1),(0,2),(0,3),(0,4),(0,5),(5,6),(3,5)\n\n";
    f << "FUNCTIONS\n";
    f << "F1:input:universe:output:universe\n";
    f << "maps:0->1,1->2,2->3,3->4,4->5\n";
    f.close();
    cout << BOLD_GREEN << "Test file 'test_input_b.txt' created." << RESET << "\n";
}
//  MAIN
int main() {
    createTestInputB();
    CDAFController cdaf;
    cdaf.run();
    return 0;
}
