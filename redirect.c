#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void execute_command(char *input, char *cmd, char *output) {
    // Split the command into program and arguments
    char *args[10];
    int i = 0;
    args[i] = strtok(cmd, " ");
    while (args[i] != NULL) {
        args[++i] = strtok(NULL, " ");
    }

    // Fork a new process
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        int fd_in, fd_out;

        // Input redirection if not "-"
        if (strcmp(input, "-") != 0) {
            fd_in = open(input, O_RDONLY);
            if (fd_in < 0) {
                perror("Failed to open input file");
                exit(1);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        // Output redirection if not "-"
        if (strcmp(output, "-") != 0) {
            fd_out = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0) {
                perror("Failed to open output file");
                exit(1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

        execvp(args[0], args);
        perror("Exec failed");
        exit(1);
    } else {
        wait(NULL);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <inp> <cmd> <out>\n", argv[0]);
        exit(1);
    }
    
    execute_command(argv[1], argv[2], argv[3]);
    return 0;
}
