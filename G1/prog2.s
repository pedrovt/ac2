# --------------------------------
# Guião 1, Ex 2
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------
# Programa 2 – teste do system call "inkey()"

				.equ inkey, 1
				.equ printStr, 8

				.data
str1:		.asciiz "Key pressed\n"

				.text
				.globl main

main:

while:												# while (1) {
				li $v0, inkey					# 	c = inkey();
				syscall

				bne $v0, 0, if				# 	while ((c = inkey()) == 0);
				j while

if:			beq $v0,'\n', endW		# 	if (c == '\n') break;

				la $a0, str1					# 	printStr("Key pressed\n");
				li $v0, printStr
				syscall

				j while								# }

endW:		ori $v0, $0, 0				# return 0
				jr $ra
