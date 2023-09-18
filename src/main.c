/**
 * @file main.c
 * @author Andrey Stekolnikov (honakac@yandex.ru)
 * @version 2.2.1
 * @date 2023-09-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "options.h"

Options options;

void transferByte(unsigned char byte) {
    if (options.ignoreSymbols != NULL 
        && strchr(options.ignoreSymbols, byte)) putchar(byte);
    else
        if (options.decrypt) putchar(byte - options.deviation);
        else                 putchar(byte + options.deviation);
}

int getSize(FILE *file) {
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    rewind(file);
    return size;
}

int main(int argc, char **argv) {
    options = parseArguments(argc, argv);
    
    if (options.input == stdin) {
        unsigned char byte;
        while ((char) (byte = fgetc(options.input)) != EOF)
            transferByte(byte);
        puts("");
    } else {
        int size = getSize(options.input);
        for (int i = 0; i < size; ++i)
            transferByte(fgetc(options.input));
    }
    fclose(options.input);
    return 0;
}