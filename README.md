# VHDL Lexer

A simple VHDL lexer implementation in C++ that tokenizes VHDL source code.

## Prerequisites

- C++ compiler (g++ or compatible)
- Make (optional, for building)

## Building the Project

### Using g++ directly:

```bash
g++ -o vhdl_lexer Main.cpp Lexer.cpp
```

## Running the Program

After building, you can run the program by providing a VHDL source file as input:

```bash
./vhdl_lexer input_file.vhd
```

The program will read the VHDL file and output the tokens found in the source code.

## Project Structure

- `Main.cpp` - Main program entry point
- `Lexer.h` - Lexer class header
- `Lexer.cpp` - Lexer implementation
- `Token.h` - Token class definition

## Example Usage

```bash
# Compile the program
g++ -o vhdl_lexer Main.cpp Lexer.cpp

# Run with a VHDL file
./vhdl_lexer example.vhd
```
