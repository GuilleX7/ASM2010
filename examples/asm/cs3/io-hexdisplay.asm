; This program shows a counter in the
; hexadecimal display

.equ HEX_DISPLAY = 0x00

main: LDI R6, HEX_DISPLAY
      LDI R1, 0
      CALL show
loop: SUBI R1, -1
      CALL show
      JMP loop
      
show: ST Y, R1
      RET
