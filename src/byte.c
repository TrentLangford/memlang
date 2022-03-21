#include "byte.h"

int expected_args[] = 
{
    4,      // ADD, control data control data
    4,      // SUB, "       "    "       "
    4,      // DIV, "
    4,      // MUL
    4,      // MOD
    4,      // SET
    1,      // LBL, destination of instruction to jump to
    3,      // GOTO, label control data
    2,      // PUT, control data
    2,      // SHOW, "     "
    4       // POINT control data control data
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
    "SHOW",
    "POINT"
};

// Returns the location of a variable if it exists and creates a new variable if not
uint8_t handleVars(uint8_t location)
{
    if (location >= varc)
    {
        variable_t newvar;
        newvar.location = byteNextLocation++;
        newvar.name = NULL; // We don't care about names anyway
        tracked_vars[varc++] = newvar;
        return newvar.location;
    }
    else return tracked_vars[location].location;
}

void addop(int rcontrol, uint8_t rloc, int addcontrol, uint8_t adata)
{
    int memLocation = 0;
    switch (rcontrol)
    {
    case 0:
    case 2:
        memLocation = rloc;
        break;
    case 1:
        memLocation = handleVars(rloc);
        break;
    }

    switch (addcontrol)
    {
    case 0:
        memSet(memLocation, memGet(memLocation) + memGet(adata));
        break;
    case 1:
        memSet(memLocation, memGet(memLocation) + memGet(handleVars(adata)));
        break;
    case 2:
        memSet(memLocation, memGet(memLocation) + adata);
        break;
    }
}

void subop(int rcontrol, uint8_t rloc, int addcontrol, uint8_t adata)
{
    int memLocation = 0;
    switch (rcontrol)
    {
    case 0:
    case 2:
        memLocation = rloc;
        break;
    case 1:
        memLocation = handleVars(rloc);
        break;
    }

    switch (addcontrol)
    {
    case 0:
        memSet(memLocation, memGet(memLocation) - memGet(adata));
        break;
    case 1:
        memSet(memLocation, memGet(memLocation) - memGet(handleVars(adata)));
        break;
    case 2:
        memSet(memLocation, memGet(memLocation) - adata);
        break;
    }
}

void divop(int rcontrol, uint8_t rloc, int addcontrol, uint8_t adata)
{
    int memLocation = 0;
    switch (rcontrol)
    {
    case 0:
    case 2:
        memLocation = rloc;
        break;
    case 1:
        memLocation = handleVars(rloc);
        break;
    }

    switch (addcontrol)
    {
    case 0:
        memSet(memLocation, (uint8_t)(memGet(memLocation) / memGet(adata)));
        break;
    case 1:
        memSet(memLocation, (uint8_t)(memGet(memLocation) / memGet(handleVars(adata))));
        break;
    case 2:
        memSet(memLocation, (uint8_t)(memGet(memLocation) / adata));
        break;
    }
}

void mulop(int rcontrol, uint8_t rloc, int addcontrol, uint8_t adata)
{
    int memLocation = 0;
    switch (rcontrol)
    {
    case 0:
    case 2:
        memLocation = rloc;
        break;
    case 1:
        memLocation = handleVars(rloc);
        break;
    }

    switch (addcontrol)
    {
    case 0:
        memSet(memLocation, memGet(memLocation) * memGet(adata));
        break;
    case 1:
        memSet(memLocation, memGet(memLocation) * memGet(handleVars(adata)));
        break;
    case 2:
        memSet(memLocation, memGet(memLocation) * adata);
        break;
    }
}

void modop(int rcontrol, uint8_t rloc, int addcontrol, uint8_t adata)
{
    int memLocation = 0;
    switch (rcontrol)
    {
    case 0:
    case 2:
        memLocation = rloc;
        break;
    case 1:
        memLocation = handleVars(rloc);
        break;
    }

    switch (addcontrol)
    {
    case 0:
        memSet(memLocation, memGet(memLocation) % memGet(adata));
        break;
    case 1:
        memSet(memLocation, memGet(memLocation) % memGet(handleVars(adata)));
        break;
    case 2:
        memSet(memLocation, memGet(memLocation) % adata);
        break;
    }
}

void setop(uint8_t fcontrol, uint8_t fdata, uint8_t scontrol, uint8_t sdata)
{
    int memLocation = 0;
    switch (fcontrol)
    {
    case 0:
    case 2:
        memLocation = fdata;
        break;
    case 1:
        memLocation = handleVars(fdata);
        break;
    }

    switch (scontrol)
    {
    case 0:
        memSet(memLocation, memGet(sdata));
        break;
    case 1:
        memSet(memLocation, memGet(handleVars(sdata)));
        break;
    case 2:
        memSet(memLocation, sdata);
    }
}

executeResults executeBytes(uint8_t *bytes, int n)
{
    executeResults res = { .error = 0, .message = NULL };

    state = bytes[0];

    int i = 0;
    while (i < n)
    {
        int operator = bytes[i];
        uint8_t *command = malloc(sizeof(uint8_t) * (expected_args[operator - 1] + 1));
        
        for (int x = 0; x < expected_args[operator - 1] + 1; x++)
            command[x] = bytes[i+x];

        executeCommand(command, expected_args[operator - 1] + 1);

        i += expected_args[operator - 1] + 1;
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

    switch (command[0])
    {
    case ADD:
        addop(command[1], command[2], command[3], command[4]);
        break;
    case SUB:
        subop(command[1], command[2], command[3], command[4]);
        break;
    case DIV:
        divop(command[1], command[2], command[3], command[4]);
        break;
    case MUL:
        mulop(command[1], command[2], command[3], command[4]);
        break;
    case MOD:
        modop(command[1], command[2], command[3], command[4]);
        break;
    case SET:
        setop(command[1], command[2], command[3], command[4]);
        break;
    case LBL:
        break;
    case GOTO:
        break;
    case PUT:
        break;
    case SHOW:
        break;
    default:
        break;
    }
}