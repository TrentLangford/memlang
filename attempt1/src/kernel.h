#ifndef __KERNEL_H_
#define __KERNEL_H_

#include "common.h"

#include "lang.h"

variable_t tracked_vars[16];
int varc;

uint8_t memory[256];

void memSet(uint8_t adr, uint8_t value);

uint8_t memGet(uint8_t adr);


#endif