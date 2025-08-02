// include/common.h
#ifndef COMMON_H
#define COMMON_H

// Includes and Logging
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define NUM_CHIP_SELECT_LINES 4
#define PORT_COUNT 5                // Total number of sensor ports
#define TIMEOUT_COUNT 2             // Number of failed attempts before marking a sensor as disconnected
#define ENABLE_LOGGING              // Enable debug logging

#define MMC5603_ADDR 0x30
#define MMC5603_OUT0_REG 0x00
#define MMC5983_OUT_BASE 0x02
#define LSM6DSM_OUT_XL   0x28

#define SENSOR_DATA_SIZE 16         // Total bytes to read per sensor
#define MMC5983_DATA_SIZE 8         // Sensor only reads 8 bytes total

//Logging Macros
#ifdef ENABLE_LOGGING
    #define LOG_PRINTF(...) printf(__VA_ARGS__)
#else
    #define LOG_PRINTF(...) do {} while (0)
#endif

#define LOG_ERROR(...) LOG_PRINTF("ERROR: " __VA_ARGS__)
#define LOG_INFO(...)  LOG_PRINTF("INFO: " __VA_ARGS__)

#endif
