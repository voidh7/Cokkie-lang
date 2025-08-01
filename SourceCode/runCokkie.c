#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define RED "\033[1;31m"
#define RESET "\033[0m"

typedef void (*CommandFunc)(const char *arg);

typedef struct {
    const char *name;
    CommandFunc func;
} Command;

#include <stdio.h>
#include <string.h>

// Função utilitária para obter apenas o nome do arquivo
// Usa o último encontrado entre '/' e '\' para determinar o separador
// Retornando o caminho passado por argumento caso não encontre nenhum dos dois
const char *get_basename(const char *path) {
    const char *slash_base = strrchr(path, '/');
    const char *backslash_base = strrchr(path, '\\');

    const char *base = slash_base > backslash_base ? slash_base : backslash_base;
    return base ? base + 1 : path;
}

void log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, RED);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, RESET);
    va_end(args);
}

void fatal_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, RED);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, RESET);
    va_end(args);
    exit(1);
}

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
        log_error("Error: Arg cannot be NULL");
    }
}

void out_printErr(const char *arg) {
    if (arg != NULL) {
        printf("\033[31m%s\033[0m\n", arg);
    } else {
        log_error("Error: Arg cannot be NULL");
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

void print_usage(const char *exe_name) {
    printf(
        "Usage: %s [-h | --help | <source_file>]\n"
        "\n-h, --help\tShow this help message\n"
        "<source_file>\tFile to execute\n"
        "Note: Only one of the above options should be used at a time.\n",
        exe_name
    );
}

int main(int argc, char* argv[]) {
    // Trata chamada do executável sem argumentos, exceto pelo seu nome 
    if (argc < 2) {
        print_usage(get_basename(argv[0]));
        return 1;
    }

    if (argc > 2) {
        log_error("Error: Too many arguments. Expected only one argument, but got %d.\n", argc - 1);
        print_usage(get_basename(argv[0]));
        return 1;
    }
    
    // Trata a opção "help", que pode serr invocada de forma simplificada ou não
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_usage(get_basename(argv[0]));
        return 0;
    }

    // A partir desse ponto assume que é o nome do arquivo
    char *file_name = argv[1];
    FILE *f = fopen(file_name, "r");
    if (!f) {
        fatal_error("Error: Cannot open file '%s'\n", file_name);
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
            log_error("Error: Unknown command: %s\n", cmd);
        }
    }

    fclose(f);
    return 0;
}