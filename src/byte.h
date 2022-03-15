#ifndef __BYTE_H_
#define __BYTE_H_

#include "common.h"


typedef enum instructions
{
    ADD = 1,
    SUB,
    DIV,
    MUL,
    MOD,
    SET,
    LBL,
    GOTO,
    PUT,
    SHOW,
    NONE
} instructions;

instructions state;
int args;

typedef struct executeResults
{
    int error;
    char *message;
} executeResults;

int cpointer;
uint8_t *command;
int cready;

executeResults executeByte(uint8_t byte);
void executeCommand(uint8_t *command, int n);



#endif