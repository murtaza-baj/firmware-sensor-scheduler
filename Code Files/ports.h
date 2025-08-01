// include/ports.h
#ifndef PORTS_H
#define PORTS_H

#include "common.h"

// Attached sensor types
enum SensorType {
    SENSOR_NONE,
    SENSOR_MMC5603,
    SENSOR_MMC5983,
    SENSOR_LSM6DSM,
    SENSOR_TYPE_COUNT
};

// Port read state machine states
enum PortStatus {
    STATE_INVALID,
    STATE_IDLE,
    STATE_READING
};

// Logical port indices
enum PortIndex {
    I2C = 0,
    SPI1 = 1,
    UNUSED1,
    SPI2 = 3,
    UNUSED2
};

// PortState Structure
// Holds per-port runtime state
typedef struct {
    enum SensorType type;                // Which sensor is attached
    bool is_connected;                   // Current physical connection status
    bool is_busy;                        // True while a transfer is in progress
    bool was_connected;                  // Previous cycle connection state
    bool prev_read_state;                // True if last transfer succeeded
    uint8_t transfer_state;              // FSM: STATE_IDLE or STATE_READING
    uint8_t byte_offset;                 // Offset in the 16-byte buffer
    uint8_t chip_select;                 // SPI chip-select line index
    uint8_t data_buffer[SENSOR_DATA_SIZE]; // Accumulated sensor data
    uint8_t timeout_counter;             // Disconnection retry counter
}PortState;

// Global array for all ports
extern volatile PortState ports[PORT_COUNT];
extern volatile uint8_t current_port;		  // Keeps track of the current port being used

const char* SensorTypeNames(enum SensorType type);
void hardware_init(void);
void retry_connection(bool *port_increment);
void read_data(bool *port_increment);

#endif