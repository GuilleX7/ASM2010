# CS2010 (Logisim version)
## Overview
This folder contains an implementation of the CS2010 architecture as detailed in https://www.dte.us.es/docencia/etsii/gii-is/estructura-de-computadores/grupo-4-2018/CS2010-2013.pdf

You will need Logisim in order to use the implementation. Logisim can be downloaded for free at http://www.cburch.com/logisim/download.html. It only requires Java 5 or greater.

The Control Unit design (from now on, UC) is detailed in a spreadsheet that contains all the microoperations done by the computer in order to perform each instruction. It is strongly recommended to take an in-depth look of the spreadsheet before modifying the datapath or the control unit.

## Loading a program
After you've opened the CS2010 implementation, you should see an overview of the computer. You can reach the *MEMCODE* by double-clicking the *MEMCODE* item at the left panel. Once you're in, you can right-click the ROM and load a program.

Logisim only allows to load hexadecimal files to both ROM and RAM memories. **AS2010 hexadecimal format is intended to be loaded here**, as it is fully compatible with Logisim.

## Running a program
After a program is loaded, you only need to press *RESET / START* button that is connected to the UC in the CS2010 overview. This will reset the UC and will send the reset signal to both *SP* and *PC* registers. Then, either toggling the clock (CTRL+T) or activating it (CTRL+K) will start the execution. *STOP* led will turn on when the computer has stopped (i.e. has reached STOP instruction).

Please note that *RESET / START* button do not reset registers nor RAM. In order to do so, you need to press CTRL+R, reseting the simulation.
