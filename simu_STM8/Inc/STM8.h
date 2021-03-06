#ifndef STM8_H_INCLUDED
#define STM8_H_INCLUDED
#endif
#include <stdio.h>
#include "libserialport.h"

int STM8_connect(const char* serialPortName, int baudrate);
int STM8_read(void* buffer, size_t bufferSize);
int STM8_write(const void* buffer, size_t bufferSize);
void STM8_disconnect();
