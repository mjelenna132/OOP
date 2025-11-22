# OOP1 Project Assignment – Command Line Interpreter  
### ETF – School Year 2024/2025  
Based on: "Projektni zadatak 2024 – v1.1".

---

## Overview  
The goal of this project is to implement a simplified command line interpreter similar to those used in operating systems.  
The interpreter reads commands from standard input and executes them one by one.  
Commands may be entered interactively or provided through a batch text file.

---

## Functional Requirements  

### **Command Execution**
- Read commands from input  
- Parse command names, arguments, and options  
- Execute supported commands  

### **Lexical Rules**
- Tokenization of input text  
- Handling quoted arguments  
- Distinguishing commands, options, arguments, and streams  

### **Input/Output Handling**
- Standard input and output  
- Optional redirection (in later phases)  
- Support for commands using files and arguments  

### **Supported Commands**
- `echo`  
- `prompt`  
- `time`  
- `date`  
- `touch`  
- `truncate`  
- `rm`  
- `wc`  
- `tr`  
- `head`  
- `batch`

### **Pipelines**
Support for connecting the output of one command to another.

### **Error Handling**
Must detect:  
- Lexical errors  
- Syntactic errors  
- Semantic errors  
- Command-specific errors  
- System errors (missing files, permissions, etc.)

---

## Project Phases  

### **Phase 1**
- Basic interpreter  
- Commands: `echo`, `time`, `date`, `touch`, `wc`  
- No redirection or pipelines  
- Only semantic and OS-level errors  
- All inputs syntactically correct  

### **Phase 2**
- Full interpreter functionality  
- Complete error handling  
- Support for redirection and pipelines  
- All specified commands implemented  

---

## Evaluation Criteria  

### **1. Style and Readability**
Clear naming, indentation, comments, and overall structure.

### **2. Object-Oriented Design**
Correct use of abstraction, interfaces, class responsibilities, and modularity.

### **3. Correctness of Implementation**
Proper use of inheritance, encapsulation, and OOP techniques.  
The project must compile and link successfully.



