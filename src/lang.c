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

// Argument count includes the initial token and the additional arguments

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

compileResults_t compileTokens(token *tokens, int count)
{
    v_count = 0;
    l_count = 0;
    next_location = 0;
    arg_count = 0;
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
                if (arg_count == 0)
                {
                    arg_count = expected_args[n];
                    last_op = i;
                }
                else
                {
                    results.errCode = 2;
                    results.errMessage = malloc(sizeof(char) * strlen("Incorrect number of arguments"));
                    strcpy(results.errMessage, "Incorrect number of arguments");
                    results.errToken = tokens[last_op];
                    break;
                }
            }
        }

        if (results.errCode != 0) break;

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
                        results.bytes[results.bytec] = variables[x].location;
                        results.bytec++;
                        is_def = 1;
                        break;
                    }
                }

                if (!is_def)
                {
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
                        results.bytes[results.bytec] = labels[x].location;
                        results.bytec++;
                        is_def = 1;
                        break;
                    }
                }

                if (!is_def)
                {
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
        printf("Byte %d token %s(%s)\n", results.bytes[results.bytec-1], tokens[i].type, tokens[i].value);
        arg_count--;
        usleep(SLEEPTIME * (flags > 0 ? flags : 1));
    }

    return results;
}


