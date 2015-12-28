# Why set these magical fuse bytes? #

Before your programmer can talk to your AVR microcontroller, you probably need to set the AVR Fuse bits (basic settings of how the MCU operates) so it will look for an external  crystal oscillator. The program expects to be running at 20 Mhz.


# Fuse byte values #

**High byte** = 0xDF

**Low byte** = 0xFF

Extended fuse byte (optional -- probably already set, check manual) = 0x01