# --------------------------------
# Guião 3, Parte 1, Ex. 1_2_3
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------

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

main:     lui $t0, SFR_BASE_HI
                                 # Configures RB ports as inputs
          lw $t1, TRISB($t0)     # $t1 <- TRISB
          andi $t1, $t1, 0xFFF0  # $t1 <- $t1 & 0xFFF0
          ori  $t1, $t1, 0x000F  # $t1 <- $t1 | 0x000F
          sw $t1, TRISB($t0)     # TRISB <- $t1

                                 # Configures RE ports as outputs
          lw $t2, TRISE($t0)     # $t2 <- TRISE
          andi $t2, $t2, 0xFFF0  # $t2 <- $t2 & 0xFFF0
          sw $t2, TRISE($t0)     # TRISE <- $t2

while:                           # while (1) {
                                 #   Read from RB ports
          lw $t3, PORTB($t0)
          andi $t3, $t3, 0x000F

                                 #   Write to RE ports
          lw $t4, LATE($t0)      #     Read
          andi $t4, $t4, 0xFFF0  #     Modify
          or $t4, $t3, $t4

          sw $t4, LATE($t0)      #     Write

          j while                # }

          jr $ra
