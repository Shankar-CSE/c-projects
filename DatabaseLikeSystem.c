#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMD_LEN 512
#define MAX_TABLE_NAME 50
#define MAX_COL_NAME 50
#define MAX_COLS 10
#define MAX_ROW_LEN 1024

// --- Function Prototypes ---
void handle_create(char *cmd);
void handle_insert(char *cmd);
void handle_select(char *cmd);
void trim_whitespace(char *str);

// --- Main Function ---
int main() {
    char cmd[MAX_CMD_LEN];
    printf("MiniSQL Engine. Use CREATE, INSERT, SELECT, or 'exit'.\n");

    while (1) {
        printf("minisql> ");
        fgets(cmd, sizeof(cmd), stdin);

        if (strncmp(cmd, "exit", 4) == 0) break;
        if (strncmp(cmd, "CREATE", 6) == 0) handle_create(cmd);
        else if (strncmp(cmd, "INSERT", 6) == 0) handle_insert(cmd);
        else if (strncmp(cmd, "SELECT", 6) == 0) handle_select(cmd);
        else printf("Unknown command.\n");
    }
    return 0;
}

// --- Core Logic ---

void trim_whitespace(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

void handle_create(char *cmd) {
    char table_name[MAX_TABLE_NAME];
    char schema_filename[MAX_TABLE_NAME + 4];
    char data_filename[MAX_TABLE_NAME + 4];

    // CREATE TABLE table_name (col1, col2, ...)
    sscanf(cmd, "CREATE TABLE %s", table_name);
    table_name[strcspn(table_name, " (")] = 0;

    sprintf(schema_filename, "%s.sch", table_name);
    sprintf(data_filename, "%s.dat", table_name);

    char *cols_str = strchr(cmd, '(');
    if (!cols_str) {
        printf("Invalid CREATE syntax.\n");
        return;
    }
    cols_str++; // Skip '('
    char *end_cols = strchr(cols_str, ')');
    if (end_cols) *end_cols = '\0';

    FILE *fp = fopen(schema_filename, "w");
    char *token = strtok(cols_str, ",");
    while (token) {
        trim_whitespace(token);
        fprintf(fp, "%s\n", token);
        token = strtok(NULL, ",");
    }
    fclose(fp);

    // Create empty data file
    fp = fopen(data_filename, "w");
    fclose(fp);

    printf("Table '%s' created.\n", table_name);
}

void handle_insert(char *cmd) {
    char table_name[MAX_TABLE_NAME];
    char data_filename[MAX_TABLE_NAME + 4];

    // INSERT INTO table_name VALUES (val1, val2, ...)
    sscanf(cmd, "INSERT INTO %s", table_name);
    
    char *values_str = strstr(cmd, "VALUES (");
    if (!values_str) {
        printf("Invalid INSERT syntax.\n");
        return;
    }
    values_str += strlen("VALUES (");
    char *end_values = strrchr(values_str, ')');
    if (end_values) *end_values = '\0';

    sprintf(data_filename, "%s.dat", table_name);
    FILE *fp = fopen(data_filename, "a");
    if (!fp) {
        printf("Table '%s' does not exist.\n", table_name);
        return;
    }

    char *token = strtok(values_str, ",");
    int first = 1;
    while (token) {
        trim_whitespace(token);
        // Remove quotes if they exist
        if (token[0] == '\'' || token[0] == '"') token++;
        if (token[strlen(token)-1] == '\'' || token[strlen(token)-1] == '"') token[strlen(token)-1] = '\0';
        
        fprintf(fp, "%s%s", (first ? "" : ","), token);
        first = 0;
        token = strtok(NULL, ",");
    }
    fprintf(fp, "\n");
    fclose(fp);

    printf("Row inserted into '%s'.\n", table_name);
}

void handle_select(char *cmd) {
    char table_name[MAX_TABLE_NAME];
    char data_filename[MAX_TABLE_NAME + 4];
    char schema_filename[MAX_TABLE_NAME + 4];
    char cols[MAX_COLS][MAX_COL_NAME];
    int col_count = 0;

    // SELECT * FROM table_name
    sscanf(cmd, "SELECT * FROM %s", table_name);
    table_name[strcspn(table_name, ";\n")] = 0;

    sprintf(data_filename, "%s.dat", table_name);
    sprintf(schema_filename, "%s.sch", table_name);

    FILE *schema_fp = fopen(schema_filename, "r");
    if (!schema_fp) {
        printf("Table '%s' does not exist.\n", table_name);
        return;
    }
    while (col_count < MAX_COLS && fscanf(schema_fp, "%s", cols[col_count]) == 1) {
        printf("%-20s", cols[col_count]);
        col_count++;
    }
    printf("\n");
    for(int i=0; i<col_count*20; i++) printf("-");
    printf("\n");
    fclose(schema_fp);

    FILE *data_fp = fopen(data_filename, "r");
    char line[MAX_ROW_LEN];
    while (fgets(line, sizeof(line), data_fp)) {
        line[strcspn(line, "\n")] = 0;
        char *token = strtok(line, ",");
        while (token) {
            printf("%-20s", token);
            token = strtok(NULL, ",");
        }
        printf("\n");
    }
    fclose(data_fp);
}
