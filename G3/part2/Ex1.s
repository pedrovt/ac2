# --------------------------------
# Guião 3, Parte 2, Ex. 1a)
# 4 Bits Up Binary Counter
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------

# Registers Map
# $s0, $s1 -> used to access TRIS/LAT/PORT registers
# $s2 -> counter
# $t0 -> LATE

          .equ SFR_BASE_HI, 0xBF88   # 16 MSbits of SFR area

          .equ TRISE, 0x6100 	       # TRISE address is 0xBF886100
          .equ PORTE, 0x6110	       # PORTE address is 0xBF886110
          .equ LATE, 0x6120 	       # LATE  address is 0xBF886120
          .equ ODCE, 0x6130 	       # ODCE  address is 0xBF886130

          .equ increment, 1          # 1 if up, -1 if down
          .equ ms, 1000              # 1 second to allow easier debugging

          .data

          .text
          .globl main

main:     subu $sp, $sp, 16	         # Save $ra, $s registers
          sw $ra, 0($sp)
          sw $s0, 4($sp)
          sw $s1, 8($sp)
          sw $s2, 12($sp)

          lui $s0, SFR_BASE_HI       # Configures RE ports as outputs
          lw $s1, TRISE($s0)         # $s1 <- TRISE
          andi $s1, $s1, 0xFFF0      # $s1 <- $s1 & 0xFFF0
          sw $s1, TRISE($s0)         # TRISE <- $s1

          li $s2, 0                  # counter = 0;

                                     # 4 Bits Up Binary Counter
while:                               # while(1) {
          li $a0, ms
          jal delay

          addi $s2, $s2, increment   #   counter++;
          andi  $s2, $s2, 0x000F     #   counter <- counter | 0x000F

          lw $t0, LATE($s0)          #   $t0 <- LATE
          andi $t0, $t0, 0xFFF0      #   $t0 <- $t0 & 0xFFF0
          or   $t0, $t0, $s2         #   $t0 <- $t0 | counter
          sw $t0, LATE($s0)          #   LATE <- $t0

          j while                    # }

          lw $ra, 0($sp)             # Restore $ra, $s registers
          lw $s0, 4($sp)
          lw $s1, 8($sp)
          lw $s2, 12($sp)
          addiu $sp, $sp, 16

          li $v0, 1
          jr $ra
