<div align="center">

# 🧮 CDAF
### Computational Discrete Analysis Framework

![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen?style=for-the-badge)
![Course](https://img.shields.io/badge/Course-Discrete%20Structure%20CS1005-orange?style=for-the-badge)
![University](https://img.shields.io/badge/FAST-NUCES%20CFD-blue?style=for-the-badge)

> A menu-driven C++ application that validates discrete mathematical structures, computes relational properties, and executes advanced graph algorithms.

</div>

---

## 📌 Project Overview

**CDAF** is built as part of the **Discrete Structure CS1005** semester project at **FAST NUCES CFD Campus**. It implements core discrete mathematics concepts in an integrated, modular C++ system.

The system is divided into **3 Questions**, each with **3 Modules**:

```
CDAF
├── Question 1 — Set Theory & Predicate Logic
│   ├── Module 1.1 → DiscreteSet<T>        (custom template set from scratch)
│   ├── Module 1.2 → PredicateEngine        (forall, exists, De Morgan's laws)
│   └── Module 1.3 → GraphConstraintBuilder (generates mandatory/forbidden nodes)
│
├── Question 2 — Graph Algorithms & Analysis
│   ├── Module 2.1 → NetworkGraph           (dual adjacency matrix + list)
│   ├── Module 2.2 → Kruskal's MST          (with mandatory vertex constraints)
│   └── Module 2.3 → Dijkstra's Algorithm   (shortest path via adjacency list)
│
└── Question 3 — Relations, Functions & Integration
    ├── Module 3.1 → BinaryRelation         (reflexive, symmetric, transitive, closures)
    ├── Module 3.2 → DiscreteFunction       (injective, surjective, bijective, inverse)
    └── Module 3.3 → CDAFController         (menu-driven integrated system)
```

---

## ⚙️ Tech Stack

- **Language:** C++17
- **Paradigm:** Object-Oriented Programming (OOP)
- **STL Used:** `vector`, `list`, `map`, `set`, `queue`, `functional`
- **Custom:** `DiscreteSet<T>` built from scratch (no `std::set`)

---

## 🚀 Getting Started

### Prerequisites
- GCC with C++17 support
- Linux / Windows (MinGW) / macOS

### Compile & Run

```bash
# Clone the repo
git clone https://github.com/your-username/CDAF.git
cd CDAF

# Compile
g++ -std=c++17 -o cdaf CDAF_Complete.cpp

# Run
./cdaf
```

---

## 🖥️ Menu

```
========================================
  CDAF - Computational Discrete Analysis
========================================
1. Load Configuration File
2. Run Full Analysis (All Modules)
3. Set Operations Only
4. Graph Property Analysis
5. Find Shortest Path (Dijkstra)
6. Build Constrained MST (Kruskal's)
7. Relation Analysis
8. Interactive Set Operations
9. Exit
```

---

## 📂 Configuration File Format

The program reads a `.txt` config file. A sample `test_input_b.txt` is auto-generated on first run.

```
COLLECTIONS
universe:{0,1,2,3,4,5,6,7}
critical_infrastructure:{1,3,5}
high_traffic:{2,3,4}
redundancy_nodes:{3,5}

NETWORK
vertices:8
directed:false
connections:
0,1,4
0,2,3
...

RELATIONS
R1:base_set:universe
pairs:(0,1),(0,2),(3,5)

FUNCTIONS
F1:input:universe:output:universe
maps:0->1,1->2,2->3
```

---

## 📊 Sample Output

```
=== CDAF ANALYSIS REPORT ===

SET OPERATIONS:
(critical_infrastructure U high_traffic) = {1,2,3,4,5}
(critical_infrastructure U high_traffic) n redundancy_nodes = {3,5}
Mandatory Nodes = {3,5}

NETWORK ANALYSIS:
Total Vertices: 8
Total Edges: 11
Euler Circuit Exists: FALSE
Hamiltonian Cycle Exists: FALSE
Complete Graph: FALSE

MINIMUM SPANNING TREE (Kruskal's):
Root Vertex: 0
Mandatory Vertices: {3,5}
Edge (1, 2): Weight 2
Edge (3, 4): Weight 2
Edge (0, 2): Weight 3
Edge (4, 5): Weight 3
Edge (2, 4): Weight 4
Edge (5, 6): Weight 5
Edge (6, 7): Weight 6
TOTAL COST: 25
RESULT: SUCCESS

SHORTEST PATH (0 to 7):
SHORTEST PATH: [0->2->4->5->6->7]
TOTAL COST: 21

RELATION R1 ANALYSIS:
Reflexive: FALSE
Symmetric: FALSE
Transitive: FALSE
Partial Order: FALSE

=== END REPORT ===
```

---

## 🧠 OOP Concepts Used

| Concept | Where |
|---------|-------|
| **Encapsulation** | All classes with private data members |
| **Abstraction** | `CDAFController` hides internal module complexity |
| **Templates** | `DiscreteSet<T>`, `LogicalPredicate<T>` |
| **Operator Overloading** | `<<`, `>>`, `==`, `<`, `>` in `DiscreteSet` |
| **Composition** | `CDAFController` owns `NetworkGraph`, `BinaryRelation`, `DiscreteFunction` |

---

## 👥 Group Members

| Name | Role |
|------|------|
| _____________________ | Set Theory & Predicate Logic — Modules 1.1, 1.2, 1.3 |
| _____________________ | Graph Algorithms & Analysis — Modules 2.1, 2.2, 2.3 |
| _____________________ | Relations, Functions & Integration — Modules 3.1, 3.2, 3.3 |

---

## 📄 License

This project was developed for academic purposes at **FAST NUCES CFD Campus**.

---

<div align="center">
Made with ❤️ by Group ___ &nbsp;|&nbsp; FAST NUCES CFD &nbsp;|&nbsp; Batch 2028
</div>
