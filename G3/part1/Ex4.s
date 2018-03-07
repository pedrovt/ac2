# --------------------------------
# Guião 3, Parte 1, Ex. 4
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------

# v -> $s0

          .equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area

          .equ TRISB, 0x6040 	# TRISB address is 0xBF886040
          .equ PORTB, 0x6050	# PORTB address is 0xBF886050
          .equ LATB, 0x6060 	# LATB  address is 0xBF886060
          .equ ODCB, 0x6070 	# ODCB  address is 0xBF886070

          .equ TRISE, 0x6100 	# TRISE address is 0xBF886100
          .equ PORTE, 0x6110	# PORTE address is 0xBF886110
          .equ LATE, 0x6120 	# LATE  address is 0xBF886120
          .equ ODCE, 0x6130 	# ODCE  address is 0xBF886130

          .data

          .text
          .globl main

main:                             # void main(void) {

          subu $sp, $sp, 8	      #    Save $ra, $s registers
          sw $ra, 0($sp)          #
          sw $s0, 4($sp)          #

          li $s0, 0               #    int v = 0;

                                  #    TRISE0 = 0; // Configura o porto RE0 como saída
          lw $t1, TRISE($t0)      #       $t1 <- TRISE
          andi $t1, $t1, 0xFFFE   #       $t1 <- $t1 & 0xFFFE (cleans bit 0, ie bit 0 = 0)
          sw $t1, TRISE($t0)      #       TRISE <- $t1


while:                            #    while(1) {
                                  #       LATE0 = v; // Escreve v no bit 0 do porto E
          lw $t2, LATE($t0)       #          $t2 <- LATE
          andi $t2, $t2, 0xFFFE   #          $t2 <- $t2 & 0xFFFE (cleans bit 0, ie bit 0 = 0)
          or $t2, $t2, $s0        #          $t2 <- $t2 | $s0
          sw $t2, LATE($t0)       #          LATE <- $t2

          li $a0, 500             #       delay(500); // Atraso de 500ms
          jal delay

          xori $s0, $s0, 1        #       v ^= 1; // complementa o bit 0 de v (v = v xor 1)

          j while                 #    }

        	lw $ra, 0($sp)          #    Restore $ra, $s registers
          lw $s0, 4($sp)          #
          addiu $sp, $sp, 8       #

          jr $ra                  # }
