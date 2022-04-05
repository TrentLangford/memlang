#ifndef __BYTE_H_
#define __BYTE_H_

#include "common.h"

#include "kernel.h"

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

int byteNextLocation;

executeResults executeBytes(uint8_t *bytes, int n);
void executeCommand(uint8_t *command, int n);



#endif