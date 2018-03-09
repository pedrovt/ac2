# --------------------------------
# Guião 3, Parte 2, Ex. 1b)
# 4 Bits Down Binary Counter
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------

# Registers Map
# $s0 -> counter
# $t0, $t1, $t2 -> used to access TRIS/LAT/PORT registers

          .equ SFR_BASE_HI, 0xBF88   # 16 MSbits of SFR area

          .equ TRISE, 0x6100 	       # TRISE address is 0xBF886100
          .equ PORTE, 0x6110	       # PORTE address is 0xBF886110
          .equ LATE, 0x6120 	       # LATE  address is 0xBF886120
          .equ ODCE, 0x6130 	       # ODCE  address is 0xBF886130

          .equ increment, -1         # 1 se crescente, -1 se descrescente
          .equ ms, 250
          .data

          .text
          .globl main

main:     lui $t0, SFR_BASE_HI
                                     # Configures RE ports as outputs
          lw $t1, TRISE($t0)         # $t1 <- TRISE
          andi $t1, $t1, 0xFFF0      # $t1 <- $t1 & 0xFFF0
          sw $t1, TRISE($t0)         # TRISE <- $t1

          subu $sp, $sp, 8	         # Save $ra, $s registers
          sw $ra, 0($sp)
          sw $s0, 4($sp)

          li $s0, 0                  # counter = 0;

                                     # 4 Bits Down Binary Counter
while:                               # while(1) {
          li $a0, ms
          jal delay

          addi $s0, $s0, increment   #   counter++;
          andi  $s0, $s0, 0x000F     #   counter <- counter | 0x000F

          lw $t3, LATE($t0)          #   $t3 <- LATE
          andi $t3, $t3, 0xFFF0      #   $t3 <- $t3 & 0xFFF0
          or   $t3, $t3, $s0         #   $t3 <- $t3 | counter
          sw $t3, LATE($t0)          #   LATE <- $t3

          j while                    # }

          lw $ra, 0($sp)             # Restore $ra, $s registers
          lw $s0, 4($sp)
          addiu $sp, $sp, 8

endWhile: jr $ra
