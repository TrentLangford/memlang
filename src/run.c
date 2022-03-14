#include "common.h"

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

    uint8_t *bytes = malloc(sizeof(uint8_t) * length);

    fread(bytes, sizeof(uint8_t), length, input);

    for (int i = 0; i < length; i++)
    {
        printf("Byte: %d\n", bytes[i]);
    }
    


    fclose(input);

    return 0;
}