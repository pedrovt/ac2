# --------------------------------
# Delay function
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------

# Map of registers
# $t0 -> counter

          .equ READ_CORE_TIMER, 11
          .equ RESET_CORE_TIMER, 12
          .equ K, 20000

          .data

          .text
          .globl delay

delay:                                      # void delay(int ms)  {
          move $t0, $a0

for:      ble $t0, 0, endFor                #   for(; ms > 0; ms--) {

          li $v0, RESET_CORE_TIMER          #     resetCoreTimer();
          syscall

while:    li $v0, READ_CORE_TIMER           #     while(readCoreTimer() < K);
          syscall                           #
          blt $v0, K, while                 #

          subu $t0, $t0, 1                  #     ms--;
          j for                             # }

endFor:   jr $ra
