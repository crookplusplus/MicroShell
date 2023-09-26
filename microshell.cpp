/**
 * Authored by Chris Rook
*/

#include "microshell.h"
#include <iostream>
#include <vector>
#include <array>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>



/// @brief contains methods for functionality of the microshell
namespace MSH{
    
    /// @brief Main function that accepts command line arguments and continues to process them until user
            ///enters "exit"
    void run(){
        std::string input;
        bool exitCommanded = false;
        int pipeCount = 0;
        int* pPipeCount = &pipeCount;

        //while loop keeps parsing arguments until critical error or "exit"
        while (!exitCommanded){
            prompt();
            std::getline(std::cin, input);

            //exit command
            if(input == "exit"){
                exitCommanded = true;
            }
            else {
                std::vector<std::string> commands = parseCommands(input, pPipeCount); //command line argumets
                if (commands.empty()){
                    continue;
                }
                else if (pipeCount==0){
                    launchProgram(commands);
                }
                else if (pipeCount>0){
                    std::vector<std::vector<std::string>> pipeArgs = prepPipe(commands, pPipeCount);
                    //informs user that executable is missing after "|"
                    if (pipeArgs[0][0] == "Error"){
                        std::cout<<"Executable file expected after \"|\""<<std::endl;
                    }
                    else{
                        executePipeline(pipeArgs);
                    }
                }
                pipeCount = 0;
            }
            
        }

    }

    
    /// @brief Helper method that outputs username
    void prompt(){
    std::string username = "cssc1401";
    std::cout<<username<<"% ";
    }

    /// @brief Help method that parses command line arguments and counts the number of pipes
    /// @param line String if input commands
    /// @param pPipeCount int* points to the pipeCount variable in the run() method
    /// @return vector of strings that contain args and/or "|"
    std::vector<std::string> parseCommands(std::string line, int* pPipeCount){
        std::vector<std::string> commands;
        std::string arg = "";

        for (auto x : line){
            if (x == ' '){
                commands.push_back(arg);
                arg = "";
            }
            else {
                arg = arg + x;
                if (x =='|'){
                    (*pPipeCount)++;
                }
            }
        }
        commands.push_back(arg);
        return commands;
    }

    /// @brief Method that further parses command line arguments and preps them for pipe related methods
    /// @param commands Vector that contains individual commands and pipes
    /// @param pPipes int* that points to the pipeCount found in run()
    /// @return A vector that contains vectors of strings for executable commands and their arguments if any
    std::vector<std::vector<std::string>> prepPipe(std::vector<std::string>& commands, int* pPipes){
        int index=0;
        int numCommands = commands.size();
        std::vector<std::vector<std::string>> args(*pPipes+1);

        for(int i=0;i<numCommands;i++){
            //checks for argument following "|"
            if((commands[i]=="|")&&(commands[i]==commands.back())){
                args[0][0]="Error";
            }
            else if(commands[i]=="|"){
                index++;
            }
            else{
                args[index].push_back(commands[i]);
            }
        }
        return args;
    }

    /// @brief Helper method that checks if an executable needs to be searched for. This is useful for finding the 
    ///        correct exec() function to call and checking if the executable is actually an executable in the first place
    /// @param fileName Name of executable file
    /// @return True if needs to be searched; false otherwise
    bool searchRequired(std::string fileName){
        if (fileName[0]=='/'){
            return false;
        }
        return true;
    }

    /// @brief Method that checks if the executable file is actually an executable or not
    /// @param filename Name of executable file
    /// @return True if executable; false otherwise
    bool executableCheck(std::string filename){
        //uses PATH variable to cheeck for file access for executable
        const char* path = std::getenv("PATH");
        
        //checks if the filename contains '/' at filename[0]
        if(searchRequired(filename)){

            if(path==nullptr){
                std::cout<<"Error: PATH environment variable not set"<<std::endl;
                return false;
            }

            std::string pathCopy(path);
            std::string fullPath;
            //tokenizes the PATH for directories
            const char* dir = std::strtok(const_cast<char*>(pathCopy.c_str()), ":");

            while (dir != nullptr){
                fullPath = dir;
                fullPath += "/";

                std::string tempPath = fullPath;
                tempPath += filename;
                
                //checks if file is executable
                if (access(tempPath.c_str(), X_OK)==0){
                    return true;
                }
                //grabs next dir
                dir = std::strtok(nullptr, ":");
            }

        }
        
        return access(filename.c_str(), X_OK) == 0;
    }


    /// @brief Method that to launch executable file and their arguments in the event that no pipe 
    //          was entered in its own process. 
    /// @param commands Parsed command line args
    void launchProgram(std::vector<std::string> commands){

        //checks for proper amount of optional arguments for the executable
        if(commands.size()>MAX_ARGUMENTS){
            std::cout<<"Too many arguments entered for the executable: \""<<commands[0]<<std::endl;
            return;
        }
        
        //checks if file is an actual executable or not
        if (!executableCheck(commands[0])){
            std::cout<<"The file entered, \""<<commands[0]<<"\", is not executable."<<std::endl;
            return;
        }

        //File is executable with proper arguments
        else{

            //prep the args for exec() call
            std::vector<char*> args;
            for(const auto& arg : commands){
                args.push_back(const_cast<char*>(arg.c_str()));
            }
            args.push_back(nullptr);

            if(searchRequired(commands[0])){
                //create new process;
                pid_t pid = fork();
                //error checking for the fork();
                if(pid<0){
                    std::cout<<"Fork failed on "<<commands[0]<<"."<<std::endl;
                    std::exit(FORK_FAILURE);
                }
                //child process
                else if (pid==0){
                    //exec call for the child process;
                    execvp(args[0], args.data());
                    std::cout<<"Failed to execute process: "<<commands[0];
                    std::exit(BAD_EXEC);
                }
                else if(pid>0){
                    //wait call for the parent process;
                    wait(nullptr);
                }

            }
            else{

                //create new process;
                pid_t pid = fork();
                //error checking for the fork();
                if(pid<0){
                    std::cout<<"Fork failed on \""<<commands[0]<<"\""<<"."<<std::endl;
                    std::exit(FORK_FAILURE);
                }
                //child process
                else if (pid==0){
                    //exec call for the child process;
                    execv(args[0], args.data());
                    std::cout<<"Failed to execute process: \""<<commands[0]<<"\"";
                    std::exit(BAD_EXEC);
                }
                else if(pid>0){
                    //wait call for the parent process;
                    wait(nullptr);
                }
            }
        }
    }

    /// @brief Overloaded method that is used to launch executables that communicate with pipes in their own process. Parent
    ///         process handle pipe resource clean up for proper synchronization and closing pipe assets
    /// @param arguments vector that holds executable command and associate argument if any
    /// @param readEnd Contains the read resource file descriptor used by the executable
    /// @param writeEnd Contains the write resource file descriptor used by the executable
    void launchProgram(std::vector<std::string> arguments, int readEnd, int writeEnd) {
    
        //argument bounds checking
        if(arguments.size()>MAX_ARGUMENTS){
            std::cout<<"Too many arguments entered for the executable: \""<<arguments[0]<<std::endl;
            return;
        }
        //is executable file check
        if (!executableCheck(arguments[0])){
            std::cout<<"The file entered, \""<<arguments[0]<<"\", is not executable."<<std::endl;
            return;
        }
    
        // Child process
        pid_t pid = fork();
        if (pid == 0) {

            // Remap stdin/stdout to the appropriate pipe resource and error check
            if(dup2(readEnd, STDIN_FILENO)==-1){
                std::cout<<"Error remapping file descriptors, readEnd: "<<readEnd
                <<", writeEnd: "<<writeEnd<<" on Executable: "<<arguments[0]<<"."
                "This error occured attempting to remap input source."<<std::endl;
            }

            if (dup2(writeEnd, STDOUT_FILENO)==-1){
                std::cout<<"Error remapping file descriptors, readEnd: "<<readEnd
                <<", writeEnd: "<<writeEnd<<" on Executable: "<<arguments[0]<<"."
                "This error occured attempting to remap output source."<<std::endl;
            }
            
            //prep the args for exec() call
            std::vector<char*> args;
            for(const auto& arg : arguments){
                args.push_back(const_cast<char*>(arg.c_str()));
            }
            args.push_back(nullptr); //add for exec() call arg format

            if(searchRequired(arguments[0])){
                // Execute the program
                execvp(args[0], (args.data()));
                std::cerr << "Error: Failed to execute program" << std::endl;
                std::exit(BAD_EXEC);
            }
            else {
                execv(args[0], (args.data()));
                std::cerr << "Error: Failed to execute program" << std::endl;
                std::exit(BAD_EXEC);
            }

         // Parent process
        } else if (pid > 0) {
            // Wait for the child process to finish
            wait(nullptr);

            //Close pipe resources
            if (readEnd != 0){
                close(readEnd);
            }
            if (writeEnd != 1){
                close(writeEnd);
            }

        } else {
            // Fork failed; pid_t = -1 case
            std::cerr << "Error: Fork failed" << std::endl;
        }
    }

    /// @brief Method that declares and initializes pipes based on the number of commands
    /// @param commands Vector that contains vectors of strings for the executable name and its argument if any
    void executePipeline(std::vector<std::vector<std::string>> commands) {
        int numCommands = commands.size();
        int numPipes = numCommands -1;
        std::vector<std::array<int, NUM_FD>> pipes(numPipes);

        // Create pipes
        for (int i = 0; i < numPipes; ++i) {

            if (pipe(pipes[i].data()) == -1) {
                std::cerr << "Error: Failed to create pipe" << std::endl;
                return;
            }
        }
        

        // Execute commands in pipeline
        for (int i = 0; i < numCommands; ++i) {
            std::vector<std::string> command;
            int commandSize = commands[i].size(); //used for compiler warning elimination
            for(int j=0;j<commandSize;j++){
                command.push_back(commands[i][j]);
            }
            //ternary statements to determine pipe resource required for process
            int readEnd = i > 0 ? pipes[i - 1][0] : STDIN_FILENO;
            int writeEnd = i < numPipes ? pipes[i][1] : STDOUT_FILENO;
            //calls overloaded method to launch executable command in a process
            launchProgram(command, readEnd, writeEnd);
        }
    }

}
