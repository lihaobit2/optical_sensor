This project implements the gesture demo for the Si1143-EB board included in 
the SI1140DK Development Kit.


Compile switches are used to implement different settings depending on the 
usage case. 

For example, the INDOORS compile switch configures the device to be in
a sensitive mode that will offer good distance of detection but may not be 
able to function outdoors under higher ambient light levels. The 
GENERAL_CLEAR_OVERLAY and GENERAL_INFRARED_OVERLAY switches allow the system
to operate under the highest levels of sunlight that the overlay in the system 
will allow.

The UDPWaveformViewer displays the following items:

    VIS  / Ch 0 - Ambient VIS
    IR   / Ch 1 - Ambient IR
    PS1  / Ch 2 - PS1 after IR Correction
    PS2  / Ch 3 - PS2 after IR Correction
    PS3  / Ch 4 - PS3 after IR Correction

Search through the source code for the REPLACE_0 macros. These
are the calls used to write to the UDP WaveformViewer.

It is important to note that channels 2, 3, and 4 are displayed AFTER infrared
correction has been implemented. Normally, Channels 2, 3, and 4 will 
automatically display raw PS1, PS2, and PS3 samples. 

For example, Channel 1 "Ambient IR" are raw values directly from the 
Si114x_PGM_Toolkit.dll to the UDPWaveformViewer

In addition to displaying the above items in the UDPWaveformViewer, the 
console port will provide information such as "swipe left" and "swipe right" 
gestures. The console display is implemented using standard C print() 
statements in the code. After each gesture, the console will also display 
4 pieces of information used by the gesture algorithm. The data labeled 1, 2, 
and 3 indicate the timing of the midpoint of each PS1, PS2, and PS3 waveforms 
from a swipe. The order of the midpoints determines the gesture. The final 
piece of information is a swipe speed that helps indicate how fast a swipe was.

The supported gestures are:

swipe left  - full hand swipe from the left side of the board to the right side
swipe right - full hand swipe from the right side of the board to the left side
swipe up    - full hand swipe from the bottom side of the board to the top side
swipe down  - full hand swipe from the top side of the board to the bottom side
tap         - a tap motion where the hand approaches the board from direction 
              above the board and retracts

