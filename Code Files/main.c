// src/main.c
#include "ports.h"
#include "timer.h"
#include "common.h"

// Callback stubs (provided externally)
void (*device_inserted_removed_callback)(uint8_t port_num, bool device_inserted);
void (*data_read_callback)(uint8_t port_num, uint8_t *data);

// `main()`
int main() {
    hardware_init();   // Set up ports and sensors
    start_timer();     // Begin 100ms tick
    while (1) {
        // Other system work can run here
    }
    return 0;
}
