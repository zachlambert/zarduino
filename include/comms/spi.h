#ifndef ZARDUINO_COMMS_SPI
#define ZARDUINO_COMMS_SPI

#include "core/pins.h"

#include <stdint.h>
#include <stdlib.h>

typedef enum {
   SPI_PHASE_SAMPLE_ON_LEADING,
   SPI_PHASE_SETUP_ON_LEADING
} SPIPhase;

typedef enum {
    SPI_POLARITY_LEADING_IS_RISING,
    SPI_POLARITY_LEADING_IS_FALLING
} SPIPolarity;

typedef enum {
    SPI_DATA_ORDER_MSB_FIRST, 
    SPI_DATA_ORDER_LSB_FIRST, 
} SPIDataOrder;

typedef enum {
    SPI_FREQUENCY_DIV_4,
    SPI_FREQUENCY_DIV_16,
    SPI_FREQUENCY_DIV_64,
    SPI_FREQUENCY_DIV_128,
    SPI_FREQUENCY_DIV_2,
    SPI_FREQUENCY_DIV_8,
    SPI_FREQUENCY_DIV_32,
    // Also 64
} SPIFrequency;

typedef struct {
    SPIPhase phase;
    SPIPolarity polarity;
    SPIDataOrder data_order;
    SPIFrequency frequency;
    void (*callback)(uint8_t);
} SPIConfig;

SPIConfig spi_create_config(void);
void spi_init_master(SPIConfig *config);
uint8_t spi_read_byte(Pin CS);
void spi_write_byte(Pin CS, uint8_t data);
void spi_read_bytes(Pin CS, uint8_t *data, size_t len);
void spi_write_bytes(Pin CS, uint8_t *data, size_t len);
uint8_t spi_transfer_byte(Pin CS, uint8_t data_in);
void spi_transfer_bytes(
    Pin CS, uint8_t *data_in, uint8_t *data_out, size_t len
);

#endif
