#include "common.h"
#include "byte.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Incorrect number of arguments\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "rb");

    if (input == NULL)
    {
        printf("Could not open file %s\n", argv[1]);
        return 1;
    }

    fseek(input, 0L, SEEK_END);
    int length = ftell(input);

    printf("length %d\n", length);

    rewind(input);

    uint8_t flags = 0;
    fread(&flags, sizeof(uint8_t), 1, input);

    int fFire = flags & 1;
    int fBugs = (flags & 2) / 2;
    int fStat = (flags & 4) / 4;
    int fCosm = (flags & 8) / 8;

    printf("fire flag: %d, bug flag: %d, static electricity flag: %d, cosmic bit flip flag: %d\n", fFire, fBugs, fStat, fCosm);


    uint8_t *bytes = malloc(sizeof(uint8_t) * length - 1);

    fread(bytes, sizeof(uint8_t), length - 1, input);

    time_t t;
    srand((unsigned) time(&t));
    cready = 0;

    executeBytes(bytes, length - 1);
    


    fclose(input);

    return 0;
}