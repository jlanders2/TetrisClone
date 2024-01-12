#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"

int main(int argc, char **argv)
{
    // Not great code if I were to need multiple options, but I just need to be
    // able to debug
    if (argc > 1)
    {
        if (strcmp(argv[1], "--speed") == 0)
        {
            a_run(atoi(argv[2]));
        }
    }
    else
    {
        a_run(50);
    }

    return 0;
}
