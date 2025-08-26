#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_CMD_LEN 256
#define MAX_PATH_LEN 1024

// --- Function Prototypes ---
void command_loop();
void do_ls(const char *path);
void do_cd(const char *path);
void do_pwd();
void do_mkdir(const char *path);
void do_rm(const char *path);

// --- Main Function ---
int main() {
    printf("Welcome to the Unix-like File Explorer!\n");
    printf("Available commands: ls, cd, pwd, mkdir, rm, exit\n");
    command_loop();
    return 0;
}

// --- Core Logic ---

void command_loop() {
    char cmd_line[MAX_CMD_LEN];
    char cwd[MAX_PATH_LEN];

    while (1) {
        do_pwd(); // Show current directory as prompt
        printf("$ ");
        
        if (fgets(cmd_line, sizeof(cmd_line), stdin) == NULL) {
            break; // EOF
        }

        // Remove trailing newline
        cmd_line[strcspn(cmd_line, "\n")] = 0;

        // Tokenize command
        char *command = strtok(cmd_line, " ");
        char *argument = strtok(NULL, " ");

        if (command == NULL) {
            continue; // Empty line
        }

        if (strcmp(command, "ls") == 0) {
            do_ls(argument ? argument : ".");
        } else if (strcmp(command, "cd") == 0) {
            do_cd(argument);
        } else if (strcmp(command, "pwd") == 0) {
            do_pwd();
            printf("\n");
        } else if (strcmp(command, "mkdir") == 0) {
            do_mkdir(argument);
        } else if (strcmp(command, "rm") == 0) {
            do_rm(argument);
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Command not found: %s\n", command);
        }
    }
}

// --- Command Implementations ---

void do_ls(const char *path) {
    DIR *d = opendir(path);
    if (d == NULL) {
        perror("ls error");
        return;
    }
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
        // Skip hidden files '.' and '..'
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
            printf("%s\n", dir->d_name);
        }
    }
    closedir(d);
}

void do_cd(const char *path) {
    if (path == NULL) {
        fprintf(stderr, "cd: missing operand\n");
        return;
    }
    if (chdir(path) != 0) {
        perror("cd error");
    }
}

void do_pwd() {
    char cwd[MAX_PATH_LEN];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s", cwd);
    } else {
        perror("pwd error");
    }
}

void do_mkdir(const char *path) {
    if (path == NULL) {
        fprintf(stderr, "mkdir: missing operand\n");
        return;
    }
    // The MinGW/Windows version of mkdir takes only one argument
    if (mkdir(path) != 0) {
        perror("mkdir error");
    }
}

void do_rm(const char *path) {
    if (path == NULL) {
        fprintf(stderr, "rm: missing operand\n");
        return;
    }
    if (remove(path) != 0) {
        perror("rm error");
    }
}
