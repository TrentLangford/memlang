#include "lang.h"

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

int *expectedArguments[] = 
{
    {2},        // ADD
    {2},        // SUB
    {2},        // DIV
    {2},        // MUL
    {2},        // MOD
    {2},        // SET
    {1},        // LBL
    {2, 3},     // GOTO
    {1},        // PUT
    {1}         // SHOW
};


compileResults_t compileTokens(token *tokens, int count)
{
    v_count = 0;
    l_count = 0;
    next_location = 0;
    compileResults_t results;

    results.bytes = malloc(sizeof(u_int8_t) * count);
    results.bytec = 0;

    results.errCode = 0;

    for (int i = 0; i < count; i++)
    {
        for (int n = 0; n < 10; n++)
        {
            if (strcmp(tokens[i].value, ordered_tokens[n]) == 0)
            {
                results.bytes[results.bytec] = (uint8_t)(n + 1);
                results.bytec++;
            }
        }

        if (strcmp(tokens[i].type, "_number") == 0)
        {
            int res = atoi(tokens[i].value);
            results.bytes[results.bytec] = (uint8_t)(res);
            results.bytec++;
        }

        if (strcmp(tokens[i].type, "_default") == 0)
        {
            if (tokens[i].value[0] == '$')
            {
                int is_def = 0;
                for (int x = 0; x < v_count; x++)
                {
                    if (strcmp(variables[x].name, tokens[i].value) == 0)
                    {
                        printf("referenced variable %s\n", variables[x].name);
                        results.bytes[results.bytec] = variables[x].location;
                        results.bytec++;
                        is_def = 1;
                        break;
                    }
                }

                if (!is_def)
                {
                    printf("creating variable\n");
                    variable_t var;
                    var.name = malloc(sizeof(char) * strlen(tokens[i].value));
                    strcpy(var.name, tokens[i].value);
                    var.location = next_location++;
                    variables[v_count] = var;
                    v_count++;

                    results.bytes[results.bytec] = var.location;
                    results.bytec++;
                }
            }
            else if (tokens[i].value[0] == '#')
            {
                int is_def = 0;
                for (int x = 0; x < l_count; x++)
                {
                    if (strcmp(labels[x].name, tokens[i].value) == 0)
                    {
                        printf("referenced label %s\n", labels[x].name);
                        results.bytes[results.bytec] = labels[x].location;
                        results.bytec++;
                        is_def = 1;
                        break;
                    }
                }

                if (!is_def)
                {
                    printf("creating label\n");
                    label_t lb;
                    lb.name = malloc(sizeof(char) * strlen(tokens[i].value));
                    strcpy(lb.name, tokens[i].value);
                    lb.location = i;
                    labels[l_count] = lb;
                    l_count++;

                    results.bytes[results.bytec] = lb.location;
                    results.bytec++;
                }
            }
            else
            {
                results.errCode = 1;
                results.errMessage = malloc(sizeof(char) * strlen("Undefined identifier"));
                strcpy(results.errMessage, "Undefined identifier");
                results.errToken = tokens[i];
                break;
            }
        }
    }

    return results;
}


