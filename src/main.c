#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#define AC_VERSION "2.2"

static struct option flags[] = {
    {"help", 0, 0, 'h'},
    {"version", 0, 0, 'v'},
    {"deviation", 1, 0, 'a'},
    {"decrypt", 0, 0, 'd'},
    {"ignore-chars", 1, 0, 'i'},
    {"newline", 0, 0, 'n'},
    {0, 0, 0, 0}
};

FILE *openfile(const char *path, const char *mode) {
    FILE *file = fopen(path, mode);
    if (!file) {
        perror("fopen()");
        exit(EXIT_FAILURE);
    }
    return file;
}

int main(int argc, char **argv) {
    struct {
        char decrypt;
        char deviation;
        char newline;
        char *ignoreSymbols;
    } options;
    options.decrypt =       0;
    options.newline =       0;
    options.deviation =     2;
    options.ignoreSymbols = NULL;

    FILE *input = stdin;
    for (int pos = 1; pos < argc; pos++) {
        if (argv[pos][0] != '-' && input == stdin)
            input = openfile(argv[pos], "r");
        for (size_t index = 0; index < sizeof(flags)/sizeof(struct option); index++) {
            if (flags[index].has_arg 
                && (!strcmp(argv[pos]+2, flags[index].name) || argv[pos][1] == flags[index].val)
                    && !strchr(argv[pos], '=')) {
                pos++;
                break;
            }
        }
    }

    int opt =      0;
    int optIndex = 0;
    while ((opt = getopt_long(argc, argv, "da:i:nhv", flags, &optIndex)) != -1) {
        switch (opt) {
            case 'd': 
                options.decrypt   = 1; 
                break;
            case 'n': 
                options.newline   = 1; 
                break;
            case 'a': 
                options.deviation = atoi(optarg); 
                break;
            case 'i':
                options.ignoreSymbols = malloc(strlen(optarg));
                bzero(options.ignoreSymbols, strlen(optarg));
                strcpy(options.ignoreSymbols, optarg);
                break;
            case 'v':
                puts("asciicaesar " AC_VERSION "\n");
                return 1;
            case 'h':
            default:
                fprintf(stderr, "usage: %s [OPTION]... [FILE]\n" \
                    "Caesar cipher encode or decode FILE, or standard input, to standard output.\n" \
                    "With no FILE, or when FILE is -, read standard input.\n" \
                    "Options:\n" \
                    " -d, --decrypt              Decrypt data.\n" \
                    " -a, --deviation=NUMBER     Amounts of deviation from the original symbol(default 2).\n" \
                    " -i, --ignore-chars=CHARS   Ignore chars\n" \
                    " -n, --newline              Don't ignore newline.\n" \
                    " -h, --help                 Show this message and exit.\n" \
                    " -v, --version              Output version information and exit.\n", argv[0]);
                return 1;
        }
    }
    char ch;
    while ((ch = fgetc(input)) != EOF) {
        if (!options.newline && ch == '\n')              putchar('\n');
        else if (options.ignoreSymbols != NULL 
                && strchr(options.ignoreSymbols, ch)) putchar(ch);
        else {
            if (options.decrypt) putchar(ch-options.deviation);
            else                 putchar(ch+options.deviation);
        }
    }
    if (input != stdin)
        puts("");
    fclose(input);
    return 0;
}