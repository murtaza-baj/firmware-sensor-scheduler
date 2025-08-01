// include/sensors.h
#ifndef SENSORS_H
#define SENSORS_H

#include "common.h"

void start_MMC5603_data_read(uint8_t *buffer);
void start_MMC5983_data_read(uint8_t chip_select, uint8_t *buffer);
void start_LSM6DSM_data_read(uint8_t chip_select, uint8_t *buffer);

#endif