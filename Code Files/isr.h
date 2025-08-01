// include/isr.h
#ifndef ISR_H
#define ISR_H

#include <stdbool.h>

void I2C_ISR(bool success);
void SPI_ISR(bool success);

#endif