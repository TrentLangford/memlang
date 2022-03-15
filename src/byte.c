#include "byte.h"

int expected_args[] = 
{
    3,      // ADD
    3,      // SUB
    3,      // DIV
    3,      // MUL
    3,      // MOD
    3,      // SET
    2,      // LBL
    3,      // GOTO
    2,      // PUT
    2,      // SHOW
};

char *ordered_tokens[] = 
{
    "ADD",
    "SUB",
    "DIV",
    "MUL",
    "MOD",
    "SET",
    "LBL",
    "GOTO",
    "PUT",
    "SHOW"
};

executeResults executeByte(uint8_t byte)
{
    executeResults res = { .error = 0, .message = NULL };

    state = byte;

    if (args == 0)
    {
        args = expected_args[state - 1] - 1;
        cpointer = 0;
        command = malloc(sizeof(uint8_t) * args);

        command[cpointer++] = byte;
    }
    else if (cpointer == args)
    {
        command[cpointer++] = byte;
        executeCommand(command, cpointer);
        free(command);

        args = 0;
    }
    else
    {
        command[cpointer++] = byte;
    }

    return res;
}

void executeCommand(uint8_t *command, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (i == 0) printf("Command has byte %s (%d)\n", ordered_tokens[command[i] - 1], command[i]);
        else printf("Command has byte (%d)\n", command[i]);
    }
}