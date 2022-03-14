#ifndef __LANG_H_
#define __LANG_H_

#include "common.h"

#define SLEEPTIME 500000

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

compileResults_t compileTokens(token *tokens, int count);

#endif