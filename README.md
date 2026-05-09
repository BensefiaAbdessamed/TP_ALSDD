# Text Analyzer & Set Operations (AVL Trees)

A comprehensive C-based implementation for text analysis and set-theoretic operations, developed for the **Advanced Data Structures (ADDS)** lab work at **ESI (ex. INI)**. This project transforms raw text files into structured mathematical sets using balanced **AVL Trees**, allowing for efficient linguistic analysis.

## 👥 Authors
- **Abdessamed Bensefia**
- **Cherimekh** (Partner)

## 🏛️ Project Architecture & Structure

The codebase is organized into modular components, following strict separation of concerns:

```text
.
├── include/                 # Header files (Definitions & Interfaces)
│   ├── avl.h                # AVL Tree structure and Abstract Machine prototypes
│   ├── paragraph.h          # Text processing and paragraph extraction logic
│   ├── set_op.h             # Set-theoretic operation prototypes
│   ├── menu.h               # UI/UX helper functions and menu structures
│   └── utils.h              # General utility functions
├── src/                     # Source files (Implementations)
│   ├── avl.c                # Core AVL algorithms (Rotations, Rebalancing)
│   ├── paragraph.c          # File I/O, regex-like parsing, and normalization
│   ├── set_op.c             # Set logic implementation at sentence and paragraph levels
│   ├── menu.c               # Interactive CLI logic and terminal animations
│   ├── utils.c              # Utility implementations
│   └── main.c               # Application entry point
├── tests/                   # Automated validation suite
├── makefile                 # Automated build system
└── README.md                # Project documentation
```

## 🧠 Why AVL Trees?

For this lab work, **AVL Trees** were specifically chosen over other dynamic structures (like linked lists or unbalanced BSTs) for the following reasons:
1. **Guaranteed Efficiency:** AVL trees maintain a height-balanced property, ensuring **O(log n)** time complexity for insertion, deletion, and searching. This is vital when processing large text files with thousands of unique words.
2. **Set Integrity:** Set theory requires unique elements. The efficient search capability of AVL trees allows the program to quickly verify if a word already exists before insertion, preventing duplicates.
3. **Lexicographical Ordering:** Being a Binary Search Tree, the AVL tree naturally keeps words sorted, which simplifies the visualization and implementation of certain set operations.

## ✅ Lab Requirements Compliance

This implementation strictly adheres to the requirements specified in the **Lab Work - ADDS** instructions:

### 1. Data Representation (Phase 1)
- **Abstract Machine:** Fully implemented in `avl.c/h`, including primitive operations like `create_node`, `RC` (Right Child), `LC` (Left Child), and rebalancing rotations.
- **Text Normalization:** The `paragraph.c` module handles punctuation removal, case normalization (lowercase), and paragraph detection.

### 2. Set Operations (Phase 2)
- **Implemented Operations:**
  - **Union (A ∪ B):** Merges vocabularies from two different paragraphs or files.
  - **Intersection (A ∩ B):** Identifies common terms shared between texts.
  - **Difference (A ∖ B):** Extracts unique terms present only in the source text.
- **Scope:** Operations can be performed between paragraphs within the same file or across different files, as requested.

### 3. User Interface & Validation (Phase 3)
- **Interactive CLI:** A user-friendly interface in `menu.c`, featuring a splash screen, loading animations, and color-coded results.
- **Memory Management:** Robust cleanup routines ensure the application is leak-free.

## 🛠️ Getting Started

### Compilation & Execution
```bash
make
./adds
```

---
*Developed as part of the Advanced Data Structures (ADDS) coursework - May 2026*
