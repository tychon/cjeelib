
///// Register addresses /////
#define DPMODE1  0x00
#define DPMODE2  0x01
#define DPPWM(x) (0x02+(x))
#define DPGRPPWM  0x12
#define DPGRPFREQ 0x13
#define DPLEDOUT(x) (0x14+(x))
#define DPSUBADR(x) (0x18+(x))
#define DPALLCALLADR 0x1B

///// Auto-Increment options ////
// to be ored with register addresses
#define DPAUTOINCR_ALL (0x04 << 5)     // 0x00 to 0x1B
#define DPAUTOINCR_PWM (0x05 << 5)     // 0x02 to 0x11
#define DPAUTOINCR_GRP (0x06 << 5)     // 0x12 to 0x13
#define DPAUTOINCR_PWM_GRP (0x07 << 5) // 0x02 to 0x13

///// LED driver options /////
// Retrieve register address of driver for given LED index.
#define DPLEDDRIVER(x) LEDOUT((x)/4)
// Retrieve position for LDR value in LEDOUT(x) register
#define DPLDR_SHIFT(x) (((x)%4) * 2)

#define DPLDR_OFF(x)     (0x00 << LDR_SHIFT(x)) // LED off
#define DPLDR_ON(x)      (0x01 << LDR_SHIFT(x)) // LED on
#define DPLDR_PWM(x)     (0x02 << LDR_SHIFT(x)) // LED controlled by PWM(x)
#define DPLDR_PWM_GRP(x) (0x03 << LDR_SHIFT(x)) // LED controlled by PWM(x) and GRP

// set all drivers in one driver register to same value
#define DPLDR_OFF_ALL     0x00
#define DPLDR_ON_ALL      0x55 // = 0b01010101
#define DPLDR_PWM_ALL     0xAA // = 0b10101010
#define DPLDR_PWM_GRP_ALL 0xFF // = 0b11111111

// Example for setting driver of LED12 and LED13 to PWM:
// register address: LEDDRIVER(12)
// register value:   (LDR_PWM(12) | LDR_PWM(13))

