# --------------------------------
# Guião 3, Parte 2, Ex. 1c)
# 4 Bits Up/Down Binary Counter
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------

# Registers Map
# $s0, $s1 -> used to access TRIS/LAT/PORT registers
# $s3 -> counter
# $t0 -> LATE
# $t1 -> PORTB

          .equ SFR_BASE_HI, 0xBF88   # 16 MSbits of SFR area

          .equ TRISE, 0x6100 	       # TRISE address is 0xBF886100
          .equ PORTE, 0x6110	       # PORTE address is 0xBF886110
          .equ LATE, 0x6120 	       # LATE  address is 0xBF886120
          .equ ODCE, 0x6130 	       # ODCE  address is 0xBF886130

          .equ TRISB, 0x6040 	       # TRISB address is 0xBF886040
          .equ PORTB, 0x6050	       # PORTB address is 0xBF886050
          .equ LATB, 0x6060 	       # LATB  address is 0xBF886060
          .equ ODCB, 0x6070 	       # ODCB  address is 0xBF886070

          .equ ms, 1000              # 1 second to allow easier debugging

          .data

          .text
          .globl main

main:     subu $sp, $sp, 20	         # Save $ra, $s registers
          sw $ra, 0($sp)
          sw $s0, 4($sp)
          sw $s1, 8($sp)
          sw $s3, 12($sp)
          sw $s3, 16($sp)

          lui $s0, SFR_BASE_HI       # Configures RE ports as outputs
          lw $s1, TRISE($s0)         # $s1 <- TRISE
          andi $s1, $s1, 0xFFF0      # $s1 <- $s1 & 0xFFF0
          sw $s1, TRISE($s0)         # TRISE <- $s1

                                     # Configures RB ports as inputs
          lw $s2, TRISB($s0)         # $s2 <- TRISB
          andi $s2, $s2, 0xFFF0      # $s2 <- $s2 & 0xFFF0
          ori  $s2, $s2, 0x000F      # $s2 <- $s2 | 0x000F
          sw $s2, TRISB($s0)         # TRISB <- $s2

          li $s3, 0                  # counter = 0;

                                     # 4 Bits Up Binary Counter
while:                               # while(1) {
          li $a0, ms
          jal delay

          lw $t1, PORTB($s0)         #   Read from RB3 port
          andi $t1, $t1, 0x0003

if:       beq $t1, 0, else
          addi  $s3, $s3,  1         #   RB3 port = '1' --> UP    (+1)

          li $a0,'+'                 #   Debug
          li $v0, 3
          syscall

          j endIf
else:     addi  $s3, $s3, -1         #   RB3 port = '0' --> DOWN  (-1)

          li $a0,'-'                 #   Debug
          li $v0, 3
          syscall

endIf:    andi  $s3, $s3, 0x000F     #   counter <- counter | 0x000F

          li $a0,' '                 #   Debug
          li $v0, 3
          syscall

          lw $t0, LATE($s0)          #   $t0 <- LATE
          andi $t0, $t0, 0xFFF0      #   $t0 <- $t0 & 0xFFF0
          or   $t0, $t0, $s3         #   $t0 <- $t0 | counter
          sw $t0, LATE($s0)          #   LATE <- $t0

          j while                    # }

          lw $ra, 0($sp)             # Restore $ra, $s registers
          lw $s0, 4($sp)
          lw $s1, 8($sp)
          lw $s3, 12($sp)
          lw $s3, 16($sp)
          addiu $sp, $sp, 20

          li $v0, 1
          jr $ra
