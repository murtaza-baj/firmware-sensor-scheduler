// src/sensors.c
#include "common.h"     // for LOG_INFO, SENSOR_DATA_SIZE, etc.
#include "ports.h"      // for PortState, ports[], current_port
#include "bus_lock.h"   // for acquire/release_semaphore
#include "sensors.h"    // for start_*_data_read()
#include "isr.h"        // for I2C_ISR/SPI_ISR prototypes
#include "callbacks.h"  // for callback functions

// I2C read for MMC5603: two-byte chunks
void start_MMC5603_data_read(uint8_t *buffer) {
    uint8_t reg = MMC5603_OUT0_REG + ports[current_port].byte_offset;
    start_i2c_transfer(MMC5603_ADDR, &reg, 1, buffer, 2);
}

// SPI read for MMC5983: select from reg_map (8 valid pairs)
void start_MMC5983_data_read(uint8_t chip_select, uint8_t *buffer) {
    static const uint8_t reg_map[8] = {0x02,0x03, 0x04,0x05, 0x06,0x07, 0x08, 0x09};
    uint8_t index = ports[current_port].byte_offset / 2;
    uint8_t cmd   = 0x80 | reg_map[index];
    uint8_t tx[2] = {cmd, 0x00};
    // Drop first received byte by offsetting buffer pointer back one
    start_spi_transfer(chip_select, tx, buffer + ports[current_port].byte_offset, 2);
}

// SPI read for LSM6DSM: auto-increment + contiguous registers
void start_LSM6DSM_data_read(uint8_t chip_select, uint8_t *buffer) {
    uint8_t reg = LSM6DSM_OUT_XL + ports[current_port].byte_offset;
    uint8_t cmd = 0x80 /*read*/ | 0x40 /*auto-inc*/ | reg;
    uint8_t tx[2] = {cmd, 0x00};
    start_spi_transfer(chip_select, tx, buffer + ports[current_port].byte_offset, 2);
}
