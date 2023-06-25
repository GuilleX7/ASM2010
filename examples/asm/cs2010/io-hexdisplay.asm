; This program shows a counter in the
; hexadecimal display

.equ HEX_DISPLAY = 0x00

main: LDI R0, HEX_DISPLAY
      LDI R1, 0
      CALL show
loop: ADDI R1, 1
      CALL show
      JMP loop
      
show: ST (R0), R1
      RET
