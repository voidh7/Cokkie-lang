#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef void (*CommandFunc)(const char *arg);

typedef struct {
    const char *name;
    CommandFunc func;
} Command;

void cmd_es_println(const char *arg) {
    if (arg != NULL) {
        printf("%s\n", arg);
    } else {
        printf("erro:arg not exist");
    }
}

void cmd_print(const char *arg) {
    if (arg != NULL) {
        printf("%s", arg);
    } else {
        printf("erro:arg not exist");
    }
}

void n(const char *arg) {
    (void)arg;
    printf("\n");
}

void end(const char *arg) {
    (void)arg;
    exit(0);
}

void cmd_clear(const char *arg) {
    (void)arg;
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void cmd_exec(const char *arg) {
    if (arg != NULL) {
        system(arg);
    } else {
        printf("erro:arg not exist");
    }
}

void out_printErr(const char *arg) {
    if (arg != NULL) {
        printf("\033[31m%s\033[0m\n", arg);
    } else {
        printf("erro:arg not exist");
    }
}

const Command command_table[] = {
    {"out.println", cmd_es_println},
    {"out.printErr", out_printErr},
    {"out.print", cmd_print},
    {"/clear", cmd_clear},
    {"/n", n},
    {"end", end},
    {"system.exec", cmd_exec},
    {NULL, NULL}
};

void trim(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r' || isspace(str[len - 1])))
        str[--len] = '\0';
}

void split_token(const char *line, char *cmd, char *arg) {
    const char *p = strchr(line, '(');
    if (!p) {
        strcpy(cmd, line);
        arg[0] = '\0';
        return;
    }

    size_t len = p - line;
    strncpy(cmd, line, len);
    cmd[len] = '\0';

    const char *start = p + 1;
    const char *end = strchr(start, ')');
    if (end) {
        strncpy(arg, start, end - start);
        arg[end - start] = '\0';
    } else {
        arg[0] = '\0';
    }
}

const Command *get_command(const char *name) {
    for (int i = 0; command_table[i].name; i++) {
        if (strcmp(name, command_table[i].name) == 0)
            return &command_table[i];
    }
    return NULL;
}

int main() {
    FILE *f = fopen("main.cokkie", "r");
    if (!f) {
        printf("Erro ao abrir main.cokkie\n");
        return 1;
    }

    char line[256];
    int inside = 0;

    while (fgets(line, sizeof(line), f)) {
        trim(line);

        if (strcmp(line, "Function main() {") == 0) {
            inside = 1;
            continue;
        }
        if (strcmp(line, "}") == 0) {
            inside = 0;
            continue;
        }
        if (!inside || line[0] == '\0') continue;

        char cmd[64], arg[192];
        split_token(line, cmd, arg);

        const Command *c = get_command(cmd);
        if (c) {
            c->func(arg);
        } else {
            printf(" erro Comando desconhecido: %s\n", cmd);
        }
    }

    fclose(f);
    return 0;
}