#ifndef __LANG_H_
#define __LANG_H_

#include "common.h"

#define SLEEPTIME 500000

#define setcontrolbyte(n) control_byte = n; write_control_byte = 1
#define writebyte(n) if (results.bytec == current_size) { current_size += 64; results.bytes = realloc(results.bytes, sizeof(uint8_t) * current_size); } results.bytes[results.bytec++] = n;

typedef struct compileResults_t
{
    uint8_t *bytes;
    int bytec;
    int errCode;
    char *errMessage;
    token errToken;
} compileResults_t;

typedef struct variable_t
{
    char *name;
    uint8_t location;
} variable_t;

typedef struct label_t
{
    char *name;
    uint8_t location;
} label_t;

variable_t variables[256];
int v_count;
label_t labels[256];
int l_count;

int arg_count;

int last_op;

uint8_t next_location;

extern uint8_t flags;

typedef enum arg_t
{
    MEM,
    VAR,
    CONST
} arg_t;

compileResults_t compileTokens(token *tokens, int count);

#endif