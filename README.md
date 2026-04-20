# Signal-Compiler-CPP

This repository contains the implementation of a compiler for a subset of the **SIGNAL** programming language. Developed as part of the course **"Fundamentals of Compiler Design"** (Основи проєктування трансляторів) at Igor Sikorsky Kyiv Polytechnic Institute.

## Acknowledgments / References

This code was written using the following educational materials, for which I am highly grateful to the authors. I utilized two specific manuals from **O.I. Marchenko** and **O.O. Marchenko**:

1. **Basics of Translator Design. Instructions and Tasks for Laboratory and Computational-Graphical Works**
   * *Authors:* Marchenko O.I., Marchenko O.O.
   * *Publisher:* Igor Sikorsky Kyiv Polytechnic Institute (2021).
   * *URI:* [https://ela.kpi.ua/handle/123456789/45711](https://ela.kpi.ua/handle/123456789/45711)

2. **Basics of Translator Design. Lecture Notes**
   * *Author:* Marchenko O.I.
   * *Publisher:* Igor Sikorsky Kyiv Polytechnic Institute (2021).
   * *URI:* [https://ela.kpi.ua/handle/123456789/45711](https://ela.kpi.ua/handle/123456789/45710)

## Phase 1: Lexical Analyzer (LA)
The current version implements a robust Lexical Analyzer designed to process SIGNAL source code into a stream of tokens. It is based on a deterministic finite automaton (FSM) developed for Variant 12.

### Features
* **Whitespace & Comment Handling**: Automatically skips all types of whitespace and ignores comments formatted as `(* <text> *)`.
* **Token Generation**: Identifies keywords, identifiers, constants, and multi-character delimiters.
* **Memory Management**: Efficiently manages identifiers and constants using custom linked-list structures.
* **Error Detection**: Detects illegal symbols, unclosed comments, and buffer overflows for long lexemes.

## Phase 2: Syntax Analyzer (SA, Parser)
The second phase introduces a Syntax Analyzer built on top of the Lexical Analyzer. It utilizes a **top-down recursive descent parsing** algorithm to validate the grammatical structure of the token stream.

### **Features:**
* **Recursive Descent Parsing:** Accurately parses the token stream according to the variant's subset of the SIGNAL grammar.
* **Abstract Syntax Tree (AST) Generation:** Constructs a hierarchical parse tree representing the logical structure of the source program.
* **Table Integration:** Updates identifier tables with semantic types required for future code generation (e.g., `variable-identifier`, `procedure-identifier`).
* **Syntax Error Detection:** Identifies structural errors and unexpected tokens, providing precise error messages with row and column tracking, and cleanly halts execution.

## Phase 3: Code Generator (CG)
The final phase focuses on semantic analysis, target code generation, and the assembly of the complete compiler pipeline for the SIGNAL programming language subset.

### Features
* **Data Integration:** Seamlessly reads and processes the abstract syntax tree (AST) and identifier tables produced by the Syntax Analyzer.
* **Semantic Analysis:** Validates the semantic correctness of the code and accurately detects semantic errors.
* **Code Generation:** Generates the target machine code or constructs the necessary internal tables for code emission.
* **Pipeline Assembly:** Combines the Lexical Analyzer, Syntax Analyzer, and Code Generator into a single, cohesive compiler application.
* **Comprehensive Error Reporting:** Produces a full listing of the input program, reporting all lexical, syntactic, and semantic errors with precise tracking.

## Future Improvements

While the current version of the compiler is fully functional, there are several architectural optimizations planned for future releases to improve performance and code maintainability:

* **Data Structure Optimization:** Transitioning the internal symbol tables (Identifiers, Constants, Keywords) from custom linked lists to dynamic arrays. This will significantly improve memory access speeds and CPU cache locality.
* **AST Traversal Optimization:** Refactoring the Code Generator to reduce deep recursion during Abstract Syntax Tree (AST) traversal. Implementing iterative approaches or tail-call optimization to prevent potential stack overflow issues on extremely large source files.
* **Modern C++ Memory Management:** Migrating from raw pointers (`new`/`delete`) to modern C++ smart pointers (`std::unique_ptr`, `std::shared_ptr`) for safer memory handling and to strictly prevent memory leaks.

## Project Structure
* `src/` — Implementation files (.cpp).
* `include/` — Header files (.h).
* `tests/` — Test input files with SIGNAL code samples.
* `docs/` — Contains reporting documents for each phase, and `lexer_fsm.png` (The logic of the scanner is governed by the following state machine).

## Build Instructions
The project uses **CMake** to ensure cross-platform compatibility.

1. **Build the project**:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
2. **Run**:
Execute the SignalCompiler binary. When prompted, enter the filename of a test case (e.g., test1.txt).
