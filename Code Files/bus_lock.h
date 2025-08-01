// include/bus_lock.h
#ifndef BUS_LOCK_H
#define BUS_LOCK_H

#include "common.h"

// Binary semaphore states
typedef enum {
    SEMAPHORE_FREE = 0,
    SEMAPHORE_TAKEN = 1
} semaphore_state_t;

extern volatile semaphore_state_t i2c_semaphore;
extern volatile semaphore_state_t spi_semaphore;

bool acquire_semaphore(volatile semaphore_state_t *sem);
void release_semaphore(volatile semaphore_state_t *sem);

#endif