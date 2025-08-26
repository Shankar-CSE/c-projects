#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <process.h> // For _spawnvp
#define chdir _chdir
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

// --- Function Prototypes ---
void parse_command(char *cmd, char **args, int *is_background);
void execute_command(char **args, int is_background);
void execute_pipe(char **args1, char **args2);

// --- Main Function ---
int main() {
    char cmd_line[MAX_CMD_LEN];
    char *args[MAX_ARGS];
    char *pipe_args[2][MAX_ARGS] = {{NULL}, {NULL}};
    int is_background;

    while (1) {
        printf("mini-shell> ");
        fflush(stdout);

        if (fgets(cmd_line, sizeof(cmd_line), stdin) == NULL) {
            break; // EOF
        }

#ifdef _WIN32
        if (strchr(cmd_line, '|')) {
            printf("Pipes are not supported on this version for Windows.\n");
            continue;
        }
#else
        // Check for pipe
        char *pipe_pos = strchr(cmd_line, '|');
        if (pipe_pos) {
            *pipe_pos = '\0'; // Split the command line into two parts
            char *cmd1_str = cmd_line;
            char *cmd2_str = pipe_pos + 1;

            parse_command(cmd1_str, pipe_args[0], &is_background);
            parse_command(cmd2_str, pipe_args[1], &is_background);
            execute_pipe(pipe_args[0], pipe_args[1]);
        } else
#endif
        {
            parse_command(cmd_line, args, &is_background);

            if (args[0] == NULL) {
                continue; // Empty command
            }
            if (strcmp(args[0], "exit") == 0) {
                break;
            }
            if (strcmp(args[0], "cd") == 0) {
                if (args[1] == NULL) {
                    fprintf(stderr, "cd: expected argument\n");
                } else {
                    if (chdir(args[1]) != 0) {
                        perror("cd");
                    }
                }
                continue;
            }
            execute_command(args, is_background);
        }
    }
    return 0;
}

// --- Core Logic ---

void parse_command(char *cmd, char **args, int *is_background) {
    int i = 0;
    cmd[strcspn(cmd, "\n")] = 0; // Remove trailing newline

    // Check for background process
    *is_background = 0;
    char *bg_pos = strchr(cmd, '&');
    if (bg_pos) {
        *is_background = 1;
        *bg_pos = '\0';
    }

    char *token = strtok(cmd, " \t\n");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

void execute_command(char **args, int is_background) {
#ifdef _WIN32
    int mode = is_background ? _P_NOWAIT : _P_WAIT;
    // _spawnvp returns -1 on error.
    intptr_t pid = _spawnvp(mode, args[0], args);
    if (pid == -1) {
        perror(args[0]);
    } else if (is_background) {
        printf("Process %d running in background\n", (int)pid);
    }
#else
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) { // Child process
        if (execvp(args[0], args) < 0) {
            perror(args[0]);
            exit(1);
        }
    } else { // Parent process
        if (!is_background) {
            waitpid(pid, NULL, 0);
        } else {
            printf("Process %d running in background\n", pid);
        }
    }
#endif
}

#ifndef _WIN32
// Piping is a POSIX-specific feature in this implementation
void execute_pipe(char **args1, char **args2) {
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0) {
        perror("pipe failed");
        return;
    }

    p1 = fork();
    if (p1 < 0) {
        perror("fork failed");
        return;
    }

    if (p1 == 0) { // Child 1 (writes to pipe)
        close(pipefd[0]); // Close read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[1]);
        if (execvp(args1[0], args1) < 0) {
            perror(args1[0]);
            exit(1);
        }
    }

    p2 = fork();
    if (p2 < 0) {
        perror("fork failed");
        return;
    }

    if (p2 == 0) { // Child 2 (reads from pipe)
        close(pipefd[1]); // Close write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe
        close(pipefd[0]);
        if (execvp(args2[0], args2) < 0) {
            perror(args2[0]);
            exit(1);
        }
    }

    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
}
#endif
