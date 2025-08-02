// src/bus_lock.c
#include "common.h"     // for LOG_INFO, SENSOR_DATA_SIZE, etc.
#include "ports.h"      // for PortState, ports[], current_port
#include "bus_lock.h"   // for acquire/release_semaphore
#include "sensors.h"    // for start_*_data_read()
#include "isr.h"        // for I2C_ISR/SPI_ISR prototypes
#include "callbacks.h"  // for callback functions

// I2C and SPI semaphores (volatile for ISR safety)
volatile semaphore_state_t i2c_semaphore = SEMAPHORE_FREE;
volatile semaphore_state_t spi_semaphore = SEMAPHORE_FREE;

// Attempt to take the semaphore; returns true on success
bool acquire_semaphore(volatile semaphore_state_t *sem) {
    if (*sem == SEMAPHORE_FREE) {
        *sem = SEMAPHORE_TAKEN;
        return true;
    }
    return false;
}

// Release the semaphore
void release_semaphore(volatile semaphore_state_t *sem) {
    *sem = SEMAPHORE_FREE;
}
