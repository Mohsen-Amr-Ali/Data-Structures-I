# Data Structures I - Alexandria University  
**Faculty of Engineering · Computer & Communication Program · Spring 2025**  

This repository contains implementations of key data structures and algorithms developed for the **Data Structures I (CSE127)** course. Each assignment includes a detailed problem statement, C source code, executable files, and enhanced user interfaces that extend beyond base requirements.  

---

## Table of Contents  
1. [Stacks](#stacks)  
2. [Queues](#queues)  
3. [Linked Lists](#linked-lists)  
4. [Expression Evaluation](#expression-evaluation)  
5. [BST Dictionary (AVL Tree)](#bst-dictionary-avl-tree)  
6. [Class Materials](#class-materials)  

---

### Stacks  
**Description**: Implementation of a stack using arrays and linked lists, with advanced utility functions.  
**Key Features**:  
- Core operations: `push`, `pop`, `peek`, `isEmpty`, `isFull`, `display`.  
- Advanced functions:  
  - String reversal using stacks.  
  - Balanced parentheses checker.  
  - Prime number deletion in stacks.  
  - Merging sorted stacks.  
**Enhancements**: Interactive menu-driven UI for testing all functions.  

---

### Queues  
**Description**: Queue operations using arrays, including complex modifications.  
**Key Features**:  
- Core operations: `enqueue`, `dequeue`, `isEmpty`, `isFull`, `display`.  
- Advanced functions:  
  - Max value finder.  
  - Odd integer reversal in queues.  
  - First `k` elements reversal.  
  - Index-based insertion.  
**Enhancements**: Robust error handling and user-friendly input prompts.  

---

### Linked Lists  
**Description**: Single linked list operations with extended functionalities.  
**Key Features**:  
- Core operations: Insertion/deletion at beginning/end, traversal.  
- Advanced functions:  
  - Search by value.  
  - Index-based insertion.  
  - List merging and intersection.  
  - Identical list checker.  
**Enhancements**: Modular design with a unified menu system for testing all operations.  

---

### Expression Evaluation  
**Description**: Infix-to-postfix converter and postfix evaluator using stacks.  
**Key Features**:  
- Handles multi-digit numbers, floating points, negatives, brackets, and power operations (`^`).  
- Algorithms:  
  - `infixToPostfix()`: Shunting-yard algorithm implementation.  
  - `evaluatePostfix()`: Stack-based evaluation.  
**Enhancements**: Supports complex expressions (e.g., `2 + (-2.5 + 3.14) * (-5.4 + 8.1)^-0.5`).  

---

### BST Dictionary (AVL Tree)  
**Description**: Spell-checking system using a self-balancing AVL Tree for optimal performance.  
**Key Features**:  
- **AVL Tree** implementation to ensure O(log n) search/insert complexity.  
- Functions:  
  - Dictionary loading with word count and tree height reporting.  
  - Sentence spell-checking with incorrect word suggestions (predecessor, successor, and last-visited node).  
- Bonus: Comparison with unbalanced BST for demonstration.  
**Enhancements**: Streamlined CLI with color-coded outputs and detailed error diagnostics.  

---

### Class Materials  
Access lecture slides, supplementary resources, and additional course materials here:  
[Class Materials (Google Drive Folder)](https://drive.google.com/drive/folders/1us8fvuYPY9d5-Z5Pd77q9njaK5E_mRE-)  

---

## Repository Structure  
Each assignment folder contains:  
- `README.pdf`: Original problem statement.  
- `.c file`: Complete source code.  
- `.exe`: Pre-compiled executable (Windows).  
- Additional notes on extended functionalities.  

---

## Notes  
- **Independent Work**: Completed all assignments individually despite the course recommendation to collaborate in groups of two.  
- **Beyond Requirements**: Added advanced UIs, input validation, and performance optimizations.  
- **AVL Tree**: Chosen for the BST Dictionary to ensure efficient search operations, as highlighted in the bonus section.  
- **Skills Demonstrated**: Self-reliance in full-cycle development, debugging, and implementation of complex data structures.  

Explore the code to see how core data structures are applied to solve real-world problems!  

*Disclaimer: This repository is for academic purposes and adheres to Alexandria University's honor code.*  
