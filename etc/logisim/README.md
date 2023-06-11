# CS2010 (Logisim version)

## Overview

This folder contains two implementations of the CS2010 architecture as detailed in https://www.dte.us.es/docencia/etsii/gii-is/estructura-de-computadores/grupo-4-2018/CS2010-2013.pdf, one with IO extension (CS2010-IO) and one without (CS2010). See more details of the CS2010-IO in the paragraphs below.

You will need Logisim in order to use the implementations. Logisim can be downloaded for free at http://www.cburch.com/logisim/download.html. It only requires Java 5 or greater.

The Control Unit design (from now on, UC) is detailed in a spreadsheet that contains all the microoperations done by the computer in order to perform each instruction. It is strongly recommended to take an in-depth look at the spreadsheet before modifying the datapath or the control unit.

## Loading a program

After you've opened the CS2010 implementation, you should see an overview of the computer. You can reach the _MEMCODE_ by double-clicking the _MEMCODE_ item at the left panel. Once you're in, you can right-click the ROM and load a program.

Logisim only allows to load hexadecimal files to both ROM and RAM memories. **AS2010 hexadecimal format is intended to be loaded here**, as it is fully compatible with Logisim.

## Running a program

After a program is loaded, you only need to press _RESET / START_ button that is connected to the UC in the CS2010 overview. This will reset the UC and will send the reset signal to both _SP_ and _PC_ registers. Then, either toggling the clock (CTRL+T) or activating it (CTRL+K) will start the execution. _STOP_ led will turn on when the computer has stopped (i.e. has reached STOP instruction).

Please note that _RESET / START_ button do not reset registers nor RAM. In order to do so, you need to press CTRL+R, reseting the simulation.

## CS2010-IO memory

The CS2010-IO adds an IO controller so it can communicate with (virtual) external devices. The IO controller has support for eight different devices, each one mapped to a different byte in the IO memory. In fact, a device could take more than one byte just by connecting it to two or more IO ports. A general memory scheme would be:

```
                       -----------------------
                       -  EXTERNAL DEVICES   -
                       -----------------------
                                 ||
-------------------   ------------------------
-        RAM      -   -     IO CONTROLLER    -
-------------------   ------------------------
	    ||                       ||
----------------------------------------------
-               REST OF CS2010               -
----------------------------------------------
```

This scheme has several implications:

-   First eight bytes of RAM are wasted, in the sense that they are never used.
-   One could write (or read) more than one byte to an external device, as the memory is contained in the device as it prefers, but only one byte can be transferred through the IO bus at once (because IO memory is accesed with regular CS2010 instructions).

As an example of this, the keyboard integrated circuit can be configurated by writing at its memory address, but it can also be read to retrieve keycodes.

The general memory map would look like this:

| Range       | Mapped to |
| ----------- | --------- |
| 0x00 - 0x07 | I/O       |
| 0x08 - 0xFF | RAM       |

The eight bytes of I/O memory are mapped as following by default:

| Range       | Mapped to         |
| ----------- | ----------------- |
| 0x00        | Hex display (1)   |
| 0x01        | Hex display (2)   |
| 0x02        | Buttons           |
| 0x03        | Keyboard          |
| 0x04        | 7-segment display |
| 0x05 - 0x06 | LED matrix        |
| 0x07        | Random generator  |

Note that this configuration can be easily changed due to "standardization" of the IO connectors. Every device has to be connected, at least, to the matching IOz port in the IO controller, with z being a number from 0 to 7. Each port is mapped to the matching 0x0z address in the IO memory (IO2 is mapped to 0x02, for example). So, the only thing a device needs to change its mapped address is to change the IOz port at which it is connected.

## Default external devices

### Hex display

**Reading/Writing:** two hexadecimal digits packed in the same byte, each one mapped to a nibble.

| Bit     | 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
| ------- | --- | --- | --- | --- | --- | --- | --- | --- |
| Content | L3  | L2  | L1  | L0  | R3  | R2  | R1  | R0  |

Lx stands for each bit of the left digit, Rx stands for each bit of right digit. Both digits are coded as unsigned.

### Buttons

**Reading:** button signals are mapped to an 8-bit word containing:

| Bit     | 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
| ------- | --- | --- | --- | --- | --- | --- | --- | --- |
| Content | BH  | BG  | BF  | BE  | BD  | BC  | BB  | BA  |

Bx stands for each button signal. A button signal is 1 when the matching button is/has been pressed. Only one signal can be on at the same time. If a button has been pressed, no other button signal can become on until the last signal has been read. If all signals are set to 0, then no button has been pressed.

**Writing:** button signals can be configurated by writing the following values:

| Bit     | 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
| ------- | --- | --- | --- | --- | --- | --- | --- | --- |
| Content | -   | -   | -   | -   | -   | -   | -   | M   |

M can take two values:

| M   | Mode            | Description                                                                                                                                                                       |
| --- | --------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 0   | Default mode    | Button signal will be available for reading until the signal has been read and button is released.                                                                                |
| 1   | Non-repeat mode | Button signal will be available for reading only once when a button is pressed.<br>Once the signal has been read, the following signals will be 0 if the button is still pressed. |

Bits marked with a hyphen can take any value.

### Keyboard

**Reading:** keystrokes are mapped to an 8-bit word containing:

| Bit     | 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
| ------- | --- | --- | --- | --- | --- | --- | --- | --- |
| Content | 0   | A6  | A5  | A4  | A3  | A2  | A1  | A0  |

Ax stands for each digit of a ASCII-encoded character. Each time the device is read, the first unread character is send. The keyboard has a buffer with capacity for 256 unread characters and will discard the following characters if it is already full. Attempting to read the keyboard without characters waiting to be read will sent a byte with all its bits set to 0.

**Writing:** keyboard can be configurated by writing the following values:

| Bit     | 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
| ------- | --- | --- | --- | --- | --- | --- | --- | --- |
| Content | -   | -   | -   | -   | -   | -   | M1  | M0  |

Mx stands for each configuration bit. The following combinations of Mx are allowed:

| M1  | M0  | Action           | Description                                                                                                                                                                                                          |
| --- | --- | ---------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 0   | 0   | Clear buffer     | Clears any unread character in the buffer                                                                                                                                                                            |
| 0   | 1   | Set peek mode    | Sets to peek mode. Next read will retrieve a byte<br>with its less significant bit set to 1 if there is<br>any unread character in the buffer, or 0 otherwise.<br>Then, it will be set automatically to default mode |
| 1   | 0   | Set default mode | Sets to default mode. Reading the device will<br>retrieve the first unread character in the buffer.<br>This mode is automatically set after reading in peek<br>mode                                                  |
| 1   | 1   | -                | -                                                                                                                                                                                                                    |

Bits marked with a hyphen can take any value. Actions marked with a hyphen mean that no action will take place.

### 7-segment display

**Reading/Writing:** 7-segment signals packed in a byte:

| Bit     | 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
| ------- | --- | --- | --- | --- | --- | --- | --- | --- |
| Content | A   | B   | C   | D   | E   | F   | G   | DP  |

Each bit stands for the matching segment in a 7-segment display, as detailed below. A bit set to 1 will turn on the matching segment, while setting a bit to 0 will turn off the segment.

```
       A
     -----
  F |     | B
    |     |
     -----
  E |  G  | C
    |     |
     ----- . DP
       D
```

### LED matrix

**Reading/Writing:** Four columns of four cells packed in two bytes. The first byte will be called B0, which should be mapped to a lower address, while the second byte will be called B1 and should be mapped to a higher address.

At B0:
| Bit | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---------|----|----|----|----|----|----|----|----|
| Content | L7 | L6 | L5 | L4 | L3 | L2 | L1 | L0 |

At B1:
| Bit | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---------|----|----|----|----|----|----|----|----|
| Content | H7 | H6 | H5 | H4 | H3 | H2 | H1 | H0 |

Setting a bit to 1 will turn on the matching pixel, while setting it to 0 will turn off the pixel. Pixels have the following distribution and relationship with each bit above:

| Column 1 | Column 2 | Column 3 | Column 4 |
| -------- | -------- | -------- | -------- |
| L3       | L7       | H3       | H7       |
| L2       | L6       | H2       | H6       |
| L1       | L5       | H1       | H5       |
| L0       | L4       | H0       | H4       |

### Random generator

**Reading:** retrieve a random 8-bit value.

| Bit     | 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
| ------- | --- | --- | --- | --- | --- | --- | --- | --- |
| Content | R7  | R6  | R5  | R4  | R3  | R2  | R1  | R0  |

Rx stands for each random bit.

**Writing:** not allowed (won't take effect).

## IOz port details

This three-bit wide port contains the following signals:

| Bit | 2   | 1   | 0   |
| --- | --- | --- | --- |
|     | R   | W   | CLK |

-   CLK signal matchs the CS2010 clock, and should be used for synchronization
-   W signal requests the device to retrieve the data that comes from the IOIN bus (matchs with a ST instruction at the address where the device is mapped to)
-   R signal requests the device to put data in the IOHOUT bus (matchs with a LD instruction at the address where the device is mapped to)

Note that, while IOIN is a read-only bus (only CS2010 puts data in there), IOHOUT is a shared, writtable bus which **ONLY** the requested device by the CS2010 should write when signal W of the appropriate IOz port is on.
