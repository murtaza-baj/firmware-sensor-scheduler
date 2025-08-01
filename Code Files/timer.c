// src/timer.c
#include "ports.h"
#include "timer.h"

void TIMER_ISR() {
    bool port_inc = false;
    retry_connection(&port_inc);
    read_data(&port_inc);

    if (port_inc) {
        do {
            current_port = (current_port + 1) % PORT_COUNT;
        } while (ports[current_port].type == SENSOR_NONE);
    }
}

void start_timer() {
    // Stub
}