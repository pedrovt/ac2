# --------------------------------
# Counter_ASM
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------

      .equ SFR_BASE_HI, 0xBF88   # 16 MSbits of SFR area

      .equ TRISE, 0x6100 	       # TRISE address is 0xBF886100
      .equ PORTE, 0x6110	       # PORTE address is 0xBF886110
      .equ LATE, 0x6120 	       # LATE  address is 0xBF886120
      .equ ODCE, 0x6130 	       # ODCE  address is 0xBF886130

      .equ TRISB, 0x6040 	       # TRISB address is 0xBF886040
      .equ PORTB, 0x6050	       # PORTB address is 0xBF886050
      .equ LATB, 0x6060 	       # LATB  address is 0xBF886060
      .equ ODCB, 0x6070 	       # ODCB  address is 0xBF886070

      .equ PRINT_STR, 8
      .equ READ_CORE_TIMER, 11
      .equ RESET_CORE_TIMER, 12

      .data
str:  .asciiz "Pedro Veloso Teixeira 84715\n"
      .text
      .globl main

main:
         lui $s0, SFR_BASE_HI

         # Set RE3-0 as outputs
         lw  $s1, TRISE($s0)
         andi $s1, $s1, 0xFFF0
         sw   $s1, TRISE($s0)

         la $a0, str
         li $v0, PRINT_STR
         syscall

         li $s2, 0  #counter -> s2
         li $s3, 0  #flag  = 0 up, = 1 down


while:
         li $s5, 500

if1:     bne $s2, 5, endIf1      # if (counter == 5) -> counter down (=5)
         li $s3, 1               #     flag = 1
         j if3                   # }
endIf1:

if2:     bne $s2, 0, endIf2      # if (counter == 0) { -> counter up (=0)
         li $s3, 0               #     flag = 0;
         j if3                   # }
endIf2:

if3:     beq $s3, 1, down         # up

         #li $a0, '+'
         #li $v0, 3
         #syscall

         addiu $s2, $s2, 1
         j endIf3

down:    subu $s2, $s2, 1

         #li $a0, '-'
         #li $v0, 3
         #syscall
endIf3:                          # send to leds

         move $a0, $s2
         li $v0, 7
         syscall

         li $a0, '\n'
         li $v0, 3
         syscall

         lw $s4, LATE($s0)
         andi $s4, $s4, 0xFFF0
         or   $s4, $s4, $s2
         sw $s4, LATE($s0)

for:    ble $s5, 0, endFor                #   for(; ms > 0; ms--) {

        li $v0, RESET_CORE_TIMER          #     resetCoreTimer();
        syscall

whileT: li $v0, READ_CORE_TIMER           #     while(readCoreTimer() < K);
        syscall                           #
        blt $v0, 20000, whileT            #

        subu $s5, $s5, 1                  #     ms--;
        j for                             # }

endFor: j while

endW:   li $v0, 1
        jr $ra
