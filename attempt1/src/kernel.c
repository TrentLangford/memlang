#include "kernel.h"


void memSet(uint8_t adr, uint8_t value)
{
    printf("Set %d to %d\n", adr, value);
    memory[adr] = value;
}

uint8_t memGet(uint8_t adr)
{
    printf("Got %d from %d\n", memory[adr], adr);
    return memory[adr];
}