.equ HEX_DISPLAY2 = 0x01
.equ BUTTONS = 0x02

main: LDI R0, HEX_DISPLAY2 ; Load IO addresses
      LDI R3, BUTTONS
loop: LD R4, (R3)          ; Get pressed button
      ST (R0), R4          ; Show it on hex display
      JMP loop             ; Loop over