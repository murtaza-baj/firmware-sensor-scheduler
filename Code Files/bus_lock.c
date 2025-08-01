// src/bus_lock.c
#include "bus_lock.h"

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