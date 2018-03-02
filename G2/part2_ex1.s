# --------------------------------
# Guião 2, Parte 2, Ex 1
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------

          .equ READ_CORE_TIMER, 11
          .equ RESET_CORE_TIMER, 12
          .equ PUT_CHAR, 3
          .equ PRINT_INT, 6

          .data

          .text
          .globl main

main:     li $t0,0

while:                                      # while (1) {
          li $v0, READ_CORE_TIMER           #   while(readCoreTimer() < 20000);
          syscall                           #
          bne $v0, 200000, while            #   Or branch if less than (blt)
                                            #   For 10 HZ, value is  2.000.000
                                            #   For 5  HZ, value is  4.000.000
                                            #   For 1  HZ, value is 20.000.000

          li $v0, RESET_CORE_TIMER          #   resetCoreTimer();
          syscall                           #

          li $a0,' '                        #   putChar(' ');
          li $v0, PUT_CHAR                  #
          syscall                           #

          addi $t0, $t0, 1                  #   printInt(++counter, 10);
          move $a0, $t0                     #
          li $a1, 10                        #
          li $v0, PRINT_INT                 #
          syscall                           #

          j while                           # }
          jr $ra                            #
