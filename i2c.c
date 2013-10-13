
#include <util/delay.h>

#include "bits.h"
#include "jeenode_pins.h"

#include "i2c.h"

#define SETSDAIN (pin_pull_up(DPORT, port->pin_sda), pin_as_input(DDDR, port->pin_sda))
#define SDAIN    (digital_read(DPIN, port->pin_sda))
#define SETSDAOUT   (digital_on(DPORT, port->pin_sda), pin_as_output(DDDR, port->pin_sda))
#define SDAOUT(val) (digital_write(DPORT, port->pin_sda, val))
#define SDAHI       (digital_on(DPORT, port->pin_sda))
#define SDALO       (digital_off(DPORT, port->pin_sda))

#define SETSCLIN (pin_pull_up(APORT, port->pin_scl), pin_as_input(ADDR, port->pin_scl))
#define SCLIN    (digital_read(APIN, port->pin_scl))
#define SETSCLOUT (digital_on(APORT, port->pin_scl), pin_as_output(ADDR, port->pin_scl))
#define SCLHI     (digital_on(APORT, port->pin_scl))
#define SCLLO     (digital_off(APORT, port->pin_scl))

#define HOLD I2C_HOLD

/**
 * Set up pins, SCL and SDA are high afterwards.
 * Finishes with a hold.
 */
void i2c_init(i2cport *port) {
  // put SDA high
  SETSDAOUT;
  // put SCL high
  SETSCLOUT;
  HOLD;
}

/**
 * Generate start condition and send device address.
 * Expects SCL and SDA to be high in the beginning.
 * SCL is low afterwards. Finishes with hold.
 * @returns If the receiver sent the ACK bit.
 */
bool i2c_start(i2cport *port, uint8_t address, bool read) {
  SDAOUT(0); HOLD;
  SCLLO; HOLD;
  return i2c_write(port, (address << 1) | read);
}

/**
 * Generate a restart condition and send device address.
 * This function may not be used directly after an
 * i2c_init (insert an I2C_HOLD).
 * Expects SCL to be low in the beginning.
 * SCL is low afterwards. Finishes with hold.
 * @returns If the receiver sent the ACK bit.
 */
bool i2c_restart(i2cport *port, uint8_t address, bool read) {
  SDAOUT(1);
  SCLHI;     HOLD;
  SDAOUT(0); HOLD;
  SCLLO;     HOLD;
  return i2c_write(port, (address << 1) | read);
}

/**
 * Write byte and receive acknowledgement.
 * Expects SCL to be low in the beginning.
 * SCL is low afterwards. Finishes with a hold.
 * @returns If the receiver sent the ACK bit.
 */
bool i2c_write(i2cport *port, uint8_t byte) {
  // send data
  for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
      SDAOUT(byte & mask);
      SCLHI; HOLD;
      SCLLO; HOLD;
  }
  // read ACK bit
  SETSDAIN;
  SCLHI; HOLD;
  uint8_t ack = ! SDAIN;
  SCLLO; HOLD;
  SETSDAOUT;
  return ack;
}

/**
 * Generates a restart condition and sends the device address
 * and then the register address.
 * There is no stop condition generated.
 * @returns If the receiver sent the ACK bit.
 */
bool i2c_register_addr(i2cport *port, uint8_t device_address, uint8_t register_address) {
  return i2c_restart(port, device_address, I2CWRITE)
      && i2c_write(port, register_address);
}

/**
 * Generates a re-/start condition and sends the device address
 * and then register address and one byte of data.
 * There is no stop condition generated.
 * @returns If the receiver sent the ACK bit.
 */
bool i2c_register_write(i2cport *port, uint8_t deviceaddr, uint8_t regaddr, uint8_t byte) {
  return i2c_restart(port, deviceaddr, I2CWRITE)
      && i2c_write(port, regaddr)
      && i2c_write(port, byte);
}

/**
 * Read one byte from slave.
 * The last byte read before a stop condition has to be read with end = true
 * to be sure, that the slave is noticing the stop condition (and not pulling
 * low SDA to send a bit).
 * Escpects SCL to be low in the beginning.
 * SCL is low afterwards. Finishes with a hold.
 * @param end If end is true, there is now ACK generated signalling the slave
 * to stop transmitting data. Otherwise an ACK is generated.
 * @returns the received byte.
 */
uint8_t i2c_read(i2cport *port, bool end) {
  uint8_t data = 0;
  SETSDAIN;
  for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
    SCLHI; HOLD;
    if (SDAIN) data |= mask;
    SCLLO; HOLD;
  }
  SETSDAOUT;
  if (end) { // send acknowledgement
    SDAOUT(0);
    SCLHI; HOLD;
    SCLLO; HOLD;
    SDAOUT(1);
  } else { // send no acknowledgement
    SDAOUT(1);
    SCLHI; HOLD;
    SCLLO; HOLD;
  }
  return data;
}

/**
 * Address slave register for reading. Does not read any data.
 * Do reading yourself, so you can have error codes of this initialization as
 * return value.
 */
uint8_t i2c_register_read_init(i2cport *port, uint8_t device_address, uint8_t register_address) {
  return i2c_restart(port, device_address, I2CWRITE)
      && i2c_write(port, register_address)
      && i2c_restart(port, device_address, I2CREAD);
}

/**
 * Address slave register and read one byte of data.
 * @param byte place to write received data to.
 * @param end Whether to signalise the slave to stop transmission.
 * @returns If the process of addressing the slave and the register were successful.
 */
bool i2c_register_read(i2cport *port, uint8_t device_address, uint8_t register_address
                     , uint8_t *byte, bool end) {
  if (! i2c_restart(port, device_address, I2CWRITE)
   || ! i2c_write(port, register_address)
   || ! i2c_restart(port, device_address, I2CREAD)) return false;
  *byte = i2c_read(port, end);
  return true;
}

/**
 * Generate a stop condition.
 * Expects SCL to be low in the beginning.
 * Finishes with a hold.
 */
void i2c_stop(i2cport *port) {
  SDAOUT(0);
  SCLHI;
  HOLD;
  SDAOUT(1);
  HOLD;
}

