// src/ports.c
#include "ports.h"

volatile PortState ports[PORT_COUNT];
volatile uint8_t current_port = 0;

// For logging: readable sensor names
const char* SensorTypeNames(enum SensorType type) {
    switch (type) {
        case SENSOR_NONE:    return "SENSOR_NONE";
        case SENSOR_MMC5603: return "SENSOR_MMC5603";
        case SENSOR_MMC5983: return "SENSOR_MMC5983";
        case SENSOR_LSM6DSM: return "SENSOR_LSM6DSM";
        default:             return "UNKNOWN_SENSOR";
    }
}

// Retry disconnected sensors after TIMEOUT_COUNT cycles
void retry_connection(bool *port_increment) {
    if (!ports[current_port].is_connected && ports[current_port].was_connected) {
        if (++ports[current_port].timeout_counter >= TIMEOUT_COUNT) {
            LOG_INFO("Retrying %s on port %d\n", SensorTypeNames(ports[current_port].type), current_port);
            ports[current_port].is_connected   = true;
            ports[current_port].was_connected  = false;
            ports[current_port].timeout_counter = 0;
            ports[current_port].transfer_state = STATE_IDLE;
            ports[current_port].byte_offset    = 0;
            memset(ports[current_port].data_buffer, 0, SENSOR_DATA_SIZE);
        } else {
            *port_increment = true;
        }
    }
}

// Set all ports to default disconnected state, then configure known sensors
void hardware_init(void) {
    for (int i = 0; i < PORT_COUNT; i++) {
        ports[i].type = SENSOR_NONE;
        ports[i].chip_select = 0xFF;
        ports[i].is_connected = false;
        ports[i].is_busy = false;
        ports[i].was_connected = false;
        ports[i].prev_read_state = false;
        ports[i].transfer_state = STATE_INVALID;
        ports[i].timeout_counter = 0;
        ports[i].byte_offset = 0;
        memset(ports[i].data_buffer, 0, sizeof(ports[i].data_buffer));
    }

    // Pre-populate with sensors present at startup
    ports[I2C].type           = SENSOR_MMC5603;
    ports[I2C].is_connected   = true;
    ports[I2C].prev_read_state= true;
    ports[I2C].transfer_state = STATE_IDLE;
    device_inserted_removed_callback(I2C, true);

    ports[SPI1].type           = SENSOR_MMC5983;
    ports[SPI1].chip_select    = 0;
    ports[SPI1].is_connected   = true;
    ports[SPI1].prev_read_state= true;
    ports[SPI1].transfer_state = STATE_IDLE;
    device_inserted_removed_callback(SPI1, true);

    ports[SPI2].type           = SENSOR_LSM6DSM;
    ports[SPI2].chip_select    = 2;
    ports[SPI2].is_connected   = true;
    ports[SPI2].prev_read_state= true;
    ports[SPI2].transfer_state = STATE_IDLE;
    device_inserted_removed_callback(SPI2, true);
}

// Initiate one 2-byte read per 100ms tick, per port
void read_data(bool *port_increment) {
    if (*port_increment) return;

    struct PortState *p = &ports[current_port];
    if (p->is_busy || !p->is_connected) return;

    switch (p->type) {
      case SENSOR_MMC5603:
        if (acquire_semaphore(&i2c_semaphore)) {
            p->is_busy        = true;
            p->transfer_state = STATE_READING;
            start_MMC5603_data_read(&p->data_buffer[p->byte_offset]);
        }
        break;

      case SENSOR_MMC5983:
        if (acquire_semaphore(&spi_semaphore)) {
            p->is_busy        = true;
            p->transfer_state = STATE_READING;
            start_MMC5983_data_read(p->chip_select, &p->data_buffer[p->byte_offset]);
        }
        break;

      case SENSOR_LSM6DSM:
        if (acquire_semaphore(&spi_semaphore)) {
            p->is_busy        = true;
            p->transfer_state = STATE_READING;
            start_LSM6DSM_data_read(p->chip_select, &p->data_buffer[p->byte_offset]);
        }
        break;

      default:
        LOG_ERROR("Unknown sensor on port %d\n", current_port);
        p->is_connected = false;
        break;
    }

    if (p->transfer_state != STATE_READING) {
        *port_increment = true;
    }
}