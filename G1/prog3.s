# --------------------------------
# Guião 1, Ex 3
# Arquitectura de Computadores II
# Pedro Teixeira, 84715, MIECT
# --------------------------------
# Programa 3 – teste dos system calls "getChar()" e "putChar()"

        .equ getChar, 2
        .equ putChar, 3

        .data

        .text
        .globl main

main:                       # int main(void) {
                            #   char c;
while:                      #   while (1) {
        li $v0, getChar     #     c = getChar();
        syscall

if:     beq $v0,'\n', endW  #     if (c == '\n') break;

        or $a0, $0, $v0     #     putChar(c);
        li $v0, putChar
        syscall

        j while             #   }

endW:   ori $v0, $0, 1       #   return 1;
        jr $ra              # }
