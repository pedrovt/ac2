# --------------------------------
# Guião 2, Parte 2, Ex. 3
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------

# Map of registers
# $t1 -> binary value of the 4 switches

          .equ PUT_CHAR, 3
          .equ PRINT_INT, 6
          .equ ms, 100                      # 100 ms <-> 2Hz

          .data

          .text
          .globl main

main:     subu $sp, $sp, 4	                # Save $ra register
          sw $ra, 0($sp)                    #

while:                                      # while (1) {
          lui $t0, 0xBF88                   #   Read binary values of the switches
          lw $t1, 0x6050 ($t0)

          andi $t1, $t1, 0x000F             #   Get bits 3 to 0 from register $t1

          move $a0, $t1                     #   printInt($a0)
          li $a1, 0x00040002
          li $v0, PRINT_INT
          syscall

          li $a0,'\n'                       #   putChar('\n');
          li $v0, PUT_CHAR                  #
          syscall

          li $a0, ms                        #   void delay(ms);
          jal delay

          j while                           # }

        	lw $ra, 0($sp)		                # Restore $ra register
        	addiu $sp, $sp, 4                 #

          jr $ra                            #
