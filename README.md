# VHDL Lexer, Parser, and Simulator

A simple VHDL sim implementation in C++ that tokenizes VHDL source code, parses the tokens to abstract syntax trees, and evalutaes the trees based on specified inputs.

## Prerequisites

- C++ compiler (g++ or compatible)
- Make (optional, for building)

## Building the Project

### Using g++ directly:

```bash
g++ -o vhdl_sim Main.cpp Lexer.cpp Parser.cpp
```

## Running the Program

After building, you can run the program by providing a VHDL source file as input:

```bash
./vhdl_sim test.vhdl
```

