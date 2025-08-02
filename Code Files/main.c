// src/main.c
#include "common.h"     // for LOG_INFO, SENSOR_DATA_SIZE, etc.
#include "ports.h"      // for PortState, ports[], current_port
#include "bus_lock.h"   // for acquire/release_semaphore
#include "sensors.h"    // for start_*_data_read()
#include "isr.h"        // for I2C_ISR/SPI_ISR prototypes
#include "callbacks.h"  // for callback functions

// `main()`
int main() {
    hardware_init();   // Set up ports and sensors
    start_timer();     // Begin 100ms tick
    while (1) {
        // Other system work can run here
    }
    return 0;
}
