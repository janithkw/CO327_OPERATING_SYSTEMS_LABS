#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024 // Maximum length of command input
#define MAX_ARGS 64 // Maximum number of arguments in a command

int main()
{
    char command[MAX_CMD_LEN]; // Buffer to hold the command input
    char *args[MAX_ARGS]; // Array to hold the command arguments

    while (1)
    {
        printf("WANASINGHE_SHELL> "); // Print the shell prompt
        fflush(stdout); // Ensure the prompt is printed immediately

        if (fgets(command, MAX_CMD_LEN, stdin) == NULL) // Read command from standard input
            break; //Break if EOF is encountered (EOF means Ctrl+D in terminal)

        // Remove newline
        command[strcspn(command, "\n")] = '\0'; // Remove trailing newline character by replacing it with null terminator

        // Tokenize command into arguments
        int i = 0; // Index for arguments
        char *token = strtok(command, " "); // Split command by spaces
        while (token != NULL && i < MAX_ARGS - 1) // Ensure we don't exceed the maximum number of arguments
        {
            args[i++] = token; // Store the token in the args array
            token = strtok(NULL, " "); // Get the next token
        }
        args[i] = NULL; // Null-terminate the args array to indicate the end of arguments

        if (args[0] == NULL) // If no command was entered, continue to the next iteration
            continue; 
        if (strcmp(args[0], "exit") == 0) // Check if the command is "exit"
            break;

        pid_t pid = fork(); // Create a new process
        if (pid == 0) // Child process
        {
            execvp(args[0], args); //This line will make the child process execute the command
            // If execvp returns, it means there was an error
            perror("exec failed");
            exit(1);
        }
        else
        {
            wait(NULL);
        }
    }

    return 0;
}
