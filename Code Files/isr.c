// src/isr.c
#include "isr.h"
#include "ports.h"
#include "common.h"     
#include "bus_lock.h"

static void read_success(void);
static void read_failure(void);
static void handle_read_complete(bool success);

void I2C_ISR(bool success) {
    release_semaphore(&i2c_semaphore);
    handle_read_complete(success);
}

void SPI_ISR(bool success) {
    release_semaphore(&spi_semaphore);
    handle_read_complete(success);
}

static void read_success(void) {
    struct PortState *p = &ports[current_port];
    if (!p->prev_read_state) {
        device_inserted_removed_callback(current_port, true);
        LOG_INFO("Reconnected %s on port %d\n", SensorTypeNames(p->type), current_port);
    }
    p->timeout_counter = 0;
    if (p->type == SENSOR_MMC5983) {
        p->byte_offset = (p->byte_offset + 2) % 8;                  //Sensor only gives 8 bytes worth of valid data
    }
    else {
        p->byte_offset = (p->byte_offset + 2) % SENSOR_DATA_SIZE;
    }
    
    // If full 16 bytes collected, deliver and reset
    if (p->byte_offset == 0) {
        if (p->type == SENSOR_MMC5983) {
          memset(&p->data_buffer[8], 0, 8);
        }
    data_read_callback(current_port, p->data_buffer);
    memset(p->data_buffer, 0, SENSOR_DATA_SIZE);
    p->transfer_state = STATE_IDLE;
    }
}

static void read_failure(void) {
    struct PortState *p = &ports[current_port];
    device_inserted_removed_callback(current_port, false);
    LOG_ERROR("Disconnect %s on port %d\n", SensorTypeNames(p->type), current_port);
    p->timeout_counter++;
    p->is_connected = false;
    p->was_connected = true;
    p->transfer_state = STATE_IDLE;
    p->byte_offset = 0;
    memset(p->data_buffer, 0, SENSOR_DATA_SIZE);
}
// Common post-read FSM: advance offset or signal disconnect
static void handle_read_complete(bool success) {
    struct PortState *p = &ports[current_port];
    p->is_busy = false;

    if (success) {
        read_success();
    } else {
        read_failure();
    }
    p->prev_read_state = success;
}