# --------------------------------
# Guião 2, Parte 2, Ex 2_Test
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------

# $s0 : counter

          .equ READ_CORE_TIMER, 11
          .equ RESET_CORE_TIMER, 12
          .equ PUT_CHAR, 3
          .equ PRINT_INT, 6
          .equ ms, 10
          .equ K, 20000

          .data

          .text
          .globl main

main:     li $t0,0

          subu $sp, $sp, 8	                # Save $ra, $s registers
          sw $ra, 0($sp)                    #
          sw $s0, 4($sp)                    #

while:                                      # while (1) {
          li $a0, ms                        #   void delay(ms);
          jal delay

          li $a0,' '                        #   putChar(' ');
          li $v0, PUT_CHAR                  #
          syscall                           #

          addi $s0, $s0, 1                  #   printInt(++counter, 10);
          move $a0, $s0                     #
          li $a1, 10                        #
          li $v0, PRINT_INT                 #
          syscall                           #

          j while                           # }

        	lw $ra, 0($sp)		                # Restore $ra, $s registers
          lw $s0, 4($sp)                    #
        	addiu $sp, $sp, 8                 #

          jr $ra                            #
