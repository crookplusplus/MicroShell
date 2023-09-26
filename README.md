# **MicroShell**
#### **A Simple Shell Implementation in C++**

&nbsp;&nbsp; The Microshell is a lightweight, command-line shell program implemented in C++. It offers basic shell functionality, including executing commands, piping, and error handling. This microshell serves as an educational project and provides insights into working with processes, pipes, and command execution in a Unix-like environment.

## Features
The Microshell showcases key computing concepts for software programming:
- Forking process to create child processes.
- Executing external programs using 'execv' and 'execvp'. 
- Building an interactive command-line interface.
- Implementing error handling.

## **Concepts Explored**
The Microshell provides hands-on experience with several important computing concepts often sought after in employment:

### **Process Management**
- **Parallel Programming through Forking Processes:** &nbsp;The Microshell demonstrates how to fork processes, creating child processes that execute commands. This concept is fundamental for concurrent and parallel programming.

### **Executing External Programs**
The Microshell program showcases the use of **'execv()'** and **'execvp()'** to execute external programs from within a C++ program. This is a crucial skill for developing software that interacts with various system utilities and tools.

- **'execv()':**&nbsp; This method requires the program name and the full path to the executable program.
- **'execvp()':**&nbsp; This method requires the program name and will search for the executable in the 'PATH' variable.

### **Command-Line Interface**
- **Interactive Command Line:**&nbsp;The Microshell offers an interactive command-line interface, where users can enter commands and see their results. Understanding how to build a command-line interface is valuable for creating user-friendly applications

### **Error Handling**
- **Basic Error Handling:**&nbsp;The Microshell incorporates essential error handling mechanisms to verify the existence and accessibility of an executable file. Proficiency in gracefully managing errors is a fundamental skill in software development, ensuring robust and reliable program execution.

## Getting Started
The program relies on a Unix-based operating system to support the forking and execution calls, specifically 'execv' and 'execvp'.

### Prerequisites
Before you can execute this program, make sure you have the following:
- GNU Make 4.2.1 or newer: required to execute the Make instructions to build the program executables.
- g++ Compiler: The GNU compiler is used to build and run the program. the compiler must support C++11. 

## Usage
- Clone this repo to your local machine.

This project is built and operated through the command line interface.<br><br>
 **To begin the program:**

- Open a terminal window and navigate to the folder that contains the project files. 

- Run the following command to build the program:
    _make ./msh_

- Run the following to run the program _./msh_

**To use the program:**<br>

&nbsp;&nbsp;After the program prompts the user with a simulated username of _"cssc1401%"_, The user can proceed to enter shell commands.
- Some Example Commands:
    - ls -la
    - ls -la | sort -fi
    - ls -la | sort -fi | grep bash.src

<u>Note:</u> &nbsp; More than three commands piped together and commands with more than one optional argument are not allowed. 


**To exit the program**:
- enter _exit_.