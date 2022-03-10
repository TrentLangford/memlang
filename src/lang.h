#ifndef __LANG_H_
#define __LANG_H_

#include "common.h"

uint8_t heap[2048];
uint8_t stack[256];
uint8_t stackptr;

uint8_t heapGet(int adr);
uint8_t stackGet();

void heapSet(int adr);
void stackSet();

void stackPush(uint8_t val);
uint8_t stackPop();

#endif