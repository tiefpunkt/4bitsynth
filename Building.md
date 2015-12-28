# Introduction #

Here are step-by-step instructions for building a 4bit synth, including the schematics and bill of materials. These instructions assume you know the following:

  * basic electronics terms
  * how a breadboard works
  * already have software installed on the microcontroller unit (MCU)

You can find numerous articles on the above. For installing software onto the MCU, I recommend the [AVRDude](http://www.bsdhome.com/avrdude/) software package and LadyAda's [USBTiny](http://www.ladyada.net/make/usbtinyisp/) In-System Programmer.

Step-by-step illustrations are included. To see a real photograph of a built synth (with others connected as well) see MultipleSynths.

## Schematic ##

http://4bitsynth.googlecode.com/svn/trunk/schematic/4bitschematic.pdf

## Bill of materials ##

### Mouser.com ###
To add a synth quickly to your mouser.com order, paste the following into the [Bill of Materials (BOM) Import page](http://www.mouser.com/BOM/BOM.aspx) to add them to your cart. Click "Import BOM" after you paste. You can do this as many times as you want synths. If you find any mistakes in the below list or the component reference below that, _please_ contact me.

140-50N2-220J-RC|2

774-2084|1

140-XRL25V2200-RC|1

78-1N914|1

806-KCDX-5S-N|1

291-5.6K-RC|1

291-220-RC|1

291-1K-RC|5

291-510-RC|3

556-ATMEGA48-20PU|1

852-PC900V0NSZXF|1

520-HCU2000-20X|1

### Component Reference Table ###

|**Component Reference**|**Description**|**[Mouser](http://www.mouser.com) part no.**|
|:----------------------|:--------------|:-------------------------------------------|
|C1                     |	22pF Ceramic Disc Capacitor|140-50N2-220J-RC                            |
|C2                     |	22pF Ceramic Disc Capacitor|140-50N2-220J-RC                            |
|C3                     |	MIDI Chan. Select DIP Switch (4x SPST)|774-2084                                    |
|C4                     |	2200uF Capacitor, optional but recommended to have one or two on the board to reduce noise|140-XRL25V2200-RC                           |
|D1                     |	1N914 Diode   |78-1N914                                    |
|P1                     |	MIDI In Female Connector|806-KCDX-5S-N                               |
|P2                     |	Audio Out     |N/A                                         |
|P3                     |	Power (5V)    |N/A                                         |
|P4                     |	Audio In      |N/A                                         |
|P5                     |	MIDI IN Alt. (for sharing one MIDI line)|N/A                                         |
|[R1](https://code.google.com/p/4bitsynth/source/detail?r=1)|	5.6k ohm resistor|291-5.6K-RC                                 |
|[R10](https://code.google.com/p/4bitsynth/source/detail?r=10)|	220 ohm resistor|291-220-RC                                  |
|[R2](https://code.google.com/p/4bitsynth/source/detail?r=2)|	1K ohm resistor|291-1K-RC                                   |
|[R3](https://code.google.com/p/4bitsynth/source/detail?r=3)|	1K ohm resistor|291-1K-RC                                   |
|[R4](https://code.google.com/p/4bitsynth/source/detail?r=4)|	1K ohm resistor|291-1K-RC                                   |
|[R5](https://code.google.com/p/4bitsynth/source/detail?r=5)|	1K ohm resistor|291-1K-RC                                   |
|[R6](https://code.google.com/p/4bitsynth/source/detail?r=6)|	1K ohm resistor|291-1K-RC                                   |
|[R7](https://code.google.com/p/4bitsynth/source/detail?r=7)|	500 ohm resistor|291-510-RC                                  |
|[R8](https://code.google.com/p/4bitsynth/source/detail?r=8)|	500 ohm resistor|291-510-RC                                  |
|[R9](https://code.google.com/p/4bitsynth/source/detail?r=9)|	500 ohm resistor|291-510-RC                                  |
|U1                     |	Atmel AVR ATMega48|556-ATMEGA48-20PU                           |
|U2                     |	Sharp PC900 Optocoupler|852-PC900V0NSZXF                            |
|X1                     |	20MHz Crystal, 20pF Load Capacitance|520-HCU2000-20X                             |

## Step one: placing major components ##

![http://4bitsynth.googlecode.com/svn/trunk/images/1.png](http://4bitsynth.googlecode.com/svn/trunk/images/1.png)

![http://4bitsynth.googlecode.com/svn/trunk/images/realbuildphotos/majorcomps.jpg](http://4bitsynth.googlecode.com/svn/trunk/images/realbuildphotos/majorcomps.jpg)


First, we place our major components: the integrated chips. At the top is basically a set of four switches that will serve as a MIDI channel selector.

Next is the brains of the synth: the microcontroller unit ("MCU"). In this case, it's an Atmel AVR ATMega48. You'll need one of these per synth.

Next down is a Sharp PC-900 photocoupler. This isolates the MIDI signals from the cables from the the rest of the components, which may be sensitive to electrical surges. It sends logic voltages (0 or 5 volts) to the MCU. If you are connecting multiple synths together, you should probably only need one of these.

Finally, the MIDI-In connector. It might be a little tight getting into the breadboard, but it should fit. If you have a lot of trouble, you can get a separate connector that you can solder some thinner wires to. With multiple synths, you probably only need one of these too.


## Step two: connecting power ##

![http://4bitsynth.googlecode.com/svn/trunk/images/2.png](http://4bitsynth.googlecode.com/svn/trunk/images/2.png)

Our ICs need power to operate. I like to use red wires for +5V and black wires for ground. Attach them as shown. Please double check the data sheets to be sure you are connecting power to the correct pins!

Connect your +5V power supply to the red vertical strips on each side, and the Ground (-) to the blue/black strips on each side. You only need to connect the power to one of the squares on each of those columns -- they are connected all the way down, unlike the other strips.

The four black wires going from the switches to ground serve to bring pins low on the MCU, which to the MCU, is actually interpreted as binary 1. See the MIDI Channel Selection section for more information.

The power lines on the optocoupler are not "power sources" per se, but they are part of a small transistor circuit. Transistors are beyond the scope of this manual. Just know that it basically transforms the MIDI input signal to output 5V (binary 1) and 0V (binary 0) to the MCU, which is exactly what it wants.


## Step three: connecting I/O ##

![http://4bitsynth.googlecode.com/svn/trunk/images/3.png](http://4bitsynth.googlecode.com/svn/trunk/images/3.png)

Now we connect the switch to the MCU (yellow lines). Each line corresponds to one bit of a four-bit binary number, which means that it can be 0 - 15, each corresponding to one of the possible 16 MIDI Channels.

The blue line is the digital connection to the MCU's general purpose Universal Synchronous-Asynchronous Receiver/Transmitter (USART). It is essentially the MIDI message receiver. This line can be shared with other 4bit Synths directly.

The green line is part of the raw MIDI signal (not a voltage signal) which will be transformed by the optocoupler's transistor circuit.


## Step four: placing minor components, MIDI ##

![http://4bitsynth.googlecode.com/svn/trunk/images/4.png](http://4bitsynth.googlecode.com/svn/trunk/images/4.png)

Now add the two resistors (220 ohm and 280 ohm) to the MIDI-In part of the circuit. Also, make sure you insert the diode (1N914) the correct way. Check for the line marking.


## Step five: placing minor components, Microcontroller ##

![http://4bitsynth.googlecode.com/svn/trunk/images/5.png](http://4bitsynth.googlecode.com/svn/trunk/images/5.png)

Now add the two 22pF ceramic capacitors and the 20MHz crystal. These will provide the clock source for the MCU. All of these are not polarized, so you can insert them either way, there is no "backwards" for these particular parts.

Also, above is a 5.6k resistor which will keep the MCU's reset line (pin 1) at 5V -- this will keep the MCU out of reset mode. If you want to reset the MCU, just plug a wire into this strip, and connect it to ground for a second. Be careful not to connect this to the same strip as the blue line.


## Step six: adding a Digital-Analog Converter ##

![http://4bitsynth.googlecode.com/svn/trunk/images/6.png](http://4bitsynth.googlecode.com/svn/trunk/images/6.png)

Now we add the Digital-Analog Converter (DAC) to the mix. This will let the 4-bit "audio level" expressed by the MCU and turn it into a corresponding voltage level -- the audio signal.

You will probably want to plug this part into a second breadboard to the right of this one.


## Step seven: connection to audio out ##

![http://4bitsynth.googlecode.com/svn/trunk/images/7.png](http://4bitsynth.googlecode.com/svn/trunk/images/7.png)

I use a couple of alligator clips for this. You can connect Audio + to both the Left and Right of a stereo signal to plug into a some speakers or your microphone/line in of your computer. Connect the Audio - to the audio cable's ground (sometimes a shielding).