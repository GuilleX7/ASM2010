.equ HEX_DISPLAY1 = 0x00
.equ HEX_DISPLAY2 = 0x01

main: LDI R0, HEX_DISPLAY1 ; Load IO addresses
      LDI R1, HEX_DISPLAY2
      LDI R2, 0            ; Clear counters
      LDI R3, 0
      CALL show
loop: ADDI R3, 1           ; Add to display 2
      CPI R3, 0            ; Check for overflow
      BREQ add
      JMP noadd
add:  ADDI R2, 1           ; Add to display 1
noadd:CALL show            ; Show counters
      JMP loop             ; Loop over
      
show: ST (R0), R2
      ST (R1), R3
      RET