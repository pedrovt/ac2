# --------------------------------
# Guião 2, Parte 2, Ex 1
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------

# Map of registers
# $t0 -> counter

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
          blt $v0, 200000,   while          #   For 100 HZ, value is    200.000
          #blt $v0, 2000000,  while         #   For 10  HZ, value is  2.000.000
          #blt $v0, 4000000,  while         #   For 5   HZ, value is  4.000.000
          #blt $v0, 20000000, while         #   For 1   HZ, value is 20.000.000

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
