/**
 * Authored by Chris Rook
*/

#ifndef MICROSHELL_H
#define MICROSHELL_H

#include <vector>
#include <string>

#define MAX_ARGUMENTS 2 //used check argument size includes one executable and one argument at most, and exit code
#define NUM_FD 2 //Number of file descriptor for read/write of pipes
#define FORK_FAILURE 5 //used in the even of a fork failure
#define BAD_PIPE 6//used in the event of failed pipe creation 
#define BAD_EXEC 7//used in the event of a failed exec call

namespace MSH{
    //Main driver function that uses a while loop to keep accepting inputs and outputting non critical errors
    void run(void);
    //Helper method that outputs the username at the start of a newline
    void prompt(void);
    //Helper method that parses Commands from one input string to a vector of strings and counts the number of pipes if any
    //std::string line: input string
    //int* pPipeCount: points to int in run() that holds the number of pipes
    //returns vector of each individual string entered by the user
    std::vector<std::string> parseCommands(std::string line, int* pPipeCount);
    //Method that further parses command line args for pipe methods
    //std::vector<std::string> commands: vector of each individual string entered by the user
    //int* pPipes: point to int variable in run() that holds the number of pipes from input
    //returns a vector of vector<string> that separate each executable and its optional argument if any
    std::vector<std::vector<std::string>> prepPipe(std::vector<std::string>& commands, int* pPipes);
    //Helper method that checks the entered file for a search requirement or if the file is a full path executable
    //std::string fileName: filename to be checked
    bool searchRequired(std::string fileName);
    //Helper method that checks the entered file to see if it actually an executable
    //std::string fileName: filename to be checked
    bool executableCheck(std::string filename);
    //An overloaded method; This method launches an executable process that does not require a pipe to another process.
    //std::vector<std::string> commands: vector that contains executable file name and optional argument if any
    void launchProgram(std::vector<std::string> commands);
    //An overlooaded method. This method launches an executable process and allocates pipe resources for interprocess communication.
    //std::vector<std::string> argument: vector that contains executable file name and optional argument if any
    //int readEnd: contains the read file descriptor for the process
    //int writeEnd: contains the write file descriptor for the process
    void launchProgram(std::vector<std::string> arguments, int readEnd, int writeEnd);
    //Method that initializes a vector of pipes and initializes them. Method changes arguments from strings to char* for the exec() call.
    //     The method determines which read and write resource a process needs and calls the executePipeline() method.
    //std::vector<std::vector<std::string>> commands: vector of vector<strings> that contains executable file name and optional argument if any
    void executePipeline(std::vector<std::vector<std::string>> commands);
}


#endif