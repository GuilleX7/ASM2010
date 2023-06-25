; This program will show a random number
; every time you click in any button

.equ HEX_DISPLAY = 0x00
.equ BUTTONS = 0x01
.equ RANDOM = 0x03
.equ BTN_NO_REPEAT = 0x01

main: LDI R0, HEX_DISPLAY
      LDI R1, BUTTONS
      LDI R2, RANDOM
      LDI R3, BTN_NO_REPEAT
      ST (R1), R3           ; Disable button repetition
      
loop: LD R3, (R1)           ; Get pressed button
      CPI R3, 0             ; Check for any pressed
      BREQ loop
      LD R3, (R2)           ; Get a random number
      ST (R0), R3           ; Show it on the display
      JMP loop
