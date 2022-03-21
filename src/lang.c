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
    "SHOW",
    "POINT"
};

// Argument count only includes arguments

int expected_args[] = 
{
    2,      // ADD
    2,      // SUB
    2,      // DIV
    2,      // MUL
    2,      // MOD
    2,      // SET
    1,      // LBL
    2,      // GOTO
    1,      // PUT
    1,      // SHOW
    2       // POINT
};



uint8_t handleLabel(token tok, int loc)
{
    for (int i = 0; i < l_count; i++)
        if (strcmp(tok.value, labels[i].name) == 0)
            return labels[i].location;  // Found a variable, return location

    label_t new_label;
    new_label.location = loc;
    new_label.name = malloc(sizeof(char) * strlen(tok.value));
    strcpy(new_label.name, tok.value);
    labels[l_count++] = new_label;
    return new_label.location;  // Return the location of the new variable
}

uint8_t handleVar(token tok)
{
    for (int i = 0; i < v_count; i++)
        if (strcmp(tok.value, variables[i].name) == 0)
            return i;   // Found a label, return location
    
    variable_t new_var;
    new_var.location = next_location++;
    new_var.name = malloc(sizeof(char) * strlen(tok.value));
    strcpy(new_var.name, tok.value);
    variables[v_count++] = new_var;
    return v_count - 1;;    // Return the location of the new label
}

void setError(compileResults_t res, int code, char *mess, token tok)
{
    res.errCode = code,
    res.errMessage = malloc(sizeof(char) * strlen(mess));
    strcpy(res.errMessage, mess);
    res.errToken = tok;
}

compileResults_t compileTokens(token *tokens, int count)
{
    int current_size = 64;
    v_count = 0;
    l_count = 0;
    next_location = 0;
    arg_count = 0;
    compileResults_t results;

    results.bytes = malloc(sizeof(uint8_t) * current_size);
    results.bytec = 0;

    results.errCode = 0;

    arg_t control_byte = 0;

    for (int i = 0; i < count; i++)
    {
        int write_control_byte = 0;
        // Check for errors
        if (arg_count < 0) setError(results, 1, "Incorrect number of arguments", tokens[i]);

        int wrote_operator = 0;

        // Operators
        for (int x = 0; x < 11; x++)
            if (strcmp(tokens[i].value, ordered_tokens[x]) == 0 && arg_count == 0)
            {
                // Push the byte to the byte array and carry on
                writebyte(x + 1);
                arg_count = expected_args[x];
                wrote_operator = 1;
                break;
            }
            else if (strcmp(tokens[i].value, ordered_tokens[x]) == 0 && arg_count != 0) setError(results, 1, "Incorrect number of arguments", tokens[i]);
        if (wrote_operator) continue;

        uint8_t res = 0;
        // Numbers, variables, memory locations
        if (strcmp(tokens[i].type, "_default") == 0)
        {
            // find and set any variables
            if (tokens[i].value[0] == '$')
            {
                res = handleVar(tokens[i]);     // Returns the location of the variable created or found
                setcontrolbyte(VAR);
            }
            else if (tokens[i].value[0] == '#') res = handleLabel(tokens[i], i); // Returns the location of the label, created or found
            else // slap doesn't recognize hex numbers, so they end up being default type
            {
                char *next;
                res = (uint8_t)strtol(tokens[i].value, &next, 16);
                if (next == tokens[i].value) setError(results, 2, "Invalid identifier (not valid hex)", tokens[i]); // I think this works?
                else setcontrolbyte(MEM);
            }
        }
        else if (strcmp(tokens[i].type, "_number") == 0)
        {
            res = (uint8_t)atoi(tokens[i].value);
            setcontrolbyte(CONST);
        }
        // Control byte: 0 if the byte following is a memory location, 1 if it refers to a variable, and 2 if it is a constant
        if (write_control_byte) 
        {
            writebyte((uint8_t)control_byte);
        }
        writebyte(res);

        // Decrement arg count and sleep
        arg_count--;
        usleep(SLEEPTIME * (flags > 0 ? flags : 1));
    }

    return results;
}


