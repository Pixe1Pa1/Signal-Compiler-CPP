# Signal-Compiler-CPP

This repository contains the implementation of a compiler for a subset of the **SIGNAL** programming language. Developed as part of the course **"Fundamentals of Compiler Design"** (Основи проєктування трансляторів) at Igor Sikorsky Kyiv Polytechnic Institute.

## Phase 1: Lexical Analyzer (LA)

The current version implements a robust Lexical Analyzer designed to process SIGNAL source code into a stream of tokens. It is based on a deterministic finite automaton (FSM) developed for Variant 12.

### Features
* **Whitespace & Comment Handling**: Automatically skips all types of whitespace and ignores comments formatted as `(* <text> *)`.
* **Token Generation**: Identifies keywords, identifiers, constants, and multi-character delimiters.
* **Memory Management**: Efficiently manages identifiers and constants using custom linked-list structures.
* **Error Detection**: Detects illegal symbols, unclosed comments, and buffer overflows for long lexemes.

## Project Structure
* `src/` — Implementation files (.cpp).
* `include/` — Header files (.h).
* `tests/` — Test input files with SIGNAL code samples.
* `docs/` — Contains `reportLA.pdf` (university lab report) and `lexer_fsm.png` (The logic of the scanner is governed by the following state machine).

## Build Instructions
The project uses **CMake** to ensure cross-platform compatibility.

1. **Build the project**:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```
2. **Run**:
Execute the SignalCompiler binary. When prompted, enter the filename of a test case (e.g., test1.txt).
