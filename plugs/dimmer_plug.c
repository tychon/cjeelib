
///// Register addresses /////
#define MODE1  0x00
#define MODE2  0x01
#define PWM(x) (0x02+(x))
#define GRPPWM  0x12
#define GRPFREQ 0x13
#define LEDOUT(x) (0x14+(x))
#define SUBADR(x) (0x18+(x))
#define ALLCALLADR 0x1B

///// Auto-Increment options ////
// to be ored with register addresses
#define AUTOINCR_ALL (0x04 << 5)     // 0x00 to 0x1B
#define AUTOINCR_PWM (0x05 << 5)     // 0x02 to 0x11
#define AUTOINCR_GRP (0x06 << 5)     // 0x12 to 0x13
#define AUTOINCR_PWM_GRP (0x07 << 5) // 0x02 to 0x13

///// LED driver options /////
// Retrieve register address of driver for given LED index.
#define LEDDRIVER(x) LEDOUT((x)/4)
// Retrieve position for LDR value in LEDOUT(x) register
#define LDR_SHIFT(x) (((x)%4) * 2)

#define LDR_OFF(x)     (0x00 << LDR_SHIFT(x)) // LED off
#define LDR_ON(x)      (0x01 << LDR_SHIFT(x)) // LED on
#define LDR_PWM(x)     (0x02 << LDR_SHIFT(x)) // LED controlled by PWM(x)
#define LDR_PWM_GRP(x) (0x03 << LDR_SHIFT(x)) // LED controlled by PWM(x) and GRP

#define LDR_OFF_ALL     0x00
#define LDR_ON_ALL      0x55 // = 0b01010101
#define LDR_PWM_ALL     0xAA // = 0b10101010
#define LDR_PWM_GRP_ALL 0xFF // = 0b11111111

// Example for setting driver of LED12 and LED13 to PWM:
// register address: LEDDRIVER(12)
// register value:   (LDR_PWM(12) | LDR_PWM(13))

