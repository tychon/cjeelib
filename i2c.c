
#include <util/delay.h>

#include "bits.h"
#include "jeenode_pins.h"

#include "i2c.h"

#ifndef HOLD_MICROSECONDS
  // 1 / (10 us) = 100 KHz
  #define HOLD_MICROSECONDS 300
  // 1 / (2.5 us) = 400 KHz
  //#define HOLD_MICROSECONDS 2.5
  // 1 / (0.588 us) = 1.7 MHz
  //#define HOLD_MICROSECONDS 0.588
#endif

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

#define HOLD   (_delay_ms(HOLD_MICROSECONDS))

/**
 * Set up pins, SCL and SDA are high afterwards.
 */
void i2c_init(i2cport *port) {
  // put SDA high
  SETSDAOUT;
  // put SCL high
  SETSCLOUT;
}

/**
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

//TODO wrong, insert HOLDs
uint8_t i2c_read(i2cport *port) {
  uint8_t data = 0;
  SETSDAIN;
  for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
    SCLHI;
    if (SDAIN) data |= mask;
    SCLLO;
  }
  SETSDAOUT;
  // send acknowledgement
  SDAOUT(0);
  SCLHI;
  SCLLO;
  SDAOUT(1);
  return data;
}

/**
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

#undef SETSDAIN
#undef SETSDAOUT
#undef SDAOUT
#undef HOLD
#undef SCLHI
#undef SCLLO

