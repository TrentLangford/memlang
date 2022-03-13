#include "common.h"
#include "byte.h"
#include "lang.h"

// This program compiles the file into bytecode
// arguments are: input file, output binary
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Incorrect number of arguments\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "wb");

    FILE *grammar = fopen("rules.bin", "rb");

    if (!input)
    {
        printf("Cannot open file %s\n", argv[1]);
        return 1;
    }
    if (!output)
    {
        printf("Cannot open file %s\n", argv[2]);
        return 1;
    }
    if (!grammar)
    {
        printf("Cannot open file rules.bin\n");
        return 1;
    }

    gram_rule *rules = readRulesFromFile(grammar);
    gram_rule g = rules[0];
    int i = 0;
    int count = 0;
    while (strcmp(g.type_name, "end"))
    {
        count++;
        g = rules[++i];
    }
    
    
    int tcount = 0;
    int tsize = 1024;
    token *tokens = malloc(sizeof(token) * tsize);
    char buff[64];
    while (fscanf(input, "%64s", buff) == 1)
    {
        token current = wordToToken(buff, rules, count);
        if (tcount < tsize) tokens[tcount++] = current;
        else
        {
            token *tokens = realloc(tokens, tsize * 2);
            tsize *= 2;
        }
    }

    compileResults_t results = compileTokens(tokens, tcount);

    if (results.errCode == 0)
    {
        printf("Compilation success\n");
    }
    else
    {
        printf("Compile error %d: %s at %s\n", results.errCode, results.errMessage, results.errToken.value);
        return results.errCode;
    }

    fclose(input);
    fclose(output);

    return 0;
}