# MIDI CC Chart #

|**CC**|**Name**|**Comments**|
|:-----|:-------|:-----------|
|1     |Duty cycle|Duty Cycle (SQR Synth only)|
|0 - 48:|50% duty cycle|            |
|49 - 95: |25% duty cycle|            |
|96 - 127: |12.5% duty cycle|            |
|7     |Master volume |Not used on 3NGL|
|8     |Fine pitch 	|Bends pitch down, 0 = no pitch change|
|9     |Volume decay enable|> 64 = on, Not used on 3NGL|
|10    |Volume decay amount|Not used on 3NGL|
|11    |Volume decay loop enable|Greater than 64 = on, Not used on 3NGL|
|12    |Sweep enable|Greater than 64 = on|
|13    |Sweep direction|Greater than 64 = up. Less than 64 = down|
|14    |Sweep amount |            |
|15    |Sweep loop enable|Greater than 64 = on|

# MIDI Channel Selection #

Each 4-bit Synth module can be assigned their own unique MIDI channel number (1 - 16), or you can have synths share the same channel, which means they will play identical notes and receive the same control messages. The DIP switch with the four switches each corresponds to a binary digit.

When the switch is "ON" it is a binary 1. When it is switch "OFF" (away from ground) it is a binary 0.

|Binary number | MIDI Channel|
|:-------------|:------------|
|0000          |1            |
|0001 	        |2            |
|0010 	        |3            |
|0011 	        |4            |
|0100 	        |5            |
|0101 	        |6            |
|0110 	        |7            |
|0111 	        |8            |
|1000 	        |9            |
|1001 	        |10           |
|1010 	        |11           |
|1011 	        |12           |
|1100 	        |13           |
|1101 	        |14           |
|1110 	        |15           |
|1111 	        |16           |