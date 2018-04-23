# --------------------------------
# Teste Prático 2015 1a)
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# Agradecimento a Rui Lopes, MIEET
# --------------------------------

# ~Importante aqui: módulo é o nº shifts
#  Módulo no mcplmento para 2, se num <0 , é != |num|

          .equ SFR_BASE_HI, 0xBF88   # 16 MSbits of SFR area

          .equ TRISE, 0x6100 	       # TRISE address is 0xBF886100
          .equ PORTE, 0x6110	       # PORTE address is 0xBF886110
          .equ LATE, 0x6120 	       # LATE  address is 0xBF886120
          .equ ODCE, 0x6130 	       # ODCE  address is 0xBF886130

          .equ TRISB, 0x6040 	       # TRISB address is 0xBF886040
          .equ PORTB, 0x6050	       # PORTB address is 0xBF886050
          .equ LATB, 0x6060 	       # LATB  address is 0xBF886060
          .equ ODCB, 0x6070 	       # ODCB  address is 0xBF886070

          .equ ms, 1000              # 1 second to allow easier debugging

          .data

          .text
          .globl main

main:     subu $sp, $sp, 16	         # Save $ra, $s registers
          sw $ra, 0($sp)
          sw $s0, 4($sp)
          sw $s1, 8($sp)
          sw $s2, 12($sp)

          lui $s0, SFR_BASE_HI

                                     # Configures RE0-3 ports as outputs
          lw $s1, TRISE($s0)         # $s1 <- TRISE
          andi $s1, $s1, 0xFFF0      # $s1 <- $s1 & 0xFFF0
          sw $s1, TRISE($s0)         # TRISE <- $s1

                                     # Configures RB0-3 ports as inputs
          lw $s2, TRISB($s0)         # $s2 <- TRISB
          andi $s2, $s2, 0xFFF0      # $s2 <- $s2 & 0xFFF0
          ori  $s2, $s2, 0x000F
          sw $s2, TRISB($s0)         # TRISB <- $s2

while:                               # while(1) {
          lw $t1, PORTB($s0)         #   Read from RB0-3 port
          andi $t1, $t1, 0x000F

                                     #   Read current state of RE port
          lw $t2, LATE($s0)          #    $t2 <- LATE
          andi $t2, $t2, 0xFFF0      #    $s1 <- $s1 & 0xFFF0

          andi $t3, $t1, 0x0008      #   Obter sinal do número     -> $t3

          andi $t4, $t1, 0x0007      #   Obter módulo do número    -> $t4

if:       beq  $t3, 0x0008, endIf    #   numero positivo --> shift amount é o 3 - módulo (lógica "inversa" -> 0 shifts => número +3/-3)
          li   $t6, 0x0003
          subu $t4, $t6, $t4

endIf:                               #   numero negativo --> shift amount é o módulo

          # move $a0,$t4             #   Debug
          # li $v0, 7
          # syscall
          #
          # li $a0,'\n'
          # li $v0, 3
          # syscall

          li   $t5, 0x0007           #   modulo = 111 (em binário)

          srlv  $t5, $t5, $t4

          or   $t5, $t5, $t3
          sw $t5, LATE($s0)          #   LATE <- $t1

          j while                    # }

          lw $ra, 0($sp)             # Restore $ra, $s registers
          lw $s0, 4($sp)
          lw $s1, 8($sp)
          lw $s2, 12($sp)
          addiu $sp, $sp, 16

          li $v0, 1
          jr $ra
