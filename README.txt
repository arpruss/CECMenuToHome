Tap into your HDMI cable in some way, and attach:

 - HDMI shield to blue pill ground
 - HDMI CEC to blue pill PA0
 - optional, once everything is working: HDMI 5V to blue pill 5V so you don't need a USB connector
 
In the code, change the physical address if your HDMI cable is hooked to something
other than the first HDMI input. You may need some trial and error to get it right.

This uses the libmaple version of the stm32f1 core and the version of the CEC library 
here: https://github.com/arpruss/cec