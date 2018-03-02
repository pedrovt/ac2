# --------------------------------
# Guião 2, Parte 2, Ex. 3
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

          lui $t1, 0xBF88                   #   Read binary values of the switches
          lw $t2, 0x6050 ($t1)

          move $a0, $t2                     #   Get bits 3 to 0 from register $t2
          andi $a0, $a0, 0x000F

          li $a1, 0x00040002                #   printInt($a0)
          li $v0, PRINT_INT
          syscall

          j while                           # }

        	lw $ra, 0($sp)		                # Restore $ra, $s registers
        	lw $s0, 4($sp)                    #
        	addiu $sp, $sp, 8                 #

          jr $ra                            #

delay:                                      # void delay(int ms)  {
          move $t0, $a0

for:      beq $t0, 0, endFor                #   for(; ms > 0; ms--) {

          li $v0, RESET_CORE_TIMER          #     resetCoreTimer();
          syscall

while2:   li $v0, READ_CORE_TIMER           #     while(readCoreTimer() < K);
          syscall                           #
          bne $v0, K, while2                #

          subu $t0, $t0, 1                  #     ms--;
          j for                             # }

endFor:   jr $ra
