
#ifndef _I2C_H
#define _I2C_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  int pin_scl, pin_sda;
} i2cport;

#define I2CPORT1(name) i2cport name = {.pin_scl = JP1A, .pin_sda = JP1D};
#define I2CPORT2(name) i2cport name = {.pin_scl = JP2A, .pin_sda = JP2D};
#define I2CPORT3(name) i2cport name = {.pin_scl = JP3A, .pin_sda = JP3D};
#define I2CPORT4(name) i2cport name = {.pin_scl = JP4A, .pin_sda = JP4D};

#define I2CWRITE 0
#define I2CREAD  1

void i2c_init(i2cport *port);
bool i2c_start(i2cport *port, uint8_t address, bool read);
bool i2c_restart(i2cport *port, uint8_t address, bool read);
bool i2c_write(i2cport *port, uint8_t byte);
uint8_t i2c_read(i2cport *port);
void i2c_stop(i2cport *port);

#endif // _I2C_H

