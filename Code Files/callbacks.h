// callbacks.h
#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <stdint.h>
#include <stdbool.h>

// These function‐pointers must be assigned by the top‐level firmware
extern void (*device_inserted_removed_callback)(uint8_t port_num, bool device_inserted);
extern void (*data_read_callback)(uint8_t port_num, uint8_t *data);

#endif // CALLBACKS_H
