#pragma once
#include <stdio.h>

typedef struct {
    char decrypt;
    char deviation;
    char *ignoreSymbols;
    FILE *input;
} Options;

/**
 * @brief Parsing Arguments and Retrieving Values
 * 
 * @param argc argc
 * @param argv argv
 * @return Options options
 */
Options parseArguments(int argc, char **argv);