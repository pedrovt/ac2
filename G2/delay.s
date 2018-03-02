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
          .globl delay

delay:                                      # void delay(int ms)  {
          move $t0, $a0

for:      beq $t0, 0, endFor                #   for(; ms > 0; ms--) {

          li $v0, RESET_CORE_TIMER          #     resetCoreTimer();
          syscall

while2:   li $v0, READ_CORE_TIMER          #     while(readCoreTimer() < K);
          syscall                           #
          bne $v0, K, while2                #

          subu $t0, $t0, 1                  #     ms--;
          j for                             # }

endFor:   jr $ra
