#include "binds.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libevdev/libevdev.h>

#define SPACE ' '
#define TAB '	'
#define NEWLINE '\n'

static int is_spaceish(char input) {
    return (input == SPACE || input == TAB || input == NEWLINE);
}

static void strip_space(char* input) {
    char* read_ptr = input;
    char* write_ptr = input;

    while (*read_ptr != '\0') {
        if (!is_spaceish(*read_ptr)) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

int* load_binds(char* path) {
    // there are 12 keys on the keypad
    int* binds = calloc(12, sizeof(int));
    

    FILE* conf_file = fopen(path, "r");
    
    char str[50] = {0};
    while (fgets(str, sizeof(str), conf_file)) {
        strip_space(str);
        char* token1 = strtok(str, "=");
        char* token2 = strtok(NULL, "=");
        // printf("%d\n", convert_to_code(token2));
        binds[atoi(token1) - 1] = libevdev_event_code_from_code_name(token2);
    }

    fclose(conf_file);

    return binds;
}