# Firmware Sensor Scheduler (ISR-Driven)

This project implements an interrupt-driven firmware system that simulates cooperative task scheduling for reading data from multiple environmental and motion sensors (MMC5603, MMC5983, and LSM6DSM) over shared I2C and SPI buses.

Designed as part of a senior firmware engineering challenge, this implementation balances real-time constraints, ISR-safe communication, and task fairness through semaphores, port-level state machines, and retry-based reconnection logic.

---

## ⚙️ Project Features

- **Bus Arbitration**: Manages shared I2C and SPI lines with non-blocking reads using semaphores.
- **ISR Coordination**: Sensor reads are broken into 2-byte transfers, triggered every 100ms via a timer ISR.
- **Sensor Management**: Handles dynamic connection/disconnection and reconnection retries per port.
- **Port Scheduling**: Implements round-robin logic for scanning ports, skipping uninitialized or inactive ports.
- **Fault Tolerance**: Supports timeout-based retries and device removal detection.
- **Data Readout**: Streams 16-byte reads per sensor in non-blocking fashion with user callback hooks.

---

## 🧪 Sensors Supported

| Sensor      | Bus  | Protocol | Notes                             |
|-------------|------|----------|-----------------------------------|
| MMC5603     | I2C  | 7-bit    | Environmental magnetic sensor     |
| MMC5983     | SPI  | SPI Mode | Reads registers 0x02 through 0x09 |
| LSM6DSM     | SPI  | SPI Mode | IMU sensor starting at 0x28       |

Each read cycle extracts 2 bytes until 16 bytes are complete.
Only first 8 bytes are valid for MMC5983. 9-16 are padded with zeros to maintain 16 byte structure for callback function.
SPI sensors read only 1 byte from the data per cycle due to 2 byte/cycle transfer limitation.

---

## 🧵 Architecture Overview

```txt
+----------------------+
|     TIMER_ISR()      |
|----------------------|
|  - Retry reconnection
|  - Initiate sensor read
|  - Rotate to next port
+----------------------+

+----------------------+        +--------------------+
|  I2C_ISR(success)    | <----- | start_i2c_transfer |
|  SPI_ISR(success)    | <----- | start_spi_transfer |
+----------------------+        +--------------------+

+-----------------------+
|  read_success/failure |
|  - Track byte offset  |
|  - Retry logic        |
|  - Trigger callbacks  |
+-----------------------+
