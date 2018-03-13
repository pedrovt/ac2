# --------------------------------
# Guião 3, Parte 1, Ex. 4
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------

# Map of Registers
# $s0 -> v

# ATTENTION !! THE FOLLOWING REGISTERS MUST BE SAVED
# $s1 -> SFR_BASE_HI
# $s2 -> TRISE
# $s3 -> LATE

          .equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area

          .equ TRISE, 0x6100 	# TRISE address is 0xBF886100
          .equ PORTE, 0x6110	# PORTE address is 0xBF886110
          .equ LATE, 0x6120 	# LATE  address is 0xBF886120

          .data

          .text
          .globl main

main:                             # void main(void) {

          subu $sp, $sp, 20	      #    Save $ra, $s registers
          sw $ra, 0($sp)          #
          sw $s0, 4($sp)          #
          sw $s1, 8($sp)          #
          sw $s2, 12($sp)         #
          sw $s3, 16($sp)         #

          li $s0, 0               #    int v = 0;

                                  #    TRISE0 = 0; // Configura o porto RE0 como saída
          lui $s1, SFR_BASE_HI    #
          lw $s2, TRISE($s1)      #       $s2 <- TRISE
          andi $s2, $s2, 0xFFFE   #       $s2 <- $s2 & 0xFFFE (cleans bit 0, ie bit 0 = 0)
          sw $s2, TRISE($s1)      #       TRISE <- $s2

while:                            #    while(1) {
                                  #       LATE0 = v; // Escreve v no bit 0 do porto E
          lw $s3, LATE($s1)       #          $s3 <- LATE
          andi $s3, $s3, 0xFFFE   #          $s3 <- $s3 & 0xFFFE (cleans bit 0, ie bit 0 = 0)
          or $s3, $s3, $s0        #          $s3 <- $s3 | $s0
          sw $s3, LATE($s1)       #          LATE <- $s3

          li $a0, 500             #       delay(500); // Atraso de 500ms
          jal delay

          xori $s0, $s0, 1        #       v ^= 1; // complementa o bit 0 de v (v = v xor 1)

          j while                 #    }

        	lw $ra, 0($sp)          #    Restore $ra, $s registers
          lw $s0, 4($sp)          #
          lw $s1, 8($sp)          #
          lw $s2, 12($sp)         #
          lw $s3, 16($sp)         #
          addiu $sp, $sp, 20      #

          li $v0, 1
          jr $ra                  # }
