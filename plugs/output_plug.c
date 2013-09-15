
/** NOTE:
 * If you aren't in sequential mode (this mode is somehow useless in this
 * configuration), send a stop signal after every register write access!
 * The plug works somehow randomly, if you don't send stop signals.
 */

/**
 * This interface works only for setting outputs of the plug, even
 * when the MCP23008 supports inputs. Don't forget the Darlington transistor
 * array connected to it.
 */

/**
 * Set up the Output Plug. This is neccessary, because the output pins of the
 * MCP23008 are configured as inputs on POR / restart.
 */
#define OP_INIT(port) { \
  i2c_register_write(&port, ADDRESS, 0x00, 0x00); \
  i2c_stop(&port); \
}

/**
 * To set output just do
 * 
 *   OP_SET(port, ADDRESS, YOURBITS);
 * 
 * Wich becomes
 * 
 *   i2c_register_write(&port, ADDRESS, 0x0A, YOURBITS);
 *   i2c_stop(&port);
 * 
 * Where `port` is your I2C port, `ADDRESS` the device address and `YOURBITS`
 * is the desired output status of the plug. For example
 * 
 *   YOURBITS = (1 << 5) | (1 << 3)
 * 
 * To active pin 3 and 5 and turn off all other pins (pins 0, 1, 2, 4, 6 and 7).
 * The third argument to i2c_register_write is `0x0A` and is the register
 * address of the OLAT register of the MCP23008.
 */
#define OP_SET(port, addr, bits) { \
  i2c_register_write(&port, ADDRESS, 0x0A, bits); \
  i2c_stop(&port); \
}
