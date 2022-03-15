#include "common.h"
#include "byte.h"
#include "lang.h"

// This program compiles the file into bytecode
// arguments are: input file, output binary    

uint8_t flags = 0;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Incorrect number of arguments\n");
        return 1;
    }

    int infn = 0;
    int outfn = 0;

    char *flagsOrdered[] = {"-f", "-b", "-e", "-c"};

    for (int i = 1; i < argc; i++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (strcmp(argv[i], flagsOrdered[x]) == 0)
            {
                double res = pow(2, x);
                flags += res;
            }
            else if (strcmp(argv[i], "-o") == 0 && outfn == 0)
            {
                outfn = i + 1;
            }
            else if (infn == 0)
            {
                infn = i;
            }
        }
    }


    FILE *input = fopen(argv[infn], "r");
    FILE *output = fopen(argv[outfn], "wb");

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

    fwrite(&flags, sizeof(uint8_t), 1, output);
    int elements_written = fwrite(results.bytes, sizeof(uint8_t), results.bytec, output);
    usleep(SLEEPTIME * results.bytec * (flags > 0 ? flags : 1));
    if (elements_written == results.bytec) printf("Write success\n");
    else printf("Write failure\n");

    fclose(input);
    fclose(output);

    return 0;
}