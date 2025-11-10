#include <stdio.h>
#include <stdlib.h>
#include "RSA-functions.h"

void Usage(char *argv[])
{
    printf("Usage: %s <Prime number> <Prime number>", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        Usage(argv[0]);
    }

    return 0;
}