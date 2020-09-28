.equ HEX_DISPLAY1 = 0x00
.equ HEX_DISPLAY2 = 0x01
.equ BUTTONS = 0x02
.equ RANDOM = 0x07
.equ BTN_NO_REPEAT = 0x01

main: LDI R0, HEX_DISPLAY1 ; Load IO addresses
      LDI R1, HEX_DISPLAY2
      LDI R2, BUTTONS
      LDI R3, RANDOM
      LDI R4, BTN_NO_REPEAT
      ST (R2), R4          ; Disable button repetition
      
loop: LD R4, (R2)          ; Get pressed button
      CPI R4, 0            ; Check for any pressed
      BREQ loop            ; Loop over
      LD R4, (R1)          ; Get second displayed number
      ST (R0), R4          ; Show it on the first display
      LD R4, (R3)          ; Get a new, random number
      ST (R1), R4          ; Show it on the second display
      JMP loop