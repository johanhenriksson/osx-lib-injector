#include "common.h"
#include <stdlib.h>
#include <stdio.h>

void panic(char* message) {
    printf("%s\n", message);
    exit(1);
}

