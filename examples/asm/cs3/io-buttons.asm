; Click on the buttons and see how the
; hexadecimal display changes!

.equ HEX_DISPLAY = 0x00
.equ BUTTONS = 0x01
.equ BTN_NO_REPEAT = 0x01

main: LDI R6, HEX_DISPLAY
      LDI R7, BUTTONS
      LDI R2, BTN_NO_REPEAT
      ST Z, R2          ; Disable button repetition
      
loop: LD R3, Z          ; Get pressed button
      CPI R3, 0            ; Check for any button pressed
      BREQ loop            ; Loop over
      CPI R3, 0x01         ; Button A pressed
      BREQ bt_a
      CPI R3, 0x02         ; Button B pressed
      BREQ bt_b
      CPI R3, 0x04         ; Button C pressed
      BREQ bt_c
      CPI R3, 0x08         ; Button D pressed
      BREQ bt_d
      CPI R3, 0x10         ; Button E pressed
      BREQ bt_e
      CPI R3, 0x20         ; Button F pressed
      BREQ bt_f
      JMP bt_gh            ; Other button pressed

bt_a: LDI R3, 0x0a
      ST Y, R3
      JMP loop

bt_b: LDI R3, 0x0b
      ST Y, R3
      JMP loop

bt_c: LDI R3, 0x0c
      ST Y, R3
      JMP loop

bt_d: LDI R3, 0x0d
      ST Y, R3
      JMP loop

bt_e: LDI R3, 0x0e
      ST Y, R3
      JMP loop

bt_f: LDI R3, 0x0f
      ST Y, R3
      JMP loop

bt_gh: LDI R3, 0x00
       ST Y, R3
       JMP loop
