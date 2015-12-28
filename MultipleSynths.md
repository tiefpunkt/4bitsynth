![http://4bitsynth.googlecode.com/svn/trunk/images/multiple.png](http://4bitsynth.googlecode.com/svn/trunk/images/multiple.png)

To connect multiple synths, connect all of their respective Audio + (red lines here) signals into one (I currently use 8 connected together, this can get messy, so use the breadboard for this however is most convenient for you.

You only need one ground line (green line here), you can plug it into one of the rails marked with the blue line, the power supply ground, or what have you.

Finally, you need to share the digital MIDI-in signal (pink line here). Since you can share these directly, you don't need to add the PC-900 optocoupler or the MIDI-in connector to every synth module. I have successfully shared the signal from just one optocoupler with 8 synths without any problems. If you wanted a "proper" MIDI setup, you'd have a MIDI thru portion for every synth module, and you'd need 8 little short MIDI cables going to and from each one. No thanks.

Here is a picture of my setup with 7 synths (3 SQRs, 2 3NGLS, and 2 XORs):

![http://4bitsynth.googlecode.com/svn/trunk/images/multipleexample.png](http://4bitsynth.googlecode.com/svn/trunk/images/multipleexample.png)

Here is a link to the high resolution image:
[High Resolution Multiple Synth Photo](http://4bitsynth.googlecode.com/files/multipleexample_large.jpg)