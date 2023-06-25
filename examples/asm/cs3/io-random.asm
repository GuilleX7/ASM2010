; This program will show a random number
; every time you click in any button

.equ HEX_DISPLAY = 0x00
.equ BUTTONS = 0x01
.equ RANDOM = 0x03
.equ BTN_NO_REPEAT = 0x01

main: LDI R6, HEX_DISPLAY
      LDI R7, BUTTONS
      LDI R3, BTN_NO_REPEAT
      ST Z, R3              ; Disable button repetition
      
loop: LD R3, Z              ; Get pressed button
      CPI R3, 0             ; Check for any pressed
      BREQ loop
      LDI R7, RANDOM        ; Load the address of the random generator
      LD R3, Z              ; Get a random number
      ST Y, R3              ; Show it on the display
      LDI R7, BUTTONS       ; Restore the address of the buttons
      JMP loop
