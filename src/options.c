/**
 * @file options.c
 * @author Andrey Stekolnikov (honakac@yandex.ru)
 * @version 2.2.1
 * @date 2023-09-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "options.h"
#include "def.h"
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

static struct option flags[] = {
    {"help", 0, 0, 'h'},
    {"version", 0, 0, 'v'},
    {"deviation", 1, 0, 'a'},
    {"decrypt", 0, 0, 'd'},
    {"ignore-chars", 1, 0, 'i'},
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

Options parseArguments(int argc, char **argv) {
    Options options;
    options.decrypt =       0;
    options.deviation =     2;
    options.ignoreSymbols = NULL;
    options.input =         stdin;

    /**
     * @brief Getting the input file
     * 
     */
    for (int pos = 1; pos < argc; pos++) {
        if (argv[pos][0] != '-' && options.input == stdin)
            options.input = openfile(argv[pos], "r");
        for (size_t index = 0; index < sizeof(flags) / sizeof(struct option); index++) {
            if (flags[index].has_arg 
                && (!strcmp(argv[pos] + 2, flags[index].name) || argv[pos][1] == flags[index].val)
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
            case 'a': 
                options.deviation = atoi(optarg); 
                break;
            case 'i':
                options.ignoreSymbols = malloc(strlen(optarg));
                bzero(options.ignoreSymbols, strlen(optarg));
                strcpy(options.ignoreSymbols, optarg);
                break;
            case 'v':
                puts("asciicaesar " AC_VERSION);
                exit(1);
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
                exit(1);
        }
    }
    return options;
}